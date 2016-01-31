var mysql = require('mysql');
var connection = mysql.createConnection({
    host: '165.132.144.98',
    user: 'root',
    password: 'bang1230',
    database: 'ESPER'
});

connection.connect(function (err) {
    if (err) {
        console.log('MySQL connection is failed.');
        console.log(err);
    } else {
        console.log('MySQL connection is success.');
    }
});

function selectFrom(reqContents, callback) {
  connection.query('SELECT ?? FROM ?? WHERE ?',
  [reqContents.attribute, reqContents.table, reqContents.GET],
  function(err, tuple, result) {
    if(err) {
      console.log(err);
    } else {
      callback(tuple);
    }
  });
}

function insertInto(reqContents, callback) {
  var DBRes = '';

  connection.query('INSERT INTO ?? SET ?', [reqContents.table, reqContents.POST],
  function(err, tuple, result) {
    if(err) {
      console.log(err);
      DBRes = 'refusal';
    } else {
      DBRes = 'approval';
    }
    callback(DBRes);
  });
}

function deleteFrom(reqContents, callback) {
  var DBRes = '';

  connection.query('DELETE FROM ?? WHERE ?', [reqContents.table, reqContents.POST],
  function(err, tuple, result) {
    if(err) {
      console.log(err);
      DBRes = 'refusal';
    } else {
      DBRes = 'approval';
    }
    callback(DBRes);
  });
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

exports.selectFrom = selectFrom;
exports.insertInto = insertInto;
exports.deleteFrom = deleteFrom;
exports.fileEnroll = fileEnroll;
exports.getMyFileList = getMyFileList;
