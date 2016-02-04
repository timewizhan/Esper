var crypto = require('crypto');

var shasum = crypto.createHash('sha1');
var sessionKey

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
		mID : queryObj.AccessorId
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
	mailInfo['contents'] = '';
	var randomNumSize = 6;

	genRandomNum(randomNumSize, function(randomNum) {
		mailInfo.contents = randomNum;

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
					authCode : mailInfo.contents
				};
				resMessageStr = JSON.stringify(resMessageObj);
				socket.write(resMessageStr);

				var milliSec = 20000;
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
			'result' : DBRes
		};
		resMessageStr = JSON.stringify(resMessageObj);
		socket.write(resMessageStr);
	});
}

function signIn(queryObj, reqMysqlDB, reqEmail, socket) {
	var resMessageObj, resMessageStr;
	var reqContents = {};
	var sessionKey;
	reqContents['table'] = 'member';
	reqContents['attribute'] = [ 'mPasswd' ];
	reqContents['GET'] = {
		mID : queryObj.ID
	};

	reqMysqlDB.selectFrom(reqContents, function(tuple) {
		if (queryObj.passwd === tuple[0].mPasswd) {
			shasum.update(JSON.stringify(queryObj));
			sessionKey = shasum.digest('hex');

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
				socket.write(resMessageStr);
			});
		} else {
			resMessageObj = {
				type : 'signIn',
				ID : queryObj.ID,
				sessionKey : '',
				result : 'refusal'
			};
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
				type : queryObj.type,
				ID : queryObj.ID,
				result : 'refusal'
			};
			resMessageStr = JSON.stringify(resMessageObj);
			socket.write(resMessageStr);
		}
	});
}

/*
 * when file is deleted, copy to some other table!!!!!!!!
 */

/* For validate session check */
function sessionChecker(queryObj, reqMysqlDB, callback) {
	var reqContents = {};

	reqContents['select'] = 'mID';
	reqContents['from'] = 'session';
	reqContents['where1'] = 'mID';
	reqContents['where2'] = 'sessionKey';
	reqContents['cond1'] = queryObj.UserId;
	reqContents['cond2'] = queryObj.SessionKey;

	reqMysqlDB.twoConditionQuery(reqContents, function(rows) {
		var sessionResult;

		if (rows[0] === undefined) {
			sessionResult = 1;

			console.log('session fail');
			callback(sessionResult);
		} else {
			if (rows[0].mID === queryObj.UserId) {
				sessionResult = 0;

				console.log('session succ');
				callback(sessionResult);
			}
		}
	});
}

