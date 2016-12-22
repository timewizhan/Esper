var crypto = require('crypto');
//var Promise = require('promise');

//var shasum = crypto.createHash('sha1');
//var sessionKey

var SUCC = 1;
var FAIL = 0;

function checkID(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr;
    var reqContents = {};
    reqContents['table'] = 'member';
    reqContents['attribute'] = [ 'mID' ];
    reqContents['GET'] = {
        mID : queryObj.ID
    };

    reqMysqlDB.selectFrom(reqContents, function(tuple) {
        if (typeof tuple[0] === 'undefined') {
            resMessageObj = {
                type : queryObj.type,
                ID : queryObj.ID,
                result : 'approval'
            };
            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
        } else {
            resMessageObj = {
                type : queryObj.type,
                ID : queryObj.ID,
                result : 'refusal'
            };
            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
        }
    });
}

function checkEmail(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr;
    var reqContents = {};
    reqContents['table'] = 'member';
    reqContents['attribute'] = [ 'mEmail' ];
    reqContents['GET'] = {
        mEmail : queryObj.email
    };

    reqMysqlDB.selectFrom(reqContents, function(tuple) {
        if (typeof tuple[0] === 'undefined') {
            resMessageObj = {
                type : queryObj.type,
                email : queryObj.email,
                result : 'approval'
            };
            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
        } else {
            resMessageObj = {
                type : queryObj.type,
                email : queryObj.email,
                result : 'refusal'
            };
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
    mailInfo['contents'] = 'This verification code is valid during 90seconds : ';
    var randomNumSize = 6;

    genRandomNum(randomNumSize, function(randomNum) {
        mailInfo.contents += randomNum;

        var reqContents = {};
        reqContents['table'] = 'authEmail';
        reqContents['POST'] = {
            email : queryObj.email,
            authCode : randomNum
        };

        reqMysqlDB.insertInto(reqContents, function(DBRes) {
            reqEmail.sendEmail(mailInfo, function() {
                resMessageObj = {
                    type : queryObj.type,
                    email : queryObj.email,
                // authCode : mailInfo.contents
                };
                resMessageStr = JSON.stringify(resMessageObj);
                socket.write(resMessageStr);

                var milliSec = 130000;
                setTimeout(function() {
                    var reqContentsTemp = {};
                    reqContentsTemp['table'] = 'authEmail';
                    reqContentsTemp['POST'] = {
                        email : queryObj.email
                    };

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

    for (var i = 0; i < randomNumSize; i++) {
        randomNum += Math.floor(Math.random() * 10);
    }
    callback(randomNum);
}

function sleep(milliSec, callback) {
    var startTime = new Date().getTime();
    while (new Date().getTime() < (startTime + milliSec)) {

    }
    callback();
}

function checkAuthCode(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resLoginStr;
    var reqContents = {};
    reqContents['table'] = 'authEmail';
    reqContents['attribute'] = [ 'authCode' ];
    reqContents['GET'] = {
        email : queryObj.email
    };

    reqMysqlDB.selectFrom(reqContents, function(tuple) {
        if ((typeof tuple[0] === 'undefined')
                || (tuple[0].authCode != queryObj.authCode)) {
            resMessageObj = {
                type : queryObj.type,
                email : queryObj.email,
                result : 'refusal'
            }
            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
        } else {
            resMessageObj = {
                type : queryObj.type,
                email : queryObj.email,
                result : 'approval'
            }
            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
        }
    });
}

function signUp(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr
    var reqContents = {};
    reqContents['table'] = 'member';
    reqContents['POST'] = {
        mID : queryObj.ID,
        mPasswd : queryObj.passwd,
        mName : queryObj.name,
        mEmail : queryObj.email,
        mDate : queryObj.date
    };

    reqMysqlDB.insertInto(reqContents, function(DBRes) {
        resMessageObj = {
            type : 'signUp',
            ID : queryObj.ID,
            result : DBRes
        };
        resMessageStr = JSON.stringify(resMessageObj);
        socket.write(resMessageStr);
    });
}

function signIn(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr;
    var reqContents = {};
    var shasum = crypto.createHash('sha1');
    var sessionTemp;
    var sessionKey;
    reqContents['table'] = 'member';
    reqContents['attribute'] = [ 'mPasswd' ];
    reqContents['GET'] = {
        mID : queryObj.ID
    };

    reqMysqlDB.selectFrom(reqContents, function(tuple) {
        if(tuple[0] !== undefined) {
            if (queryObj.passwd === tuple[0].mPasswd) {
                shasum.update(JSON.stringify(queryObj));
                sessionKeyTemp = shasum.digest('hex');
                sessionKey = sessionKeyTemp.substring(0, 29);    
                var reqContentsTemp = {};
                reqContentsTemp['table'] = 'session';
                reqContentsTemp['mID'] = queryObj.ID;
                reqContentsTemp['POST'] = {
                    mID : queryObj.ID,
                    sessionKey : sessionKey
                };
    
                reqMysqlDB.insertInto(reqContentsTemp, function() {
                    resMessageObj = {
                        type : 'signIn',
                        ID : queryObj.ID,
                        sessionKey : sessionKey,
                        result : 'approval'
                    };
                    resMessageStr = JSON.stringify(resMessageObj);
                    console.log('Tx: ' + resMessageStr);
                    socket.write(resMessageStr);
                });
            } else {
                resMessageObj = {
                    type : 'signIn',
                    ID : queryObj.ID,
                    sessionKey : '',
                    result : 'refusal'
                };
             resMessageStr = JSON.stringify(resMessageObj) + null;
                console.log('Rx: ' + resMessageStr);
                socket.write(resMessageStr);
            }
        }
        else {
            resMessageObj = {
                type : 'signIn',
                ID : queryObj.ID,
                sessionKey : '',
                result : 'refusal'
            }
            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
        }
    });
}

function signOut(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr;
    var reqContents = {};
    reqContents['table'] = 'session';
    reqContents['attribute'] = [ 'sessionKey' ];
    reqContents['GET'] = {
        mID : queryObj.ID
    };

    reqMysqlDB.selectFrom(reqContents, function(tuple) {
        if (queryObj.sessionKey === tuple[0].sessionKey) {
            var reqContentsTemp = {};
            reqContentsTemp['table'] = 'session';
            reqContentsTemp['POST'] = {
                mID : queryObj.ID
            };

            reqMysqlDB.deleteFrom(reqContentsTemp, function(DBRes) {
                resMessageObj = {
                    type : queryObj.type,
                    ID : queryObj.ID,
                    result : DBRes
                };
                resMessageStr = JSON.stringify(resMessageObj);
                socket.write(resMessageStr);
            });
        } else {
            resMessageObj = {
                type : 'err',
                msg : 'sessionKey error'
            }
            resMessageStr = JSON.stringify('resMessageObj');
            socket.write(resMessageStr);
        }
    });
}

function findID(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr;
    var reqContents = {};
    reqContents['table'] = 'member';
    reqContents['GET'] = {
        mEmail : queryObj.email
    };

    reqMysqlDB.selectFromFileDB(reqContents, function(tuple) {
        if (queryObj.name === tuple[0].mName
                && queryObj.email === tuple[0].mEmail) {
            var mailInfo = {};
            mailInfo['service'] = 'Gmail';
            mailInfo['sender'] = 'esper.dev@gmail.com';
            mailInfo['receiver'] = queryObj.email;
            mailInfo['passwd'] = 'bang1230';
            mailInfo['senderInfo'] = '<방경용 esper.dev@gmail.com>';
            mailInfo['subject'] = 'Find ID';
            mailInfo['contents'] = 'Your ESPER ID: ' + tuple[0].mID;

            reqEmail.sendEmail(mailInfo, function() {
                resMessageObj = {
                    type : queryObj.type,
                    name : queryObj.name,
                    email : queryObj.email,
                    result : 'approval'
                };

                resMessageStr = JSON.stringify(resMessageObj);
                socket.write(resMessageStr);
            });
        } else {
            console.log(tuple[0].mName + '  ' + tuple[0].mEmail);
            resMessageObj = {
                type : 'err',
                msg : 'There is no your member information'
            }
            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
        }

    });

}

function findPasswd(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr;
    var reqContents = {};
    reqContents['table'] = 'member';
    reqContents['GET'] = {
        mEmail : queryObj.email
    };

    reqMysqlDB.selectFromFileDB(reqContents,
            function(tuple) {
                if (queryObj.ID === tuple[0].mID
                        && queryObj.email === tuple[0].mEmail) {
                    var mailInfo = {};
                    mailInfo['service'] = 'Gmail';
                    mailInfo['sender'] = 'esper.dev@gmail.com';
                    mailInfo['receiver'] = queryObj.email;
                    mailInfo['passwd'] = 'bang1230';
                    mailInfo['senderInfo'] = '<방경용 esper.dev@gmail.com>';
                    mailInfo['subject'] = 'Find Passwd';
                    mailInfo['contents'] = 'Your ESPER Passwd: ';

                    var randomNumSize = 4;
                    genRandomNum(randomNumSize, function(randomNum) {
                        mailInfo.contents += randomNum;

                        var reqContentsTemp = {};
                        reqContentsTemp['table'] = 'member';
                        reqContentsTemp['SET'] = {
                            mPasswd : randomNum
                        };
                        reqContentsTemp['where'] = {
                            mID : queryObj.ID
                        };
                        reqMysqlDB.updateTo(reqContentsTemp, function(DBRes) {
                            reqEmail.sendEmail(mailInfo, function() {
                                resMessageObj = {
                                    type : queryObj.type,
                                    ID : queryObj.ID,
                                    result : DBRes
                                };
                                resMessageStr = JSON.stringify(resMessageObj);
                                socket.write(resMessageStr);
                                //console.log(resMessageStr);
                            });
                        });
                    });
                } else {
                    console.log(tuple[0].mName + '  ' + tuple[0].mEmail);
                    resMessageObj = {
                        type : 'err',
                        msg : 'refusal'
                    }
                    resMessageStr = JSON.stringify(resMessageObj);
                    socket.write(resMessageStr);
                }
            });

}

function changePasswd(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr;
    var reqContents = {};
    reqContents['table'] = 'session';
    reqContents['attribute'] = [ 'sessionKey' ];
    reqContents['GET'] = {
        mID : queryObj.ID
    };

    reqMysqlDB.selectFrom(reqContents, function(tuple) {
        if (queryObj.sessionKey === tuple[0].sessionKey) {
            var reqContentsTemp = {};
            reqContentsTemp['table'] = 'member';
            reqContentsTemp['SET'] = {
                mPasswd : queryObj.changedPasswd
            };
            reqContentsTemp['where'] = {
                mID : queryObj.ID
            };

            reqMysqlDB.updateTo(reqContentsTemp, function(DBRes) {
                resMessageObj = {
                    type : queryObj.type,
                    ID : queryObj.ID,
                    result : DBRes
                };
                resMessageStr = JSON.stringify(resMessageObj);
                socket.write(resMessageStr);
            });
        } else {
            resMessageObj = {
                type : 'err',
                msg : 'sessionKey error'
            }
            resMessageStr = JSON.stringify('resMessageObj');
            socket.write(resMessageStr);
        }
    });
}

function withdrawal(queryObj, reqMysqlDB, reqEmail, socket) {
    var resMessageObj, resMessageStr;
    var reqContents = {};
    reqContents['table'] = 'session';
    reqContents['attribute'] = [ 'sessionKey' ];
    reqContents['GET'] = {
        mID : queryObj.ID
    };

    reqMysqlDB.selectFrom(reqContents, function(tuple) {
        if (queryObj.sessionKey === tuple[0].sessionKey) {
            var reqContentsTemp = {};
            reqContentsTemp['table'] = 'member';
            reqContentsTemp['POST'] = {
                mID : queryObj.ID
            };
            console.log(reqContentsTemp);
            reqMysqlDB.deleteFrom(reqContentsTemp, function(DBRes) {
                resMessageObj = {
                    type : queryObj.type,
                    ID : queryObj.ID,
                    result : DBRes
                };
                resMessageStr = JSON.stringify(resMessageObj);
                socket.write(resMessageStr);
            });
        } else {
            resMessageObj = {
                type : 'err',
                msg : 'sessionKey error'
            }
            resMessageStr = JSON.stringify('resMessageObj');
            socket.write(resMessageStr);
        }
    });
}

/*
 * when file is deleted, copy to some other table!!!!!!!!
 */

/* For validate session check */
function sessionChecker(queryObj, reqMysqlDB, callback) {

    if (queryObj.SessionKey === null) {
        resMessageObj = {
            type : 'invalidSessionKey',
            result : 'fail'
        };

        resMessageStr = JSON.stringify(resMessageObj);
        socket.write(resMessageStr);
    } else {
      //  console.log('222');
        var reqContents = {};

        reqContents['select'] = 'mID';
        reqContents['from'] = 'session';
        reqContents['where1'] = 'mID';
        reqContents['where2'] = 'sessionKey';
        reqContents['cond1'] = queryObj.UserID;
        reqContents['cond2'] = queryObj.SessionKey;
        
        reqMysqlDB.twoConditionQuery(reqContents, function(state, rows) {
        //    console.log('333'+state);
            if (state === FAIL) {
                console.log(rows.msg);
            } else if (state === SUCC) {
                var sessionResult;

                /* session check error */
                if (rows[0] === undefined) {
                    //console.log('rows : ' + rows[0]);
                    sessionResult = SUCC;

                    console.log('[Info] : session fail');
                    callback(sessionResult);

                    /* session check success */
                } else {
                    console.log(rows[0].mID);
                    if (rows[0].mID === queryObj.UserID) {
                        sessionResult = FAIL;

                        console.log('[Info] : session succ');
                        callback(sessionResult);
                    }
                }
            } else {
                console.log('[Info] : Invalid state');
            }
        });
    }
}

/* Check whether real access user or not */
function accessorCheck(queryObj, reqMysqlDB, reqEmail, socket) {
   // console.log('111');
    sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
        var resMessageObj, resMessageStr;

        /* Session check */
        if (sessionResult === 0) {
            var reqContents = {};

            reqContents['attribute'] = [ 'mID' ];
            reqContents['table'] = 'member';
            reqContents['GET'] = {
                mID : queryObj.AccessorID
            };

            reqMysqlDB.selectFrom(reqContents, function(tuple) {
                try {
                    // console.log(typeof tuple[0] === undefined);
                    if (tuple[0] === undefined) {
                        resMessageObj = {
                            type : 'accessorCheck',
                            result : 'fail'
                        };

                        resMessageStr = JSON.stringify(resMessageObj);
                        socket.write(resMessageStr);
                        console.log(resMessageStr);
                    } else {
                        if (tuple[0].mID === queryObj.AccessorID) {
                            resMessageObj = {
                                type : 'accessorCheck',
                                result : 'succ'
                            };

                            resMessageStr = JSON.stringify(resMessageObj);
                            socket.write(resMessageStr);
                            console.log(resMessageStr);
                        }
                    }
                } catch (e) {
                    console.log('err');
                }
            });
        } else {
            resMessageObj = {
                type : 'sessionCheck',
                result : 'fail'
            };

            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
            // console.log(resMessageStr);
        }
    });
}

function wrappingReq(queryObj, reqMysqlDB, reqEmail, socket) {
    sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
        var resMessageObj, resMessageStr;

        /* validate sessionKey */
        if (sessionResult === 0) {
            var nextCount;
            var reqContents = {};

            reqContents['table'] = 'fileDB';
            reqContents['attribute'] = 'fileId';

            reqMysqlDB.getTableRowCount(reqContents, function(tuple) {
                if (tuple === undefined) {
                    resMessageObj = {
                        type : 'wrapping1',
                        FileId : -1,
                        result : 'fail'
                    };

                    resMessageStr = JSON.stringify(resMessageObj);
                    socket.write(resMessageStr);
                    console.log(resMessageStr);
                } else {
                    /* Sending next fileId */
                    nextCount = tuple[0].AUTO_INCREMENT + 1;
                    console.log("[nextCount] : ", nextCount);
                    resMessageObj = {
                        type : 'wrapping1',
                        FileId : nextCount,
                        result : 'succ'
                    };

                    resMessageStr = JSON.stringify(resMessageObj);
                    socket.write(resMessageStr);
                    //console.log("resInfo : ", resMessageStr);
                }
            });
            /* Invalidate sessionKey */
        } else {
            resMessageObj = {
                type : 'sessionCheck',
                result : 'fail'
            };

            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
            //console.log(resMessageStr);
        }
    });
}

function wrappingRes(queryObj, reqMysqlDB, reqEmail, socket) {
    sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
        var resMessageObj, resMessageStr;

        /* validate sessionKey */
        if (sessionResult === 0) {
            if (queryObj.WrappingResult === 'succ') {
                var reqContents = {};
                   
                reqContents['table'] = 'fileDB';
                reqContents['POST'] = {
                    fileId : queryObj.FileID,
                    mID : queryObj.UserID,
                    fileName : queryObj.FileName
                };

                /* insert file to fileDB */
                reqMysqlDB.insertInto(reqContents, function(insertRes) {
                    var innerReqContents = {};

                    if (insertRes === 'approval') {
                        /* insert file to connect */
                        for(var i=0; i< queryObj.AccessorID.length; i++) { 
                            console.log('[Info] : '+queryObj.AccessorID[i]);
                            innerReqContents['table'] = 'connect';
                            innerReqContents['POST'] = {
                                mID:queryObj.AccessorID[i],
                                fileId : queryObj.FileID
                            }
                            reqMysqlDB.insertInto(innerReqContents, function(res) {
                                if (res == 'approval') {
                                    resMessageObj = {
                                        'type' : 'insertRes',
                                        'result' : 'succ'
                                    };
    
                                    resMessageStr = JSON.stringify(resMessageObj);
                                    socket.write(resMessageStr);
                                    console.log(resMessageStr);
                                }
                                else if(res == 'error') {
                                    resMessageObj = {
                                        'type' : 'insertRes',
                                        'result' : 'fail'
                                    }

                                    resMessageStr = JSON.stringify(resMessageObj);

                                    socket.write(resMessageStr);
                                    console.log(resMessageStr);
                                } else {
                                    resMessageObj = {
                                        'type' : 'insertRes',
                                        'result' : 'fail'
                                    };

                                    resMessageStr = JSON.stringify(resMessageObj);
    
                                    socket.write(resMessageStr);
                                    console.log(resMessageStr);
                                }
                            });
                        }
                    } else {
                        console.log('first insert fail');
                        resMessageObj = {
                            'type' : 'insertRes',
                            'Result' : 'fail'
                        }

                        resMessageStr = JSON.stringify(resMessageObj);

                        socket.write(resMessageStr);
                        console.log(resMessageStr);
                    }
                });

            } else if (queryObj.WrappingResult === 'fail') {
                console.log('wrapping fail in client');
            } else {
                /* when come in strange value */
                console.log('Invalidate values');
            }
            /* Invalidate sessionKey */
        } else {
            resMessageObj = {
                type : 'sessionCheck',
                result : 'fail'
            };

            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
            console.log(resMessageStr);
        }

    });
}

/* wrapped file open */
function auth(queryObj, reqMysqlDB, reqEmail, socket) {
    sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
        var resMessageObj, resMessageStr;

        /* validate sessionKey */
        if (sessionResult === 0) {
            var reqContents = {};

            reqContents['select'] = [ 'mID', 'del' ];
            reqContents['from'] = 'connect';
            reqContents['where1'] = 'mID';
            reqContents['where2'] = 'fileId';
            reqContents['cond1'] = queryObj.UserID;
            reqContents['cond2'] = queryObj.FileID;

            reqMysqlDB.twoConditionQuery(reqContents, function(state, tuple) {
                if (state === SUCC) {
                    if (tuple[0] === undefined) {
                        resMessageObj = {
                            type : 'auth',
                            result : 'fail'
                        };

                        resMessageStr = JSON.stringify(resMessageObj);
                        socket.write(resMessageStr);
                        // console.log(resMessageStr);

                        /* when result is succ, file is unwrapped */
                    } else {
                        if (tuple[0].del === 0) {
                            resMessageObj = {
                                type : 'auth',
                                result : 'succ'
                            };

                            resMessageStr = JSON.stringify(resMessageObj);
                            socket.write(resMessageStr);
                            // console.log(resMessageStr);
                        } else if (tuple[0].del === 1) {
                            resMessageObj = {
                                type : 'auth',
                                result : 'del'
                            };

                            resMessageStr = JSON.stringify(resMessageObj);
                            socket.write(resMessageStr);
                            // console.log(resMessageStr);
                        } else {
                            console.log('tuple result error');
                        }
                    }
                } else if (state === FAIL) {
                    console.log(tuple.msg);
                } else {
                    console.log('Invalid state');
                }
            });

            /* Invalidate sessionKey */
        } else {
            resMessageObj = {
                type : 'sessionCheck',
                result : 'fail'
            };

            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
            // console.log(resMessageStr);
        }
    });
}

function authUpdate(queryObj, reqMysqlDB, reqEmail, socket) {
    sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
        var resMessageObj, resMessageStr;
    
        reqContents = {};
    
        reqContents['table'] = 'connect';
        reqContents['SET'] = {
            del : 1
        }
        reqContents['where'] = {
            fileId : queryObj.FileID
        }
        reqMysqlDB.updateTo(reqContents, function(tuple) {
            reqContents['SET'] = {
                del : 0
            }
            if(tuple === 'succ') {
                    reqContents['where1'] = 'fileId';
                    reqContents['where2'] = 'mID';
                    reqContents['cond1'] = queryObj.FileID;
                
                    for (var i=0; i<queryObj.Accessor.length; i++) {
                        reqContents['cond2'] = queryObj.Accessor[i];
                        console.log(reqContents);
    
                        reqMysqlDB.delUpdateTo(reqContents, function(tuple) {
                            if(tuple === 'succ') {
                                resMessageObj = {
                                    type : 'authUpdate',
                                    FileID : queryObj.FileID,
                                    result : 'succ'
                                }

                                resMessageStr = JSON.stringify(resMessageObj);
                                socket.write(resMessageStr);
                                console.log(resMessageStr);
                            } else {
                                resMessageObj = {
                                    type : 'authUpdate',
                                    FileID : queryObj.FileID,
                                    result : 'fail'
                                }
    
                                resMessageStr = JSON.stringify(resMessageObj);
                                socket.write(resMessageStr);
                                console.log(resMessageStr);
                            }
                        });
                    }
            }
        });
    });
}

function remoteDel(queryObj, reqMysqlDB, reqEmail, socket) {
    sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
        var resMessageObj, resMessageStr;

        /* validate sessionKey */
        if (sessionResult === 0) {
            var reqContents = {};

            /*
             * if del attribute is 0 -> no erase if del attribute is 1 -> will
             * be erased
             */
            
            reqContents['table'] = 'fileDB';
            reqContents['SET'] = {
                del : queryObj.Del
            }
            reqContents['where'] = {
                fileId : queryObj.FileID
            }
            
            reqMysqlDB.updateTo(reqContents, function(tuple) {
                if(tuple == 'succ') {
                    resMessageObj = {
                        type : 'remoteDel',
                        result : 'succ'
                    }
                } else {
                    resMessageObj = {
                        type : 'remoteDel',
                        result : 'fail'
                    }
                }
            });
        }

        else {
            resMessageObj = {
                type : 'sessionCheck',
                result : 'fail'
            };

            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
            console.log(resMessageStr);
        }
    });
}

