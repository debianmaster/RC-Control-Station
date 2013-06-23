var io = require("socket.io").listen(3000);
var SerialPort = require('serialport2').SerialPort;
var port = new SerialPort();

port.on('data', function(data) {
  console.log(data.toString());
  //socket.emit("data", data.toUpperCase(), { length: data.length });
});

port.on('error', function(err) {
  console.log(err);
});

port.open('COM11', {
  baudRate: 115200
},function(err) {    
});


io.sockets.on("connection", function (socket) {     
   socket.on("data", function (data) {      
	   console.log(data);	
      if (data) {
		if(data=="close")
		port.close();
		else if(data=="start"){
			port.open('COM11', {
				baudRate: 115200
				},function(err) {
			});
		 }		
		else
		port.write(data.toUpperCase());		         
      }
   });
});
process.on('uncaughtException', function (err) {
  console.log('Caught exception: ' + err);
});
//socket.emit("data", data.toUpperCase(), { length: data.length });