function accessorCheck(queryObj, reqMysqlDB, reqEmail, socket) {
	sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
		var resMessageObj, resMessageStr;

		/* Session check */
		if (sessionResult === 0) {
			var reqContents = {};

			reqContents['attribute'] = [ 'mID', 'fileId' ];
			reqContents['table'] = 'connect';
			reqContents['GET'] = {
				mID : queryObj.AccessorId
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
						if (tuple[0].mID === queryObj.AccessorId) {
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
			console.log(resMessageStr);
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
				if (tuple[0] === undefined) {
					resMessageObj = {
						type : 'wrapping1',
						result : 'fail'
					};

					resMessageStr = JSON.stringify(resMessageObj);
					socket.write(resMessageStr);
					console.log(resMessageStr);
				} else {
					/* Sending next fileId */
					nextCount = tuple[0].fileId + 1;

					resMessageObj = {
						type : 'wrapping1',
						FileId : nextCount,
						result : 'succ'
					};

					resMessageStr = JSON.stringify(resMessageObj);
					socket.write(resMessageStr);
					console.log(resMessageStr);
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
			console.log(resMessageStr);
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
					fileId : queryObj.FileId,
					mID : queryObj.UserId
				};

				/* insert file to fileDB */
				reqMysqlDB.insertInto(reqContents, function(insertRes) {
					var innerReqContents = {};

					innerReqContents['table'] = 'connect';
					innerReqContents['POST'] = {
						mID : queryObj.AccessorId,
						fileId : queryObj.FileId
					};

					if (insertRes === 'approval') {
						/* insert file to connect */
						reqMysqlDB.insertInto(innerReqContents, function(res) {
							if (res == 'approval') {
								resMessageObj = {
									'type' : 'insertRes',
									'Result' : 'succ'
								};

								resMessageStr = JSON.stringify(resMessageObj);
								socket.write(resMessageStr);
								console.log(resMessageStr);
							} else {
								resMessageObj = {
									'type' : 'insertRes',
									'Result' : 'fail'
								};

								resMessageStr = JSON.stringify(resMessageObj);
								socket.write(resMessageStr);
								console.log(resMessageStr);
							}
						});
					} else {
						console.log('first insert fail');
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

function auth(queryObj, reqMysqlDB, reqEmail, socket) {
	sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
		var resMessageObj, resMessageStr;

		/* validate sessionKey */
		if (sessionResult === 0) {
			var reqContents = {};

			reqContents['attribute'] = [ 'mID', 'del' ];
			reqContents['table'] = 'connect';
			reqContents['where1'] = 'mID';
			reqContents['where2'] = 'fileId';
			reqContents['UserId'] = queryObj.UserId;
			reqContents['fileId'] = queryObj.FileId;

			reqMysqlDB(reqContents, function(tuple) {
				if (tuple[0] === undefined) {
					resMessageObj = {
						type : 'auth',
						result : 'fail'
					};

					resMessageStr = JSON.stringify(resMessageObj);
					socket.write(resMessageStr);
					console.log(resMessageStr);
				} else {
					if (tuple[0].del === 0) {
						resMessageObj = {
							type : 'auth',
							result : 'succ'
						};

						resMessageStr = JSON.stringify(resMessageObj);
						socket.write(resMessageStr);
						console.log(resMessageStr);
					} else {
						resMessageObj = {
							type : 'auth',
							result : 'del'
						};

						resMessageStr = JSON.stringify(resMessageObj);
						socket.write(resMessageStr);
						console.log(resMessageStr);
					}
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
			console.log(resMessageStr);
		}
	});
}

function remoteDel(queryObj, reqMysqlDB, reqEmail, socket) {
	sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
		var resMessageObj, resMessageStr;

		/* validate sessionKey */
		if (sessionResult === 0) {
			var reqContents = {};

			reqContents['table'] = 'connect';
			reqContents['SET'] = {
				del : 1
			};
			reqContents['where1'] = 'mID';
			reqContents['where2'] = 'fileId';
			reqContents['AccessorId'] = queryObj.AccessorId;
			reqContents['fileId'] = queryObj.FileId;

			reqMysqlDB.delUpdateTo(reqContents, function(tuple) {

			});

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

function authUpdate(queryObj, reqMysqlDB, reqEmail, socket) {
	sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
		var resMessageObj, resMessageStr;

		/* validate sessionKey */
		if (sessionResult === 0) {
			var reqContents = {};

			/*
			 * if del attribute is 0 -> no erase if del attribute is 1 -> will
			 * be erased
			 */
			reqContents['table'] = 'connect';
			reqContents['SET'] = {
				del : queryObj.AccessorId.del
			};
			reqContents['where'] = {
				mID : queryObj.AccessorId.mID
			};

			reqMysqlDB.updateTo(reqContents, function(res) {
				console.log(res);
				if (res === 'succ') {
					resMessageObj = {
						type : 'authUpdate',
						result : 'succ'
					};

					resMessageStr = JSON.stringify(resMessageObj);
					socket.write(resMessageStr);
					console.log(resMessageStr);

				} else {
					resMessageObj = {
						type : 'authUpdate',
						result : 'fail'
					};

					resMessageStr = JSON.stringify(resMessageObj);
					socket.write(resMessageStr);
					console.log(resMessageStr);
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
			console.log(resMessageStr);
		}
	});
}

function fileListReq(queryObj, reqMysqlDB, reqEmail, socket) {
	sessionChecker(queryObj, reqMysqlDB, function(sessionResult) {
		var resMessageObj, resMessageStr;

		/* validate sessionKey */
		if (sessionResult === 0) {
			var reqContents = {};

			reqContents['attribute'] = 'fileId';
			reqContents['table'] = 'fileDB';
			reqContents['GET'] = {
				mID : queryObj.UserId
			};

			reqMysqlDB.selectFrom(reqContents, function(tuple) {
				console.log(tuple);
				console.log(tuple[0].fileId);
				console.log(tuple.length);

				var ownFileList = [];

				for ( var list in tuple.length) {
					ownFileList[list] = tuple[list];
				}

				var innerReqContents = {};

				/* require discussion */
				innerReqContents['attribute'] = 'mID';
				innerReqContents['table'] = 'connect';
				
				for ( var count in tuple.length) {
					innerReqContents['GET'] = ownFileList[count];
					
				}
				selectFrom()
			});

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
