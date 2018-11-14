let express = require('express')
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

server.on('connection', function(sock) {
    console.log('CONNECTED to TCP');
    sock.on('data', function(data) {
     // for (const b of data) {
      console.log(data.readInt32LE(0));
      console.log(data.readInt32LE(4));
      console.log(data.readInt32LE(8));
     // }
      var incoming_data = []
      incoming_data.push({
        x: data[0], 
        y: data[1] 
      });
      io_3000.emit('dataLine', incoming_data); 
    }); 

});
server.on('disconnect', function() {
  console.log('TCP connection closed');
});

