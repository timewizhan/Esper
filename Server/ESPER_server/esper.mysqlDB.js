var mysql = require('mysql');

var connection = mysql.createConnection({
	
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
	connection.query('select ?? from ?? where ?', [ reqContents.attribute,
			reqContents.table, reqContents.GET ], function(err, tuple, result) {
		if (err) {
			console.log(err);
		} else {
			callback(tuple);
		}
	});
}

function insertInto(reqContents, callback) {
	var DBRes = '';

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
}

function deleteFrom(reqContents, callback) {
	var DBRes = '';

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
}

function updateTo(reqContents, callback) {
	connection.query('UPDATE ?? SET ? WHERE ?', [ reqContents.table,
			reqContents.SET, reqContents.where ], function(err, tuple, result) {
		if (err) {
			console.log(err);
		} else {
			callback(tuple);
		}
	});
}

function twoConditionQuery(reqContents, callback) {
	var sessionResult;

	connection.query('select ?? from ?? where ?? = ? and ?? = ?', [
			reqContents.select, reqContents.from, reqContents.where1,
			reqContents.cond1, reqContents.where2, reqContents.cond2 ],
			function(err, tuple, result) {
				if (err) {
					console.log(err);
				} else {
					callback(tuple);
				}
			});
}

function getTableRowCount(reqContents, callback) {
	connection.query('select ?? from ?? where ?? = (select max(??) from ??)', [
			reqContents.attribute, reqContents.table, reqContents.attribute,
			reqContents.attribute, reqContents.table ], function(err, tuple,
			result) {
		if (err) {
			console.log(err);
		} else {
			callback(tuple);
		}
	});
}

exports.selectFrom = selectFrom;
exports.insertInto = insertInto;
exports.deleteFrom = deleteFrom;
exports.updateTo = updateTo;
exports.twoConditionQuery = twoConditionQuery;
exports.getTableRowCount = getTableRowCount;
