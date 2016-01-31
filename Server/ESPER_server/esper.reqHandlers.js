var crypto = require('crypto');

var shasum = crypto.createHash('sha1');
var sessionKey

function checkID(queryObj, reqMysqlDB, reqEmail, socket) {
  var resMessageObj, resMessageStr;
  var reqContents = {};
  reqContents['table'] = 'member';
  reqContents['attribute'] = ['mID'];
  reqContents['GET'] = {mID: queryObj.ID};

  reqMysqlDB.selectFrom(reqContents, function(tuple) {
    if(typeof tuple[0] === 'undefined') {
      resMessageObj = {type: queryObj.type, ID: queryObj.ID, result: 'approval'};
      resMessageStr = JSON.stringify(resMessageObj);
      socket.write(resMessageStr);
    } else {
      resMessageObj = {type: queryObj.type, ID: queryObj.ID, result: 'refusal'};
      resMessageStr = JSON.stringify(resMessageObj);
      socket.write(resMessageStr);
    }
  });
}

function checkEmail(queryObj, reqMysqlDB, reqEmail, socket) {
  var resMessageObj, resMessageStr;
  var reqContents = {};
  reqContents['table'] = 'member';
  reqContents['attribute'] = ['mEmail'];
  reqContents['GET'] = {mEmail: queryObj.email};

  reqMysqlDB.selectFrom(reqContents, function(tuple) {
    if(typeof tuple[0] === 'undefined') {
      resMessageObj = {type: queryObj.type, email: queryObj.email, result: 'approval'};
      resMessageStr = JSON.stringify(resMessageObj);
      socket.write(resMessageStr);
    } else {
      resMessageObj = {type: queryObj.type, email: queryObj.email, result: 'refusal'};
      resMessageStr = JSON.stringify(resMessageObj);
      socket.write(resMessageStr);
    }
  });
}

function authEmail(queryObj, reqMysqlDB, reqEmail, socket) {
  var resMessageObj, resLoginStr;
  var mailInfo = {};
  mailInfo['service'] = 'Gmail';
  mailInfo['sender'] = 'esper.dev@gmail.com';
  mailInfo['receiver'] = queryObj.email;
  mailInfo['passwd'] = 'bang1230';
  mailInfo['senderInfo'] = '<방경용 esper.dev@gmail.com>';
  mailInfo['subject'] = 'Email Authorization'
  mailInfo['contents'] = '';
  var randomNumSize = 6;

  genRandomNum(randomNumSize, function(randomNum) {
    mailInfo.contents = randomNum;

    var reqContents = {};
    reqContents['table'] = 'authEmail';
    reqContents['POST'] = {email: queryObj.email, authCode: randomNum};

    reqMysqlDB.insertInto(reqContents, function(DBRes) {
      reqEmail.sendEmail(mailInfo, function() {
        resMessageObj = {type: queryObj.type, email: queryObj.email,
          authCode: mailInfo.contents};
        resMessageStr = JSON.stringify(resMessageObj);
        socket.write(resMessageStr);

        var milliSec = 20000;
        setTimeout(function() {
          var reqContentsTemp = {};
          reqContentsTemp['table'] = 'authEmail';
          reqContentsTemp['POST'] = {email: queryObj.email};

          reqMysqlDB.deleteFrom(reqContentsTemp, function(DBRes) {
            console.log(DBRes);
          });
        }, milliSec);
      });
    });
  });
}

function genRandomNum(randomNumSize, callback) {
  var randomNum = '';

  for(var i = 0; i<randomNumSize; i++) {
    randomNum += Math.floor(Math.random() * 10);
  }
  callback(randomNum);
}

function sleep(milliSec, callback) {
  var startTime = new Date().getTime();
  while(new Date().getTime() < (startTime + milliSec)) {

  }
  callback();
}

