let express = require('express')
var app = express()

app.use(express.static('./public'))

//app.use('/', express.static('public'));

//First route
app.get('/', function (req, res) {
    res.sendfile('./public/test.html')
    
});

app.get('/json', function (req, res) {
    res.sendfile('./public/jsontest.html')
    
});

// Listen to port 5000
app.listen(5000, function () {
    console.log('App is listening on port 5000!');
});

