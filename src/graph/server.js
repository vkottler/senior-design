let express = require('express')
var app = express()
var http = require('http').Server(app);
var io = require('socket.io')(http);

app.use(express.static('./public'))

//app.use('/', express.static('public'));

//First route
app.get('/', function (req, res) {
    res.sendfile('./public/test.html')
    
});

app.get('/json', function (req, res) {
    res.sendfile('./public/jsontest.html')
    
});

io.on('connection', function(socket){
    console.log('a user connected');
  });
  
  http.listen(5000, function(){
    console.log('listening on *:5000');
  });

// Listen to port 5000
//app.listen(5000, function () {
  //  console.log('App is listening on port 5000!');
//});

