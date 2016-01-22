function route (data, dataHandle, reqHandle, reqMysqlDB, socket) {
    var  queryObj = dataHandle['str2obj'](data);    
    
    console.log('About to route a request for' + queryObj.type);

    if (typeof reqHandle[queryObj.type] === 'function') {
        reqHandle[queryObj.type](data, queryObj, dataHandle, reqMysqlDB, socket);
    } else {
        console.log('No request handler found for ' + queryObj.type);
    }   
}

exports.route = route;
