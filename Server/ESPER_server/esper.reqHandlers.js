var crypto = require('crypto');

var shasum = crypto.createHash('sha1');
var sessionKey

function login (data, queryObj, dataHandle, reqMysqlDB, socket) {
    console.log('reqID: ' + queryObj.ID + ' reqPasswd: ' + queryObj.passwd);
    var mPasswd = reqMysqlDB['getPasswd'](queryObj.ID, function (mPasswd) {
        console.log('queryObj.passwd: ' + queryObj.passwd + ' mPasswd: ' + mPasswd);
        if (queryObj.passwd === mPasswd) {
            shasum.update(data);
            sessionKey = shasum.digest('hex');
            socket.write(sessionKey);
        } else {
            socket.write('login is denied');
        }   
    }); 
    //console.log('after return ' + mPasswd);
}

function logout (query, socket) {

}

// File enroll in database
function fileOwnerEnroll(data, queryObj, dataHandle, reqMysqlDB, socket) {
    var result = reqMysqlDB['fileEnroll'](queryObj, function(queryObj, connection) {
        connection.query('insert into connect (mID, filehash) value(?, ?)', [queryObj.Sender, queryObj.File.fileHash], function(err, rows, cols) {
            if(err) {
                console.log('ConnectDb insert Error : ', err);
                return 1;
            } else {
                console.log('ConnectDb insert success');
                return 0;
            }   
        }); 
    }); 
    
    if(result) {
        socket.write('fail');    
    } else {
        socket.write('success');
    }   
}

// Bring the information that who own my file.
function getMyFileList(data, queryObj, dataHandle, reqMysqlDB, socket) {   
    var result = reqMysqlDB['getMyFileList'](queryObj, function(rows, connection){
        console.log('111');
        for(row in rows) {
            console.log(rows[row].filehash);
            connection.query('select mID from connect where filehash = ?', rows[row].filehash, function(err, connectRows, connectCols) {
                if(err) {
                    console.log("Get file Error : ", err);
                } else {
                    for(idx in connectRows) {
                        console.log(connectRows[idx].mID);
                    }
                }
            });
        }
    });

    if(result) {
        socket.write('fail');
    } else {
        socket.write('success');
    }
}

function askEraseFile(data, queryObj, dataHandle, reqMysqlDB, socket) {
    // There is implemented by using session key.
    // need more discussion.
} 

exports.login = login;
exports.logout = logout;
exports.fileOwnerEnroll = fileOwnerEnroll;
exports.getMyFileList = getMyFileList;