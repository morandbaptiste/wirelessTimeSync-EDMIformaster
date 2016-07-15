var clp = require('./clp.js');
var fs = require('fs');

var SerialPort = require('serialport');
var StringDecoder = require('string_decoder').StringDecoder;

var config = undefined;
var tick = undefined;

var configfile = __dirname + "/config.json";
var lastdate = undefined;
var count = 0;
var port = undefined;

function n(n){
    return n > 9 ? "" + n: "0" + n;
}

function requestdata()
{
	var buf = Buffer("02450CE472A742C064E8000052F530ACA203","hex");
	port.write(buf, function(err) {
		if (err) {
			return console.log('Error on write: ', err.message);
		}
		console.log('message written');
	})
}

console.log(clp);

fs.readFile( configfile, 'utf8', function (err, data) {
	if(!err)
	{
		console.log( data );
		var out = JSON.parse( data );
		console.log( out );
	
		config = out;
		
		var watch_triggered = false;
		fs.watch( configfile, function ( event, filename ) {
			if( event == 'change' && filename == "config.json" )
			{
				if(!watch_triggered) 
				{
					fs.readFile( configfile, 'utf8', function (err, data) {
						if(!err)
						{
							var oldconfig = config;
							console.log( data );
							var out = JSON.parse( data );
							console.log( out );
	
							config = out;
							
							if(config.port != oldconfig.port)
							{
								// reset serial
								clearInterval(tick);
								port.close();
								try{
									port = new SerialPort(config.port, {
										baudRate : config.baud,
										parser : SerialPort.parsers.byteDelimiter([clp.config.start, clp.config.end])
									});
								}
								catch(err)
								{
									console.log(err);
								}
							}
							else if(config.baud != oldconfig.baud)
							{
								// update baud rate
								clearInterval(tick);
								port.update({ baudRate : config.baud }, function(err){
									if(!err)
									{
										tick = setInterval(requestdata, config.tick);
									}
									else
									{
										console.log(err.message);
									}
								});
							}
							
							if(config.dataroot != oldconfig.dataroot)
							{
								//  should deal with itself
							}
							
							if(config.tick != oldconfig.tick)
							{
								// restart ticker for samples
								if(tick)
								{
									clearInterval(tick);
									tick = setInterval(requestdata, config.tick);
								}
							}
						}
						else
						{
							console.log(err);
						}
					});
				}
				
				// Stop double hit
				watch_triggered = !watch_triggered;
			}
			else
			{
				console.log("Watch file event: " + event + " " + filename);
			}
		});
		
		fs.readFile( __dirname + "/config-clp.json", 'utf8', function (err, data) {
			if(!err)
			{
				console.log( data );
				var out = JSON.parse( data );
				console.log( out );
	
				clp.config = out;
				console.log( clp );
				
				port = new SerialPort(config.port, {
					baudRate : config.baud,
					parser : SerialPort.parsers.byteDelimiter([clp.config.start, clp.config.end])
				});

				port.on('open', function() {
					console.log("PORT OPEN");
					tick = setInterval(requestdata, config.tick);
				});

				// open errors will be emitted as an error event
				port.on('error', function(err) {
					console.log('Error: ', err.message);
				})

				port.on('data', function(buffer) {
//					var decoder = new StringDecoder('hex');
//					var data = decoder.write( buffer );
					//console.log(buffer);
					
					var noescape = [];
					for(var i = 0; i < buffer.length; ++i)
					{
						if(buffer[i] == 16)
						{
							noescape.push(buffer[i+1]-64);
							i++;
						}
						else
						{
							noescape.push(buffer[i]);
						}
					}
					//console.log(noescape);
					
					var packet = Buffer(noescape);
					console.log(packet);
					var data = undefined;
					var data2 = undefined;
					
					var type = String.fromCharCode(parseInt(buffer[1], 10));
					var parse = clp.parsers[clp.config.commands[type].name];
					console.log(clp.config.commands[type].name);
					if(type == "E")
					{
						console.log(typeof parse);
						data = parse(packet.slice(2,12));
						console.log(packet.slice(2,12));
						type = String.fromCharCode(parseInt(packet[12], 10));
						parse = clp.parsers[clp.config.commands[type].name];
						console.log(clp.config.commands[type].name);
						console.log(packet.slice(13, packet.length-3));
						var com = parse(packet.slice(13, packet.length-3));
						
						for(var i in com)
						{
							data[i] = com[i];
						}
						data.extended = true;
					}
					else
					{
						data = parse(packet);
						data.extended = false;
					}
					
					data.type = type;
					
					console.log('message read');
					console.log( data );
					
					//var f = Buffer(data.data).readFloatBE();
					//parse = clp.parsers.readfloat;
					
					if((data.type == 'R') && (data.reg == 0xF530))
					{
						parse = clp.parsers.readspecialf530;
						data.data =  parse(Buffer(data.data)).data;
						console.log( data );
					
						var out = n(data.data.time_date.getHours()) + ':' + n(data.data.time_date.getMinutes()) + ':' + n(data.data.time_date.getSeconds());
						out += ',' + data.data.voltage[0] + ',' + data.data.voltage[1] + ',' + data.data.voltage[2] ;
						out += ',' + data.data.current[0] + ',' + data.data.current[1] + ',' + data.data.current[2] ;
						out += ',' + data.data.angle[0] + ',' + data.data.angle[1] + ',' + data.data.angle[2] ;
						out += ',' + data.data.angle_vv[0] + ',' + data.data.angle_vv[1] + ',' + data.data.angle_vv[2] ;
						out += ',' + data.data.angle_vi[0] + ',' + data.data.angle_vi[1] + ',' + data.data.angle_vi[2] ;
						out += ',' + data.data.watts[0] + ',' + data.data.watts[1] + ',' + data.data.watts[2] ;
						out += ',' + data.data.vars[0] + ',' + data.data.vars[1] + ',' + data.data.vars[2] ;
						out += ',' + data.data.va[0] + ',' + data.data.va[1] + ',' + data.data.va[2] ;
						out += ',' + data.data.frequency;
						console.log(out);
					
						fs.writeFile( config.dataroot + "/live.csv", out, 'utf8',  function (err, written, buffer) {
							if(err)
							{
								console.log(err);
							}
							else
							{
								console.log("Live file saved.");
							}
						});
					
						var date = n(data.data.time_date.getDate()) + '.' + n(data.data.time_date.getMonth()+1) + '.' + n(data.data.time_date.getFullYear());
						
						fs.appendFile( config.dataroot + '/' + date + ".csv", out + '\r\n', 'utf8',  function (err, written, buffer) {
							if(err)
							{
								console.log(err);
							}
							else
							{
								console.log("Log file saved.");
							}
						});
					}
					else if(data.type == 'V' && data.offset)
					{
						var today = new Date();
						var out = n(today.getHours()) + ':' + n(today.getMinutes()) + ':' + n(today.getSeconds()) + ':' + today.getMilliseconds();
						out += ',' + data.offset;
						
						var filename;
						if(data.master)
						{
							filename = config.dataroot + '/master_offset.csv';
						}
						else
						{
							filename = config.dataroot + '/slave_offset.csv';
						}
						
						fs.appendFile( filename, out + '\r\n', 'utf8',  function (err, written, buffer) {
							if(err)
							{
								console.log(err);
							}
							else
							{
								console.log("Offset file saved.");
							}
						});
					}
				})
			}
			else
			{
				console.log(err);
			}
		});
	}
	else
	{
		console.log(err);
	}
});

SerialPort.list(function (err, ports) {
	ports.forEach(function(port) {
		console.log(port.comName);
		console.log(port.pnpId);
		console.log(port.manufacturer);
	});
});

