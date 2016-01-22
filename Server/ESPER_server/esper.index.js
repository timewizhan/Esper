var server = require('./esper.server.js');
var mysqlDB = require('./esper.mysqlDB.js');
var reqDataHandlers = require('./esper.reqDataHandlers.js');
var router = require('./esper.router.js');
var reqHandlers = require('./esper.reqHandlers.js');
 
var reqMysqlDB = {};
reqMysqlDB['getPasswd'] = mysqlDB.getPasswd;
reqMysqlDB['fileEnroll'] = mysqlDB.fileEnroll;
reqMysqlDB['getMyFileList'] = mysqlDB.getMyFileList;

var dataHandle = {};
dataHandle['str2obj'] = reqDataHandlers.str2obj;
dataHandle['obj2str'] = reqDataHandlers.obj2str;
 
var reqHandle = {};
reqHandle['login'] = reqHandlers.login;
reqHandle['logout'] = reqHandlers.logout;
reqHandle['signUp'] = reqHandlers.signUp;
reqHandle['header'] = reqHandlers.header;
reqHandle['fileOwnerEnroll'] = reqHandlers.fileOwnerEnroll;
reqHandle['getMyFileList'] = reqHandlers.getMyFileList;
 
server.start(router.route, dataHandle, reqHandle, reqMysqlDB);