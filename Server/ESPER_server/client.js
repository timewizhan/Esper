var net = require('net');

var socket = net.connect(8000, '192.168.0.19', function() {
  console.log('Client Start');
});

socket.on('data', function(data) {
  console.log(data.toString());
});

process.stdin.resume();
process.stdin.on('data', function(data) {
  var queryObj = {};
  var queryStr
  var dataLen = data.toString().length;
  dataTemp = data.toString().substring(0, dataLen - 1);

  switch(dataTemp) {
    case 'checkID' :
    queryObj['Type'] = 'checkID';
    queryObj['ID'] = 'idophio';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'checkEmail' :
    queryObj['type'] = 'checkEmail';
    queryObj['email'] = 'auslese7@gmail.com';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'authEmail' :
    queryObj['type'] = 'authEmail';
    queryObj['email'] = 'idophio@gmail.com';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'checkAuthCode' :
    queryObj['type'] = 'checkAuthCode';
    queryObj['email'] = 'idophio@gmail.com';
    queryObj['authCode'] = '011244';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'signUp' :
    queryObj['type'] = 'signUp';
    queryObj['ID'] = 'idophio';
    queryObj['passwd'] = '0000';
    queryObj['name'] = 'bang';
    queryObj['email'] = 'idophio@gmail.com';
    queryObj['date'] = '2016-01-24';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'signIn' :
    queryObj['type'] = 'signIn';
    queryObj['ID'] = 'idophio';
    queryObj['passwd'] = '0000';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'signOut' :
    queryObj['type'] = 'signOut';
    queryObj['ID'] = 'idophio';
    queryObj['sessionKey'] = '2ca766dd616a17185cce04ae4a5fbbb00b772dd0';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'findID' :
    queryObj['type'] = 'findID';
    queryObj['name'] = 'bang';
    queryObj['email'] = 'idophio@gmail.com';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'findPasswd' :
    queryObj['type'] = 'findPasswd';
    queryObj['ID'] = 'idophio';
    queryObj['email'] = 'idophio@gmail.com';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'changePasswd' :
    queryObj['type'] = 'changePasswd';
    queryObj['ID'] = 'idophio';
    queryObj['changedPasswd'] = '0101';
    queryObj['sessionKey'] = '2ca766dd616a17185cce04ae4a5fbbb00b772dd0';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    case 'withdrawal' :
    queryObj['type'] = 'withdrawal';
    queryObj['ID'] = 'idophio';
    queryObj['sessionKey'] = '2ca766dd616a17185cce04ae4a5fbbb00b772dd0';

    queryStr = JSON.stringify(queryObj);
    socket.write(queryStr);
    break;

    default:
    console.log('There is no the instruction');
  }
  });
