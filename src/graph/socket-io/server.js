var express = require('express')
var process = require('process');
var util = require('util');
var app = express();
var http = require('http').Server(app);

const lidar_data_line = "lidar_line"
const gyro_data_line = "gyro_line"
const pid_data_line = "pid_line"
const esc_data_line = "esc_line"
const batt_data_line = "batt_line"

const buffer_offset = 4;

//TESTING UI LOOK, REMOVE
function getRandomIntInclusive(min, max)
{
    min = Math.ceil(min);
    max = Math.floor(max);
    // The maximum is inclusive and the minimum is inclusive
    return Math.floor(Math.random() * (max - min + 1)) + min;
}

//port to receive data
var io_3000 = require('socket.io')(http);
app.use(express.static("./"));
app.get('/', function(req, res) {
    res.sendFile(__dirname + '/index.html');
});

io_3000.on('connection', (socket) => {
    console.log('A user is connected to receive data -- id = ' + socket.id);
    var server_status = 1;
    io_3000.emit("server_status", server_status);
    socket.on('disconnect', function() {
        server_status = 0;
        console.log('A user is disconnected to receive data -- id = ' + socket.id);
        io_3000.emit("server_status", server_status);
    });
});

http.listen(3000, function(){
    console.log('Listening on HTTP port: 3000');
});

//ABOVE = UI CONFIG
//BELOW = TCP CONNECTION TO QUADCOPTER

//TCP CONFIG
var net = require('net');
var HOST = '';
var MANIFEST_PORT = 5000;
var DATA_PORT = 6000;

var ind_buff_size = 128;

//MANNIFEST TCP CONNECTION SETUP
var manifest_server = net.createServer();
manifest_server.listen(MANIFEST_PORT, HOST);
console.log('Listening on Manifest Port: '+ MANIFEST_PORT)

//DATA server
var data_server = net.createServer();

//DATA TCP CONNECTION SETUP
data_server.listen(DATA_PORT, HOST);
console.log('listening on Data port: '+ DATA_PORT)

//MANIFEST TABLE
var manifest_arr = [];

function handle_manifest_data(data)
{
    manifest_lines = data.toString("utf-8").replace("\r\n", "").split(",");
    var channel_object = {
        "index"     :  manifest_lines[0],
        "name"      :  manifest_lines[1],
        "units"     :  manifest_lines[2],
        "data_type" :  manifest_lines[3],
        "size"      :  manifest_lines[4]
    };
    console.log(channel_object);
    manifest_arr.push(channel_object)
    io_3000.emit('manifestLine', manifest_arr);
}

//GET INCOMING MANIFESET DATA
manifest_server.on('connection', function(sock)
{
    console.log('CONNECTED to TCP on port 5000');
    sock.on('data', handle_manifest_data);
    sock.on('close', (data) => {
        console.log('TCP connection closed with Manifest Server');
    });
});

function increment_byte_index(curr_byte,max_byte)
{
    //get next chunk of data (32 bits, 4 bytes)
	  if (curr_byte < max_byte)
		    curr_byte += 4;
	  return curr_byte;
}

function handle_telemetry(data)
{
    console.log("Size of the incoming buffer: " + data.length)
    var byte_index = 0;
    var max_byte = data.length;
    var channel_count = data.readUInt32LE(byte_index);
    byte_index = increment_byte_index(byte_index,max_byte);
    var total_datasize_bytes = data.readUInt32LE(byte_index);
    byte_index = increment_byte_index(byte_index,max_byte);
    var checksum = data.readUInt32LE(byte_index);
    byte_index = increment_byte_index(byte_index,max_byte);
    var channel_index_arr = []
    var data_arr = []

    //LOOP ONE TO CREATE CHANNEL INDEX ARRAY
    for(i = 0; i<channel_count; i++) {
        var index = data.readUInt32LE(byte_index);
        channel_index_arr.push(index)
        byte_index = increment_byte_index(byte_index,max_byte);
    }
    //LOOP TWO TO EXTRACT ALL OF THE DATA
    for(i = 0; i < channel_count; i++) {
        var curr_data = data.readUInt32LE(byte_index);
        data_arr.push(curr_data)
        byte_index = increment_byte_index(byte_index,max_byte);
    }
    for(i = 0; i<channel_count; i++) {
        var channel_index = channel_index_arr[i]
        var emit_line = manifest_arr[channel_index].name.toString()
        console.log(emit_line.length)
        var data_to_emit = data_arr[i]
        console.log(data_to_emit)
        io_3000.emit(emit_line,data_to_emit)
    }
    //Updates Green light
    var data_status = 1
    io_3000.emit("data_status", data_status);
}

function handle_connection_close(data)
{
    data_status = 0;
    io_3000.emit("data_status", data_status);
    console.log('TCP connection closed with Manifest Server');
}

data_server.on('connection', function(sock) {
    console.log('CONNECTED to TCP via port 6000');
    sock.on('data', handle_telemetry);
	  sock.on('close', handle_connection_close);
});

function print_array(arr)
{
    for (i = 0; i<arr.length;i++)
    {
        console.log("-------------------------------------------");
        console.log("Channel Index: " + arr[i].index);
        console.log("Channel Type: " + arr[i].data_type);
        console.log("Channel Size: " + arr[i].size);
        console.log("Channel Name: " + arr[i].name);
        console.log("Channel Units: " + arr[i].units);
	  }
}
