var express = require('express')
var process = require('process');
var util = require('util');

const lidar_data_line = "lidar_line"
const gyro_data_line = "gyro_line"
const pid_data_line = "pid_line"
const esc_data_line = "esc_line"
const batt_data_line = "batt_line"

const buffer_offset = 4;

//MANIFEST TABLE
var manifest_arr = [];

/* initialize express */
var app = express();
var http = require('http').Server(app);
app.use(express.static("./"));
app.get('/', function(req, res) {
    res.sendFile(__dirname + '/index.html');
});

//port to receive data
var socket_io = require('socket.io')(http);

socket_io.on('connection', (socket) => {

    console.log(`client ${socket.id} connected`);
    var server_status = 1;
    socket_io.emit("server_status", server_status);

    /* handle this client disconnecting */
    socket.on('disconnect', function() {
        server_status = 0;
        console.log(`client ${socket.id} disconnected`);
        socket_io.emit("server_status", server_status);
    });

    /* send this client the current manifest */
    if (manifest_arr.length > 1)
    {
        console.log(`sent ${socket.id} the existing channel manifest`);
        socket.emit('manifestLine', manifest_arr);
    }
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

//MANNIFEST TCP CONNECTION SETUP
var manifest_server = net.createServer();
manifest_server.listen(MANIFEST_PORT, HOST);
console.log('Listening on Manifest Port: '+ MANIFEST_PORT)

//DATA server
var data_server = net.createServer();

//DATA TCP CONNECTION SETUP
data_server.listen(DATA_PORT, HOST);
console.log('listening on Data port: '+ DATA_PORT)

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

    /* check if we already have this channel to support re-transmission */
    let channel_present = false;
    for (let i in manifest_arr)
    {
        if (manifest_arr[i]["name"] == channel_object["name"])
        {
            channel_present = true;
            break;
        }
    }
    if (!channel_present)
    {
        console.log("new channel:");
        console.log(channel_object);
        manifest_arr.push(channel_object)
        socket_io.emit('manifestLine', [channel_object]);
    }
    else console.log(`got duplicate channel '${channel_object["name"]}'`);
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
        socket_io.emit(emit_line,data_to_emit)
    }
    //Updates Green light
    var data_status = 1
    socket_io.emit("data_status", data_status);
}

function handle_connection_close(data)
{
    data_status = 0;
    socket_io.emit("data_status", data_status);
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
