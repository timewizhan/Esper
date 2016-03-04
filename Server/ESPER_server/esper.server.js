var net = require('net');
var connect = require('connect');

var PORT = 8002;
var IP = '165.132.120.150';

function start(route, reqHandle, reqMysqlDB, reqEmail) {
	var server = net.createServer(function(socket) {
		socket.setEncoding('utf8');
	});

	server.on('listening', function() {
		console.log('Server Running at port = ' + PORT);
		console.log(server.address().address);
	});

	server.on('connection', function(socket) {
		console.log('New connection');
		console.log('client info = %s: %d', socket.remoteAddress,
				socket.remotePort);

		socket.on('data', function(data) {
			// console.log(data);
			route(data, reqHandle, reqMysqlDB, reqEmail, socket);
		});

		socket.on('error', function(error) {
			console.log('Socket error : ', error);
		});

		socket.on('end', function() {
			console.log('connection terminated');
		});
	});

	server.on('close', function() {
		console.log('server closed');
	});

	server.listen(PORT, IP, function() {
		console.log('Server Running at Port ' + PORT);
	});
}

exports.start = start;
