let express = require('express')
var app = express();
var http = require('http').Server(app);

app.use(express.static('/public'))



function getRandomIntInclusive(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min; //The maximum is inclusive and the minimum is inclusive 
}

var io = require('socket.io')(http);

app.get('/', function(req, res){
  res.sendFile(__dirname + '/index.html');
});


io.on('connection', (socket) => {
  console.log('A user is connected');
  socket.on('disconnect', function(){
    console.log('A user is disconnected');
  });
});

var count = 0; 
setInterval(function(){
  var data_set = []; 
  data_set.push({
    x: count++, 
    y: getRandomIntInclusive(1,4)
  });
  io.emit('dataLine', data_set); 
}, 1000);


http.listen(3000, function(){
  console.log('listening on *:3000');
});