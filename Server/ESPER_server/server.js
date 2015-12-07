var router = require('./router.js');
var net = require('net');
var connect = require('connect');

var port = 8000;
var IP = '165.132.120.150';

var timeout = 60000;

function start(route, handle) {
  var server = net.createServer(function(socket) {
	  socket.setEncoding('utf8');
	  //this.timeout(timeout);
  });
  
  server.on('listening', function() {
	  console.log('Server Running at Port = ' + port);
	  console.log(server.address().address);
  });
  
  server.on('connection', function(socket) {
	  console.log('New connection');
	  console.log('client info = %s: %d', socket.remoteAddress, socket.remotePort);
	  
	  socket.on('data', function(JSONData) {
		  console.log(JSONData);		  
		  var data = JSON.parse(JSONData);
		  
		  router.route(handle, data, socket);
	  });
	  
	  socket.on('timeout', function(){
		  socket.write('timeout');
	  });
	  
	  socket.on('error', function() {
		  console.log('Socket error : ', JSON.stringify(error));
	  });
	  
	  socket.on('end', function() {
		  console.log('connection terminated');
	  });
  });
  
  server.on('close', function(){
	  console.log('server closed');
  });
  
  server.listen(port, IP, function(){
	  console.log('running');
  });
  
  // To manage session
  //server = connect().use(connect.cookieParser());
  /*
  server.use(connect.cookieParser());
  server.use(connect.session({
	  secret: 'string',
	  key: 'another sid',
	  cookie: {
		  maxAge: 60*1000
	  }
  }));
  */
  
}

exports.start = start;