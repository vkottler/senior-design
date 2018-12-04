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
var manifest = [];

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
    if (manifest.length > 1)
    {
        console.log(`sent ${socket.id} the existing channel manifest`);
        socket.emit('manifestLine', manifest);
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
        "index"     :  Number(manifest_lines[0]),
        "name"      :  manifest_lines[1],
        "units"     :  manifest_lines[2],
        "data_type" :  manifest_lines[3],
        "size"      :  Number(manifest_lines[4])
    };

    /* check if we already have this channel to support re-transmission */
    let channel_present = false;
    for (let i in manifest)
    {
        if (manifest[i]["name"] == channel_object["name"])
        {
            channel_present = true;
            break;
        }
    }
    if (!channel_present)
    {
        console.log("new channel:");
        console.log(channel_object);
        manifest[channel_object["index"]] = channel_object;
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

function handle_telemetry(data)
{
    let index = 0;

    let channel_count = data.readUInt32LE(index);
    index += 4;

    let data_size = data.readUInt32LE(index);
    index += 4;

    let checksum = data.readUInt32LE(index);
    index += 4;

    /* get channels by index */
    let channels = [];
    for (let i = 0; i < channel_count; i++)
    {
        let channel_index = data.readUInt32LE(index);
        if (channel_index >= manifest.length)
        {
            let msg = `packet index ${channel_index} larger than manifest size ${manifest.length}`;
            console.log(msg);
            return;
        }
        index += 4;
        channels.push(manifest[channel_index]);
    }

    /* read data according to channel size */
    for (let i = 0; i < channel_count; i++)
    {
        let current_channel_name = channels[i]["name"];
        let current_channel_size = channels[i]["size"];
        let current_channel_type = channels[i]["data_type"];
        let channel_data;
        switch (current_channel_type)
        {
            case "INT8":
                channel_data = data.readInt8(index);
                break;
            case "UINT8":
                channel_data = data.readUInt8(index);
                break;
            case "INT16":
                channel_data = data.readInt16LE(index);
                break;
            case "UINT16":
                channel_data = data.readUInt16LE(index);
                break;
            case "INT32":
                channel_data = data.readInt32LE(index);
                break;
            case "UINT32":
                channel_data = data.readUInt32LE(index);
                break;
            case "FLOAT":
                channel_data = data.readFloatLE(index);
                break;
            case "STRING":
                channel_data = data.toString("utf-8", index, index + current_channel_size);
                break;
            default:
                channel_data = "error";
        }
        index += current_channel_size;
        console.log(`${current_channel_name}: ${channel_data}`);
        socket_io.emit(current_channel_name, channel_data);
    }

    // Updates Green light
    var data_status = 1;
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
