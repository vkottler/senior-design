var express = require('express')
var process = require('process');
var util = require('util');
var app = express();
var http = require('http').Server(app);


//TESTING UI LOOK, REMOVE 
function getRandomIntInclusive(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min; //The maximum is inclusive and the minimum is inclusive 
}

//TCP Connection 
var net = require('net');
//var HOST = '10.42.0.79';
var HOST = 'localhost'
var PORT = 5000;

app.use(express.static("./JS"))

//port to receive data 
var io_3000 = require('socket.io')(http);


app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

io_3000.on('connection', (socket) => {
  console.log('A user is connected to receive data -- id = ' + socket.id);
  /*TESTING PURPOSE, REMOVE*/ 
  
  setInterval(function(){  
    var incoming_data = []
    incoming_data.push({
      y: getRandomIntInclusive(1,4)
    });
    io_3000.emit('testdataLine', incoming_data);  
  }, 1000);
  /*TESTING PURPOSE, REMOVE*/ 

  socket.on('disconnect', function(){
    console.log('A user is disconnected to receive data -- id = ' + socket.id);
  });
});

http.listen(3000, function(){
  console.log('listening on *:3000');
});   

//TCP CONNECTION 
var server = net.createServer();
server.listen(PORT, HOST);
console.log('listening on port: '+ PORT)

function send_and_print(x_data, y_data, z_data)
{
  process.stdout.clearLine();
  process.stdout.cursorTo(0);
  process.stdout.write(util.format("x: %i\ty: %i\tz: %i", x_data, y_data, z_data));
  var incoming_data = []
  incoming_data.push({
    x: x_data,
    y: y_data,
    z: z_data
  });
  io_3000.emit('dataLine', incoming_data);
}

server.on('connection', function(sock) {
    console.log('CONNECTED to TCP');
    var x_data = -1;
    var y_data = -1;
    var z_data = -1;
    var x_ready = false;
    var y_ready = false;
    var z_ready = false;
    var bytes_recv = 0;
    sock.on('data', (data) => {

      var bytes_consumed = 0;
      var initial_offset = bytes_recv % 6;

      while (bytes_consumed < data.length)
      {
        var curr_var = (initial_offset + bytes_consumed) % 6;

        if (curr_var == 0)
        {
          x_data = data.readInt16LE(bytes_consumed);
          x_ready = true;
        }
        else if (curr_var == 2)
        {
          y_data = data.readInt16LE(bytes_consumed);
          y_ready = true;
        }
        else if (curr_var == 4)
        {
          z_data = data.readInt16LE(bytes_consumed);
          z_ready = true;
        }

        bytes_consumed += 2;

        /* send (and print) the variables if they're ready */
        if (x_ready && y_ready && z_ready)
        {
          console.log(x_data)
          console.log(y_data)
          console.log(z_data)

          send_and_print(x_data, y_data, z_data);
          x_ready = false;
          y_ready = false;
          z_ready = false;
        }
      }

      bytes_recv += data.length;
    });

});
server.on('disconnect', function() {
  console.log('TCP connection closed');
});


//testing to visualize final ui layout (send random number every second, every telem will use same data)
