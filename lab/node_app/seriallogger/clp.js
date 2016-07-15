var Parser = require('binary-parser').Parser;

exports.parsers = {

extended : function(data)
{
	var parser = new Parser()
		.endianess('big')
		.uint32('dest')
		.uint32('src')
		.uint16('frame');

	return parser.parse(data);
},
	
read : function(data)
{
	var parser = new Parser()
		.endianess('big')
		.uint16('reg')
		.array('data', { type : 'uint8', length : data.length - 2 });

	return parser.parse(data);
},

readfloat : function(data)
{
	var parser = new Parser()
		.endianess('big')
		.float('data')

	return parser.parse(data);
},

readuint32 : function(data)
{
	var parser = new Parser()
		.endianess('big')
		.uint32('data')

	return parser.parse(data);
},

readuint16 : function(data)
{
	var parser = new Parser()
		.endianess('big')
		.uint16('data')

	return parser.parse(data);
},

readuint8 : function(data)
{
	var parser = new Parser()
		.endianess('big')
		.uint8('data')

	return parser.parse(data);
},

readspecialf530 : function(data)
{
	var parser = new Parser()
		.endianess('big')
		.array('voltage', { 'type' : 'uint8', 'length' : 12 })
		.array('current', { 'type' : 'uint8', 'length' : 12 })
		.array('angle', { 'type' : 'uint8', 'length' : 12 })
		.array('angle_vv', { 'type' : 'uint8', 'length' : 12 })
		.array('angle_vi', { 'type' : 'uint8', 'length' : 12 })
		.array('watts', { 'type' : 'uint8', 'length' : 12 })
		.array('vars', { 'type' : 'uint8', 'length' : 12 })
		.array('va', { 'type' : 'uint8', 'length' : 12 })
		.float('frequency')
		.uint8('button_jumper_state')
		.uint8('elements')
		.string('active_efa', { length : 17 })
		.string('latched_efa', { length : 17 })
		.uint32('time_date')
		
	var values = parser.parse(data);
	//console.log(values);
		
	var out = {};
	
	out.voltage = [];
	for(var i = 0; i < values.voltage.length / 4; ++i)
	{
		var b = Buffer(values.voltage.slice(i*4,i*4+4));
		var f = b.readFloatBE();
		out.voltage.push(f);
	}
	
	out.current = [];
	for(var i = 0; i < values.current.length / 4; ++i)
	{
		var b = Buffer(values.current.slice(i*4,i*4+4));
		var f = b.readFloatBE();
		out.current.push(f);
	}
	
	out.angle = [];
	for(var i = 0; i < values.angle.length / 4; ++i)
	{
		var b = Buffer(values.angle.slice(i*4,i*4+4));
		var f = b.readFloatBE();
		out.angle.push(f);
	}
	
	out.angle_vv = [];
	for(var i = 0; i < values.angle_vv.length / 4; ++i)
	{
		var b = Buffer(values.angle_vv.slice(i*4,i*4+4));
		var f = b.readFloatBE();
		out.angle_vv.push(f);
	}
	
	out.angle_vi = [];
	for(var i = 0; i < values.angle_vi.length / 4; ++i)
	{
		var b = Buffer(values.angle_vi.slice(i*4,i*4+4));
		var f = b.readFloatBE();
		out.angle_vi.push(f);
	}
	
	out.watts = [];
	for(var i = 0; i < values.watts.length / 4; ++i)
	{
		var b = Buffer(values.watts.slice(i*4,i*4+4));
		var f = b.readFloatBE();
		out.watts.push(f);
	}
	
	out.vars = [];
	for(var i = 0; i < values.vars.length / 4; ++i)
	{
		var b = Buffer(values.vars.slice(i*4,i*4+4));
		var f = b.readFloatBE();
		out.vars.push(f);
	}
	
	out.va = [];
	for(var i = 0; i < values.va.length / 4; ++i)
	{
		var b = Buffer(values.va.slice(i*4,i*4+4));
		var f = b.readFloatBE();
		out.va.push(f);
	}
	
	out.frequency = values.frequency;
	
	var date = new Date(0);
	date.setFullYear(1996);
	date.setTime(date.getTime() + values.time_date * 1000);
	out.time_date = date;
	
	return { 'data' : out };
},

write : function (packet)
{
	var clp = {};

	return clp;
},

logon : function (packet)
{
	var clp = {};

	return clp;
},

securelogon : function (packet)
{
	var clp = {};

	return clp;
},

logoff : function (packet)
{
	var clp = {};

	return clp;
},

fileaccess : function (packet)
{
	var clp = {};

	return clp;
},

timesync : function (buffer)
{
	var clp = {};

	return clp;
},

event : function (data)
{
	var parser = new Parser()
		.endianess('big')
		.uint8('type')

	var out = parser.parse(data.slice(2));
	var off = undefined;
	
	console.log(out)
	switch(out.type)
	{
		// MASTER
		case 1: {
			parser = new Parser()
				.endianess('little')
				.int32('offset')
				
			off = parser.parse(data.slice(3));
			out.master = true;
			
		}
		break;
		// SLAVE
		case 2: {
			parser = new Parser()
				.endianess('little')
				.int32('offset')
				
			off = parser.parse(data.slice(3));
			out.master = false;
			
		}
		break;
		default: return;
	}
	
	out.offset = off.offset;
	
	return out;
}

}