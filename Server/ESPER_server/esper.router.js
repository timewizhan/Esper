
function route(data, reqHandle, reqMysqlDB, reqEmail, socket) {
    try {
        var queryObj = JSON.parse(data);

        console.log('Connection Test with Client(String):  ' + data);
    
            try {
                if (typeof reqHandle[queryObj.type] === 'function') {
                    console.log('ok')
                    reqHandle[queryObj.type](queryObj, reqMysqlDB, reqEmail, socket);
                } else {
                    throw {
                        msg : 'At router: No request handler found for '
                                + queryObj.type
                    }
                }
            } catch (err) {
                console.log(err.msg);
        
                var resMessageObj;
                resMessageObj = {
                    type : 'err',
                    msg : err.msg
                }
                resMessageStr = JSON.stringify(resMessageObj);
                socket.write(resMessageStr);
            }
    } catch(err) {
       var resMessageObj;
       resMessageObj = {
            type : 'err',
            msg : err.msg
       }
       resMessageStr = JSON.stringify(resMessageObj);
       socket.write(resMessageStr);
    }
}


exports.route = route;

