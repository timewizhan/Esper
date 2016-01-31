var nodemailer = require('nodemailer');

function sendEmail(mailInfo, callback) {
  console.log('access mail function');
  var transporter = nodemailer.createTransport({
    service: mailInfo.service,
    auth: {
      user: mailInfo.sender,
      pass: mailInfo.passwd
    },
    tis: {
      rejectUnauthorized: 'false'
    }
  });

  var mailOptions = {
    from: mailInfo.senderInfo,
    to: mailInfo.receiver,
    subject: mailInfo.subject,
    text: mailInfo.contents
  }

  transporter.sendMail(mailOptions, function(err, info) {
    if(err) {
      console.log(err);
    } else {
      console.log('Message sent: ' + info.response);
    }
    transporter.close();
  });
  
  callback();
}

exports.sendEmail = sendEmail;
