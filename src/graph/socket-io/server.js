var express = require('express')
var process = require('process');
var util = require('util');
var app = express();
var http = require('http').Server(app);

const accel_data_line = "accel_line"
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
	//DEMO PURPOSE TO POPULATE GRAPH 
	/*setInterval(function(){  
		var incoming_accelgyro_data = []
		incoming_accelgyro_data.push({
			x: getRandomIntInclusive(1,3),
			y: getRandomIntInclusive(4,6),
			z: getRandomIntInclusive(7,9)
		});
		io_3000.emit(accel_data_line, incoming_accelgyro_data); 		
	}, 100);*/
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
//var HOST = '10.141.65.106';		//death-star IP
var HOST = '172.16.42.157';		//apt IP 
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
		var max_str_len = 32; 
		
		console.log("Total buffer size: " + byte_len);
		for(i = 0; i < byte_len; i += ind_buff_size)	
		{	
			var channel_index = data.readUInt32LE(i)  
			var channel_type = data.readUInt32LE(i+buffer_offset)
			var channel_size = data.readUInt32LE(i+(2*buffer_offset))
			var channel_string = data.toString('utf8',i,i+max_str_len);
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
    manifest_arr = []; 
    });
	sock.on('close', (data) => {
		console.log('TCP connection closed with Manifest Server');	
	});
});


//GET INCOMING STREAM OF DATA 
/* This is the rought impl of incomign data 
data_server.on('connection', function(sock) {
    console.log('CONNECTED to TCP via port 6000');
    sock.on('data', (data) => {
		
		//TODO: See what type of data is coming in, forward it on that line (check indexs and size)
		var curr_byte = 0 
		var channel_count = data.readUInt32LE(curr_byte);	//see number of channels in packet
		curr_byte = curr_byte + 4
		var packet_total_size = data.readUInt32LE(curr_byte)
		curr_byte = curr_byte + 4
		var channel_indexes = []; //see all the channels in packet
		//find the indexs of the channels 
		for(i = 0; i < channel_count;i++ ){
			channel_indexes[i] = data.readUInt32LE(curr_byte)
			curr_byte = curr_byte + 4
		}
		console.log(channel_count)
		console.log(packet_total_size)
		console.log(channel_count[1])
		//should be at the start of data bytes 
		switch(true) {
			//ACCEL DATA
			case (channel_indexes[0] < 3):		
				break;
			//GYRO DATA
			case (channel_indexes[0] < 6):
				io_3000.emit(gyro_data_line, data); 		
				break;	
			//PID DATA
			case (channel_indexes[0] < 9):
				io_3000.emit(pid_data_line, data); 		
				break;
			//ESC DATA
			case (channel_indexes[0] < 13):
				io_3000.emit(esc_data_line, data); 		
				break;	
			//BATT DATA
			case (channel_indexes[0] < 18):
				io_3000.emit(batt_data_line, data); 		
				break;	
			default:
					console.log('No telem found')
			}	
	});
});
*/

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
		for(i = 0; i<channel_count; i++)
		{
			var index = data.readUInt32LE(byte_index);
			channel_index_arr.push(index)
			byte_index = increment_byte_index(byte_index,max_byte);
		}
		//LOOP TWO TO EXTRACT ALL OF THE DATA
		for(i = 0; i<channel_count; i++)
		{
			var curr_data = data.readUInt32LE(byte_index);
			data_arr.push(curr_data)
			byte_index = increment_byte_index(byte_index,max_byte);
		}

		//FORWARD DATA ON CORRESPONDING LINE  
		switch(channel_index_arr.length) {
			//ACCEL DATA/GYRO
			case (6):
				console.log('here')
				var incoming_accel_data = []
				incoming_accel_data.push({
					x: data_arr[0],
					y: data_arr[1],
					z: data_arr[2]
				})
				var incoming_gyro_data = []
				incoming_gyro_data.push({
					x: data_arr[3],
					y: data_arr[4],
					z: data_arr[5]
				})
				io_3000.emit(accel_data_line, incoming_accel_data);
				io_3000.emit(gyro_data_line, incoming_gyro_data); 			
				break;	
			//PID DATA
			case (4): //need to change to 3 
				io_3000.emit(pid_data_line, data); 		
				break;
			//ESC DATA
			case (4):
				io_3000.emit(esc_data_line, data); 		
				break;	
			//BATT DATA
			case (5):
				io_3000.emit(batt_data_line, data); 		
				break;	
			default:
					console.log('No telem found')
			}	

		/*for(i = 0; i < data.length-3; i+=buffer_offset)
		{
			console.log('data: ' + data.readUInt32LE(i))
		}*/


		var data_status = 1 
   		io_3000.emit("data_status", data_status); 	
	
	});
	sock.on('close', (data) => {
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
