/**
 * http://usejsdoc.org/
 */
var server = require('./server.js');
var router = require('./router.js');
var requestHandlers = require('./requestHandlers.js');

var handle = {};

handle['login'] = requestHandlers.login;
handle['logout'] = requestHandlers.logout;
handle['signUp'] = requestHandlers.signUp;
handle['header'] = requestHandlers.header;

server.start(router.route, handle);