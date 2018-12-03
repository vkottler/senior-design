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
function getRandomIntInclusive(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min; //The maximum is inclusive and the minimum is inclusive 
}

app.use(express.static("./JS"))

//port to receive data 
var io_3000 = require('socket.io')(http);
app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

io_3000.on('connection', (socket) => {
  console.log('A user is connected to receive data -- id = ' + socket.id);
   var server_status = 1 
   io_3000.emit("server_status", server_status); 		
  socket.on('disconnect', function(){
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
//var HOST = '192.168.1.2';    	//PI IP
var HOST = '10.141.65.106';		//death-star IP
//var HOST = '172.16.42.157';		//apt IP 
var MANIFEST_PORT = 5000;
var DATA_PORT = 6000; 

var ind_buff_size = 256; 
 
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

//GET INCOMING MANIFESET DATA 
manifest_server.on('connection', function(sock) {
    console.log('CONNECTED to TCP on port 5000');
    sock.on('data', (data) => {
		var byte_len = data.byteLength
		
		//indexing into buffer (vaughn will throw up, need to change these in future)
		var max_str_len = 20; 
		
		console.log("Total buffer size: " + byte_len);
		for(i = 0; i < byte_len; i += ind_buff_size)	
		{	
			var channel_index = data.readUInt32LE(i)  
			var channel_type = data.readUInt32LE(i+buffer_offset)
			var channel_size = data.readUInt32LE(i+(2*buffer_offset))
			var channel_string = data.toString('utf8',i+(3*buffer_offset),i+(3*buffer_offset)+max_str_len);
			var split = channel_string.split(',');
			var channel_name = split[0]; 
			var channel_units = split[1];
			var channel_object = { 
				"index"  	:  channel_index, 
				"data_type"	:  channel_type, 
				"size"      :  channel_size,
				"name"      :  channel_name,
				"units"     :  channel_units
			  }
			manifest_arr.push(channel_object)
		}
    print_array(manifest_arr);
    io_3000.emit('manifestLine', manifest_arr); 
    });
	sock.on('close', (data) => {
		console.log('TCP connection closed with Manifest Server');	
	});
});

function increment_byte_index(curr_byte,max_byte){
	if(curr_byte < max_byte)
	{
		curr_byte += 4;	//get next chunk of data (32 bits, 4 bytes)
	}
	return curr_byte;
}

data_server.on('connection', function(sock) {
    console.log('CONNECTED to TCP via port 6000');
    sock.on('data', (data) => {
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
	
	});
	sock.on('close', (data) => {
		//Updates red light
		data_status = 0 
		io_3000.emit("data_status", data_status); 
		console.log('TCP connection closed with Manifest Server');	
	});
});


function print_array(arr){
	for(i = 0; i<arr.length;i++){
		console.log("-------------------------------------------")
		console.log("Channel Index: " + arr[i].index)
		console.log("Channel Type: " + arr[i].data_type)
		console.log("Channel Size: " + arr[i].size)
		console.log("Channel Name: " + arr[i].name)
		console.log("Channel Units: " + arr[i].units)
	}
}
