let express = require('express')
let process = require('process');
let util = require('util');
var app = express();
var http = require('http').Server(app);

//TCP SHIT
var net = require('net');
var HOST = '10.42.0.79';
var PORT = 5000;

app.use(express.static("./JS"))

//port to receive data 
var io_3000 = require('socket.io')(http);


app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});

io_3000.on('connection', (socket) => {
  console.log('A user is connected to receive data -- id = ' + socket.id);
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
    let x_data = -1;
    let y_data = -1;
    let z_data = -1;
    let x_ready = false;
    let y_ready = false;
    let z_ready = false;
    let bytes_recv = 0;
    sock.on('data', (data) => {

      let bytes_consumed = 0;
      let initial_offset = bytes_recv % 6;

      while (bytes_consumed < data.length)
      {
        let curr_var = (initial_offset + bytes_consumed) % 6;

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

