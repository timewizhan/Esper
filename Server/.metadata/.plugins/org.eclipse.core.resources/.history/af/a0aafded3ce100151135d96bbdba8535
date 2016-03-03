function route(data, reqHandle, reqMysqlDB, reqEmail, socket) {
	var queryObj = JSON.parse(data);

	console.log('At router: About to route a request for ' + queryObj.type);

	try {
		if (typeof reqHandle[queryObj.type] === 'function') {
			reqHandle[queryObj.type](queryObj, reqMysqlDB, reqEmail, socket);
		} else {
			throw {msg: 'At router: No request handler found for ' + queryObj.type}
		}
	} catch(err) {
		console.log(err.msg);

		var resMessageObj
		resMessageObj = {
			type: 'err',
			msg: err.msg
		}
		resMessageStr = JSON.stringify(resMessageObj);
		socket.write(resMessageStr);
	}
}

exports.route = route;
