var express = require('express');
var app = express();
var fs = require("fs");

var bodyParser = require('body-parser');
var multer  = require('multer');
var upload = multer({ dest: './tmp/'});

app.use(express.static('resources'));
app.use(bodyParser.urlencoded({ extended: false }));
//app.use(multer({ dest: './tmp/'}));

app.get('/index.html', function (req, res) {
   res.sendFile( __dirname + "/" + "index.htm" );
})

// upload.single('<fieldname>')
// req.file = {}
// upload.array('<fieldname>')
// req.files = [{}]
// upload.fields('<fieldname>')
// no work
app.post('/file_upload', upload.single('file'), function (req, res) {

   console.log(req);
   console.log(req.file.originalname);
   console.log(req.file.path);
   console.log(req.file.mimetype);

   var file = __dirname + "/" + req.file.orignalname;
   fs.readFile( req.file.path, function (err, data) {
        fs.writeFile(file, data, function (err) {
         if( err ){
              console.log( err );
         }else{
               response = {
                   message:'File uploaded successfully',
                   filename:req.file.filename
              };
          }
          console.log( response );
          res.end( JSON.stringify( response ) );
       });
   });
})

var server = app.listen(8081, function () {

  var host = server.address().address
  var port = server.address().port

  console.log("Example app listening at http://%s:%s", host, port)

})