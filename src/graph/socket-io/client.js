var net = require('net');

var HOST = 'localhost';
var PORT = 5000;

function getRandomIntInclusive(min, max) {
  min = Math.ceil(min);
  max = Math.floor(max);
  return Math.floor(Math.random() * (max - min + 1)) + min; //The maximum is inclusive and the minimum is inclusive 
}


var client = new net.Socket();
client.connect(PORT, HOST, function() {
    console.log('CONNECTED TO: ' + HOST + ':' + PORT);
    /*SENDING RANDOM DATA*/
    var count = 0; 
    
    setInterval(function(){
    var x = count++    
    var y =  getRandomIntInclusive(1,4)
    const buf = Buffer.from([x, y]);
    client.write(buf);
    }, 1000);

    
});

