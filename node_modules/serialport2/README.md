# SerialPort2

Node.js serial port driver. Works on Windows, OSX, and linux.

# Install

```
npm install serialport2
```

**Note for windows users.** If you are using Node v0.6.x you will need to have Python 2.7 installed.
See [node-gyp](https://github.com/TooTallNate/node-gyp) for instruction on getting a working node-gyp
environment.

# Quick Example

```javascript
var SerialPort = require('serialport2').SerialPort;
var port = new SerialPort();

port.on('data', function(data) {
  console.log(data.toString());
});

port.on('error', function(err) {
  console.log(err);
});

port.open('COM4', {
  baudRate: 9600,
  dataBits: 8,
  parity: 'none',
  stopBits: 1
}, function(err) {
  port.write("hello world");
  port.close();
});
```

```javascript
var sp = require('../');
sp.list(function(err, ports) {
  console.log(ports);
});
```

# API

## SerialPort

### new SerialPort()

Creates the serial port.

### open(portName, [options], [callback])

Open the serial port.

__Arguments__

  * portName - The name of the port to open. Example: Windows 'COM1', Linux '/dev/ttyUSB0'.
  * options - Options for open
    * baudRate - The baud rate [default: 9600]
    * dataBits - The data bits [default: 8]
    * parity - The parity, can be (none, odd, even) [default: 'none']
    * stopBits - The number of stop bits, can be (1, 1.5, 2) [default: 1]
    * flowControl - Enable flow control (true, false) [default: false]
  * callback(err) - Callback called after opening the port.

### write(buffer, [callback])

Writes data to the serial port.

__Arguments__

  * buffer - This can be a node Buffer object or a string.
  * callback(err, byteWritten) - Callback called after writing bytes.

### close(callback)

Closes the serial port.

__Arguments__

  * callback(err) - Callback called after the port is closed.

### Event: data

Event emitted by SerialPort when data is available. The first argument will contain a Buffer with the data.

### Event: error

Event emitted by SerialPort if an error occurs. The first argument will contain the error.

## list(callback)

Lists the serial ports on the machine.

__Arguments__

  * callback(err, ports) - Callback called after the list is retrieved.
