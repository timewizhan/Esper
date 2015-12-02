var Parse = require('parse/node').Parse;

var APP_ID = 'Q7wwjwBEHXoM7RPncoL13cZUExtY8h9Mxa5XGo2J';
var MASTER_KEY = 'V9bpNszsEP86FQxQhRlovCZMibZNOeIIjM2c8wxp';
var JAVASCRIPT_KEY = 'wgUa4ShzKlPhLtQZAIxI0mQGjFsZRDO5idB3tLgB';
	
Parse.initialize(APP_ID, JAVASCRIPT_KEY);

var user = new Parse.User();
var query = new Parse.Query(Parse.User);

function login(data, socket) {
	/*console.log('login');
	console.log(data);

	query.find({
		success: function(users) {
			for(var i= 0; i<users.length; ++i) {
				if(users[i].get('username') == data.username) {
					break;
				} else {
					console.log('Do not exist id');
				}
			}
		},
		
		error : function(error) {
		
		}
	});*/
}

function logout(data, socket) {
	console.log('logout');
}

function signUp(data, socket) {
	user.set("username", data.username);
	user.set("password", data.password);
	user.set("email", data.email);
	
	user.signUp(null, {
		success: function(user) {
			// 이 부분에서 클라이언에 등록이 완료되었다는 메시지를 보내야함
			console.log(data.username + " is enrolled");
		},
		error: function(user, error) {
			// 이 부분에서 클라이언트에게 등록이 실패했다는 메시지를 보내야함
			console.log(error);
		}
	});
}

function handshake(data, socket) {
	
}

exports.login = login;
exports.logout = logout;
exports.signUp = signUp;
