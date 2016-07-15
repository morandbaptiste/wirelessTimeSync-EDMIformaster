var express = require('express');
var app = express();
var fs = require("fs");
var dot = require("dot");

var config = undefined;

app.use(express.static('resources'));

function LiveCSVToJSON(csv)
{
  var values = csv.split(",");
  var json = { 
               time     : values[0],
               voltage  : [ values[1], values[2], values[3] ],
               current  : [ values[4], values[5], values[6] ],
               angle    : [ values[7], values[8], values[9] ],
               angle_vv : [ values[10], values[11], values[12] ],
               angle_vi : [ values[13], values[14], values[15] ],
               watts    : [ values[16], values[17], values[18] ],
               vars     : [ values[19], values[20], values[21] ],
               va       : [ values[22], values[23], values[24] ],
               frequency: values[25]
             };

  return json;
}

function HistoryCSVToJSON(csv)
{
  var lines = csv.split("\r\n");
  var json = { live: false,
		      data: []
             };

  for(var i=0;i<lines.length;++i)
  {
    var values = lines[i].split(",");
    json.data.push({
                     time     : values[0],
                     voltage  : [ values[1], values[2], values[3] ],
                     current  : [ values[4], values[5], values[6] ],
                     angle    : [ values[7], values[8], values[9] ],
                     angle_vv : [ values[10], values[11], values[12] ],
                     angle_vi : [ values[13], values[14], values[15] ],
                     watts    : [ values[16], values[17], values[18] ],
                     vars     : [ values[19], values[20], values[21] ],
                     va       : [ values[22], values[23], values[24] ],
                     frequency: values[25]
    });
  }

  return json;
}

app.get('/live', function (req, res) {
   fs.readFile( __dirname + config.dataroot + "/" + config.livedata, 'utf8', function (err, data) {
     fs.readFile( __dirname + "/resources/" + "live.html", 'utf8', function (err, format) {
         console.log( data );
         var live = LiveCSVToJSON( data );

         console.log( live ) ;
         console.log( format );
         var gen = dot.template( format );
         var out = gen( live );

         console.log( out );
         res.end( out );
    });
  });
})

app.get('/live/json', function (req, res) {
   fs.readFile( __dirname + config.dataroot + "/" + config.livedata, 'utf8', function (err, data) {
       console.log( data );
       var out = LiveCSVToJSON( data );

       console.log( out );
       res.end( JSON.stringify(out) );
   });
})

app.get('/history', function (req, res) {
   fs.readFile( __dirname + config.dataroot + "/" + config.histdata, 'utf8', function (err, data) {
      fs.readFile( __dirname + "/resources/" + "history.html", 'utf8', function (err, format) {
         console.log( data );
         var h = HistoryCSVToJSON( data );

         console.log( h ) ;
         console.log( format );
         var gen = dot.template( format );
         var out = gen( h );

         console.log( gen );
         console.log( out );
         res.end( out );
      });
   });
})

app.get('/history/json', function (req, res) {
   fs.readFile( __dirname + config.dataroot + "/" + config.histdata, 'utf8', function (err, data) {
       console.log( data );
       var out = HistoryCSVToJSON( data );
       console.log( out );
       res.end( JSON.stringify(out) );
   });
})

app.get('/plot', function (req, res) {
  res.sendFile( __dirname + '/resources/line-test-json.html', null, function (err) {

  });
})

app.get('/favicon.ico', function (req, res) {
  res.sendFile( __dirname + '/resources/ss.png', null, function (err) {

  });
})

app.get('/:id', function (req, res) {
   // First read existing users.
   fs.readFile( __dirname + "/resources/" + "users.json", 'utf8', function (err, data) {
       users = JSON.parse( data );
       var user = users["user" + req.params.id] 
       console.log( user );
       res.end( JSON.stringify(user));
   });
})

app.get('/*', function (req, res) {
    res.status(404).sendFile(  __dirname + "/resources/404.html", null, function (err) {

  });
})

fs.readFile( __dirname + "/config.json", function (err, data) {
	if(!err)
	{
		var out = JSON.parse( data );
		
		var server = app.listen(config.port, function () {

			var host = server.address().address
			var port = server.address().port

			console.log("Example app listening at http://%s:%s", host, port)
		})
	}
	else
	{
		console.log(err);
	}
})