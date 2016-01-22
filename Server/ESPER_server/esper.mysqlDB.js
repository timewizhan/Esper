var mysql = require('mysql');
var connection = mysql.createConnection({
    host: '',
    user: '',
    password: '',
    database: ''
});

connection.connect(function (err) {
    if (err) {
        console.log('MySQL connection is failed.');
        console.log(err);
    } else {
        console.log('MySQL connection is success.');
    }   
});

function getPasswd (ID, callback) {
    var passwd = ''; 
    var GET = {'mID':ID};
    console.log('DB ' + ID);
    connection.query('SELECT mPasswd from member where ?', GET, function(err, rows, fields) {
        if (err) {
            console.log(err);
        } else {
            for (idx in rows) {
                passwd += rows[idx].mPasswd;
            }   
        }   

        console.log('before return ' + passwd);
        //return passwd;
        callback(passwd);
    }); 
    connection.end();
}

function fileEnroll(queryObj, callback) {
    // return 0 == successi
    //    console.log(fileDbInsertQuery);
    connection.query('insert into fileDB (filehash, filename, filesize, fileowner) values(?,?,?,?)', [queryObj.File.fileHash, queryObj.File.fileName, queryObj.File.fileSize, queryObj.File.fileOwner], function(err, rows, cols) {
        if(err) {
            console.log('FileDb insert Error : ', err);
            return 1;
        } else {
            console.log('Filedb insert success');
        }   
        var result = callback(queryObj, connection);
        if(result) {
            return 1;
        } else {
            return 0;
        }
    });
}

function getMyFileList(queryObj, callback) {
    connection.query('select filehash from fileDB where fileowner = ?', queryObj.Sender , function(err, rows, colum) {
        if(err) {
            console.log('Get file list Error : ', err);
            return 1;
        } else {
            /*for(element in rows) {
                console.log(rows[element].filehash);
            }*/
            // return row;
        }

        callback(rows, connection);
    });
}

exports.getPasswd = getPasswd;
exports.fileEnroll = fileEnroll;
exports.getMyFileList = getMyFileList;