function checkAuthCode(queryObj, reqMysqlDB, reqEmail, socket) {
  var resMessageObj, resLoginStr;
  var reqContents = {};
  reqContents['table'] = 'authEmail';
  reqContents['attribute'] = ['authCode'];
  reqContents['GET'] = {email: queryObj.email};

  reqMysqlDB.selectFrom(reqContents, function(tuple) {
    if((typeof tuple[0] === 'undefined') || (tuple[0].authCode != queryObj.authCode)) {
      resMessageObj = {type: queryObj.type, email: queryObj.email, result: 'refusal'}
      resMessageStr = JSON.stringify(resMessageObj);
      socket.write(resMessageStr);
    } else {
      resMessageObj = {type: queryObj.type, email: queryObj.email, result: 'approval'}
      resMessageStr = JSON.stringify(resMessageObj);
      socket.write(resMessageStr);
    }
  });
}

function signUp(queryObj, reqMysqlDB, reqEmail, socket) {
  var resMessageObj, resMessageStr
  var reqContents = {};
  reqContents['table'] = 'member';
  reqContents['POST'] = {mID: queryObj.ID, mPasswd: queryObj.passwd, mName: queryObj.name,
    mEmail: queryObj.email, mDate: queryObj.date};

  reqMysqlDB.insertInto(reqContents, function(DBRes) {
    resMessageObj = {type: 'signUp', ID: queryObj.ID, 'result': DBRes};
    resMessageStr = JSON.stringify(resMessageObj);
    socket.write(resMessageStr);
  });
}

function signIn(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr;
    var reqContents = {};
    var sessionKey;
    reqContents['table'] = 'member';
    reqContents['attribute'] = ['mPasswd'];
    reqContents['GET'] = {mID: queryObj.ID};

    reqMysqlDB.selectFrom(reqContents, function(tuple) {
      if(queryObj.passwd === tuple[0].mPasswd) {
        shasum.update(JSON.stringify(queryObj));
        sessionKey = shasum.digest('hex');

        var reqContentsTemp = {};
        reqContentsTemp['table'] = 'session';
        reqContentsTemp['mID'] = queryObj.ID;
        reqContentsTemp['POST'] = {mID: queryObj.ID, sessionKey: sessionKey};

        reqMysqlDB.insertInto(reqContentsTemp, function() {
          resMessageObj = {type: 'signIn', ID: queryObj.ID,
          sessionKey: sessionKey, result: 'approval'};
          resMessageStr = JSON.stringify(resMessageObj);
          socket.write(resMessageStr);
        });
      } else {
        resMessageObj = {type: 'signIn', ID: queryObj.ID, sessionKey: '',
        result: 'refusal'};
        resMessageStr = JSON.stringify(resMessageObj);
        socket.write(resMessageStr);
      }
    });
}

function signOut(queryObj, reqMysqlDB, reqEmail, socket) {
  var resMessageObj, resMessageStr;
  var reqContents = {};
  reqContents['table'] = 'session';
  reqContents['attribute'] = ['sessionKey'];
  reqContents['GET'] = {mID: queryObj.ID};

  reqMysqlDB.selectFrom(reqContents, function(tuple) {
    if(queryObj.sessionKey === tuple[0].sessionKey) {
      var reqContentsTemp = {};
      reqContentsTemp['table'] = 'session';
      reqContentsTemp['POST'] = {mID: queryObj.ID};

      reqMysqlDB.deleteFrom(reqContentsTemp, function(DBRes) {
        resMessageObj = {type: queryObj.type, ID: queryObj.ID, result: DBRes};
        resMessageStr = JSON.stringify(resMessageObj);
        socket.write(resMessageStr);
      });
    } else {
      resMessageObj = {type: queryObj.type, ID: queryObj.ID, result: 'refusal'};
      resMessageStr = JSON.stringify(resMessageObj);
      socket.write(resMessageStr);
    }
  });
}

// File enroll in database
function fileOwnerEnroll(queryObj, reqMysqlDB, reqEmail, socket) {
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
function getMyFileList(queryObj, reqMysqlDB, reqEmail, socket) {
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

function askEraseFile(queryObj, reqMysqlDB, reqEmail, socket) {
    // There is implemented by using session key.
    // need more discussion.
}

//Manage membership information
exports.checkID = checkID;
exports.checkEmail = checkEmail;
exports.authEmail = authEmail;
exports.checkAuthCode = checkAuthCode;
exports.signIn = signIn;
exports.signOut = signOut;
exports.signUp = signUp;

//
exports.fileOwnerEnroll = fileOwnerEnroll;
exports.getMyFileList = getMyFileList;
