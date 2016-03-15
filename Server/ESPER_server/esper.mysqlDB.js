var mysql = require('mysql');

var SUCC = 1;
var FAIL = 0;

var connection = mysql.createConnection({
	host : '165.132.144.98',
	user : 'root',
	password : 'bang1230',
	database : 'ESPER'
});

connection.connect(function(err) {
	if (err) {
		console.log('MySQL connection is failed.');
		console.log(err);
	} else {
		console.log('MySQL connection is success.');
	}
});

function selectFrom(reqContents, callback) {
	try {
		connection.query('select ?? from ?? where ?', [ reqContents.attribute,
				reqContents.table, reqContents.GET ], function(err, tuple,
				result) {
			if (err) {
				console.log(err);
			} else {
				callback(tuple);
			}
		});
	} catch (err) {
		console.log(err.msg);
	}
}

function selectFromFileDB(reqContents, callback) {
	try {
		connection.query('select * from ?? where ?', [ reqContents.table,
				reqContents.GET ], function(err, tuple, result) {
			if (err) {
				console.log(err);
			} else {
				callback(tuple);
			}
		});
	} catch (err) {
		console.log(err);
	}
}

function insertInto(reqContents, callback) {
	var DBRes = '';

	try {
		connection.query('INSERT INTO ?? SET ?', [ reqContents.table,
				reqContents.POST ], function(err, tuple, result) {
			if (err) {
				console.log(err);
				DBRes = 'refusal';
			} else {
				DBRes = 'approval';
			}
			callback(DBRes);
		});
	} catch (err) {
		console.log(err.msg);
	}
}

function deleteFrom(reqContents, callback) {
	var DBRes = '';

	try {
		connection.query('DELETE FROM ?? WHERE ?', [ reqContents.table,
				reqContents.POST ], function(err, tuple, result) {
			if (err) {
				console.log(err);
				DBRes = 'refusal';
			} else {
				DBRes = 'approval';
			}
			callback(DBRes);
		});
	} catch (err) {
		console.log(err.msg);
	}
}

function updateTo(reqContents, callback) {
	try {
		connection.query('UPDATE ?? SET ? WHERE ?', [ reqContents.table,
				reqContents.SET, reqContents.where ], function(err, tuple,
				result) {
			if (err) {
				console.log(err);
				callback('fail');
			} else {
				// console.log('111');
				callback('succ');
			}
		});
	} catch (err) {
		console.log(err.msg);
	}
}

function delUpdateTo(reqContents, callback) {
	try {
		connection.query('UPDATE ?? SET ? WHERE ?? = ? and ?? = ?', [
				reqContents.table, reqContents.SET, reqContents.where1,
				reqContents.cond1, reqContents.where2, reqContents.cond2 ],
				function(err, tuple, result) {
					if (err) {
						console.log(err);
					} else {
						callback('succ');
					}
				});
	} catch (err) {
		console.log(err.msg);
	}
}

function twoConditionQuery(reqContents, callback) {
	var sessionResult;

	try {
		connection.query('select ?? from ?? where ?? = ? and ?? = ?', [
				reqContents.select, reqContents.from, reqContents.where1,
				reqContents.cond1, reqContents.where2, reqContents.cond2 ],
				function(err, tuple, result) {
					if (err) {
						console.log(err);
					} else {
						callback(SUCC, tuple);
					}
				});
	} catch (err) {
		console.log(err.msg);
		callback(FAIL, err);
	}
}

function getTableRowCount(reqContents, callback) {
	try {
		connection.query(
				'select ?? from ?? where ?? = (select max(??) from ??)', [
						reqContents.attribute, reqContents.table,
						reqContents.attribute, reqContents.attribute,
						reqContents.table ], function(err, tuple, result) {
					if (err) {
						console.log(err);
					} else {
						callback(tuple);
					}
				});
	} catch (err) {
		console.log(err.msg);
	}
}

function equalJoin(callback) {
	try {
		connection
				.query(
						'SELECT * FROM fileDB LEFT OUTER JOIN connect ON fileDB.fileId = connect.fileId',
						function(err, tuple, result) {

							if (err) {
								console.log(err);
							} else {
								callback(tuple);
							}
						});
	} catch (err) {
		console.log(err.msg);
	}
}

exports.selectFrom = selectFrom;
exports.selectFromFileDB = selectFromFileDB;
exports.insertInto = insertInto;
exports.deleteFrom = deleteFrom;
exports.updateTo = updateTo;
exports.delUpdateTo = delUpdateTo;
exports.twoConditionQuery = twoConditionQuery;
exports.getTableRowCount = getTableRowCount;
exports.equalJoin = equalJoin;