function fileListReq(queryObj, reqMysqlDB, reqEmail, socket) {
    sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
        var resMessageObj, resMessageStr;

        /* validate sessionKey */
        if (sessionResult === 0) {
            fileList = [];
            var reqContents = {};

            reqContents['UserID'] = queryObj.UserID;

            reqMysqlDB.equalJoin(reqContents, function(tuple) {
                flag = 0;

                userIdList = [];
                prevInfo = [];
                fileList = [];
                prevFileId = null;
                
                console.log("[filereq tuple check] : ", tuple[0], tuple.length);

                for ( var i=0; i< tuple.length; i++) {
                        if(flag==0 && tuple.length==1) {
                           tmp = [
                                {"fileName" : tuple[i].fileName}, {"fileId":tuple[i].fileId}, {"userId":tuple[i].mID}, {"del":tuple[i].del}
                           ]; 
                            fileList.push(tmp);                            
                        }   
                        else if(flag==0 && tuple.length>1) {
                            userIdList.push(tuple[i].mID)
                            prevInfo.push(tuple[i].fileName, tuple[i].fileId, tuple[i].del);
                            prevFileId = tuple[i].fileId;
                            flag = 1
                        } else if(flag==1) {
                            if(prevFileId == tuple[i].fileId) {
                                userIdList.push(tuple[i].mID)
                                if(i==tuple.length-1) {
                                    tmp = [{"fileName" : prevInfo[0]}, {"fileId" : prevInfo[1]},
                                        {"userId" : userIdList}, {"del" : prevInfo[2]}];
                                    fileList.push(tmp);
                                }

                            } else {
                                tmp = [{"fileName" : prevInfo[0]}, {"fileId" : prevInfo[1]},
                                        {"userId" : userIdList}, {"del" : prevInfo[2]}];
                                fileList.push(tmp);
                                
                                if(i == tuple.length-1) {
                                    tmp = [{"fileName" : tuple[i].fileName}, {"fileId" : tuple[i].fileId},
                                            {"userId" : userIdList}, {"del" : tuple[i].del}];
                                    fileList.push(tmp);
                                }

                                tmp = [];
                                prevInfo = [];
                                prevInfo.push(tuple[i].fileName, tuple[i].fileId, tuple[i].del);

                                userIdList = [];
                                userIdList.push(tuple[i].mID);

                                prevFileId = tuple[i].fileId;

                                console.log("[fileList check] : ", prevFileId, i);

                            }
                        }
                         
                            //tmp = [{"fileName" : tuple[i].fileName}, {"fileId" : tuple[i].fileId}, 
                            //        {"userId" : tuple[i].mID}, {"del" : tuple[i].del}];
                            //fileList.push(tmp);
                     //   }     
                   // }
                }

                //fileList = tuple;

                resMessageObj = {
                    type : 'fileListReq',
                    result : 'succ',
                    File : fileList
                };
                resMessageStr = JSON.stringify(resMessageObj);
                socket.write(resMessageStr);
                console.log("[fileList resMsg] : ", resMessageStr);
            })

            /* Invalidate sessionKey */
        } else {
            resMessageObj = {
                type : 'sessionCheck',
                result : 'fail'
            };

            resMessageStr = JSON.stringify(resMessageObj);
            socket.write(resMessageStr);
            console.log(resMessageStr);
        }
    });
}

function test(queryObj, reqMysqlDB, reqEmail, socket) {
    console.log(queryObj.AccessorId.mID);
}

// Manage membership information
exports.checkID = checkID;
exports.checkEmail = checkEmail;
exports.authEmail = authEmail;
exports.checkAuthCode = checkAuthCode;
exports.signIn = signIn;
exports.signOut = signOut;
exports.signUp = signUp;
exports.findID = findID;
exports.findPasswd = findPasswd;
exports.changePasswd = changePasswd;
exports.withdrawal = withdrawal;

/* file control */
exports.accessorCheck = accessorCheck;
exports.wrappingReq = wrappingReq;
exports.wrappingRes = wrappingRes;
exports.auth = auth;
exports.remoteDel = remoteDel;
exports.authUpdate = authUpdate;
exports.fileListReq = fileListReq;

/* for test */
exports.test = test;

