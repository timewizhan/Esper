function route(handle, query, socket) {
	if (query.Type != null) {
		console.log('About to route a request for ' + query.Type);
		if (typeof handle[query.Type] === 'function') {
			handle[query.Type](query, socket);
		} else {
			console.log('No request handler found for ' + query.Type);
		}
	} 
	/*else if(query.Header != null) {
		console.log('About to route a request for ' + query.Header);
		
		if(typeof handle[query.Header] === 'function') {
			handle[query.Header](query, socket);
		} else {
			console.log('No request handler found for ' + query.Header);
		}
	}*/
}

exports.route = route;