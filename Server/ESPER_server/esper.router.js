function route(data, reqHandle, reqMysqlDB, reqEmail, socket) {
	var queryObj = JSON.parse(data);

	console.log('At router: About to route a request for ' + queryObj.type);
	if (typeof reqHandle[queryObj.type] === 'function') {
		reqHandle[queryObj.type](queryObj, reqMysqlDB, reqEmail, socket);
	} else {
		console.log('At router: No request handler found for ' + queryObj.type);
	}
}

exports.route = route;
