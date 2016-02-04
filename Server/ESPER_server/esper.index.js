var server = require('./esper.server.js');
var mysqlDB = require('./esper.mysqlDB.js');
// var reqDataHandlers = require('./esper.reqDataHandlers.js');
var router = require('./esper.router.js');
var reqHandlers = require('./esper.reqHandlers.js');
var email = require('./esper.email.js');

var reqMysqlDB = {};
// DML
reqMysqlDB['selectFrom'] = mysqlDB.selectFrom;
reqMysqlDB['insertInto'] = mysqlDB.insertInto;
reqMysqlDB['deleteFrom'] = mysqlDB.deleteFrom;
reqMysqlDB['updateTo'] = mysqlDB.updateTo;
reqMysqlDB['delUpdateTo'] = mysqlDB.delUpdateTo;

reqMysqlDB['twoConditionQuery'] = mysqlDB.twoConditionQuery;
reqMysqlDB['getTableRowCount'] = mysqlDB.getTableRowCount;

/*
 * var dataHandle = {}; dataHandle['str2obj'] = reqDataHandlers.str2obj;
 * dataHandle['obj2str'] = reqDataHandlers.obj2str;
 */

var reqHandle = {};
// Manage membership information
reqHandle['checkID'] = reqHandlers.checkID;
reqHandle['checkEmail'] = reqHandlers.checkEmail;
reqHandle['authEmail'] = reqHandlers.authEmail;
reqHandle['checkAuthCode'] = reqHandlers.checkAuthCode;
reqHandle['signUp'] = reqHandlers.signUp;
reqHandle['signIn'] = reqHandlers.signIn;
reqHandle['signOut'] = reqHandlers.signOut;

/* file control */
reqHandle['accessorCheck'] = reqHandlers.accessorCheck;
reqHandle['wrappingReq'] = reqHandlers.wrappingReq;
reqHandle['wrappingRes'] = reqHandlers.wrappingRes;
reqHandle['auth'] = reqHandlers.auth;
reqHandle['remoteDel'] = reqHandlers.remoteDel;
reqHandle['authUpdate'] = reqHandlers.authUpdate;
reqHandle['fileListReq'] = reqHandlers.fileListReq;

reqHandle['test'] = reqHandlers.test;


var reqEmail = {};
reqEmail['sendEmail'] = email.sendEmail;

server.start(router.route, reqHandle, reqMysqlDB, reqEmail);
