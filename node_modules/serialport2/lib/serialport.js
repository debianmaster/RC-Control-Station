'use strict';

var SerialPortBinding = require("bindings")("serialport2.node");
var util = require('util');
var fs = require('fs');
var stream = require('stream');
var path = require('path');
var async = require('async');
var child_process = require('child_process');

function SerialPort () {
  stream.Stream.call(this);
}
util.inherits(SerialPort, stream.Stream);
exports.SerialPort = SerialPort;

SerialPort.prototype.open = function (path, options, callback) {
  var self = this;
  process.nextTick(function () {
    options = options || {};
    options.baudRate = options.baudRate || 9600;
    options.dataBits = options.dataBits || 8;
    options.parity = options.parity || 'none';
    options.stopBits = options.stopBits || 1;
    if (!('flowControl' in options)) {
      options.flowControl = false;
    }
    options.bufferSize = options.bufferSize || 100;
    options.dataCallback = function (data) {
      self.emit('data', data);
    };
    options.errorCallback = function (err) {
      self.emit('error', err);
    };
    options.disconnectedCallback = function () {
      self.emit('error', new Error("Disconnected"));
      self.close();
    };

    if (process.platform == 'win32') {
      path = '\\\\.\\' + path;
    } else {
      self.readStream = fs.createReadStream(path, { bufferSize: options.bufferSize });
      self.readStream.on("data", options.dataCallback);
      self.readStream.on("error", options.errorCallback);
      self.readStream.on("close", function () {
        self.close();
      });
      self.readStream.on("end", function () {
        self.emit('end');
      });
    }

    SerialPortBinding.open(path, options, function (err, fd) {
      self.fd = fd;
      if (callback) {
        callback(err);
      }
      if (err) {
        return self.emit('error', err);
      }

      self.emit('open');
    });
  });
};

SerialPort.prototype.write = function (buffer, callback) {
  var self = this;
  if (!this.fd) {
    if (callback) {
      return callback(new Error("Serialport not open."));
    } else {
      return;
    }
  }

  if (!Buffer.isBuffer(buffer)) {
    buffer = new Buffer(buffer);
  }
  SerialPortBinding.write(this.fd, buffer, function (err, results) {
    if (err) {
      self.emit('error', err);
    }
    if (callback) {
      callback(err, results);
    }
  });
};

SerialPort.prototype.close = function (callback) {
  var fd = this.fd;
  this.fd = 0;

  if (this.closing) {
    return;
  }
  if (!fd) {
    if (callback) {
      return callback(new Error("Serialport not open."));
    } else {
      return;
    }
  }

  this.closing = true;
  try {
    var self = this;
    if (self.readStream) {
      self.readStream.destroy();
    }

    SerialPortBinding.close(fd, function (err) {
      if (err) {
        self.emit('error', err);
      }
      if (callback) {
        callback(err);
      }
      self.emit('close');
      this.closing = false;
    });
  } catch (ex) {
    this.closing = false;
    throw ex;
  }
};

function listUnix (callback) {
  fs.readdir("/dev/serial/by-id", function (err, files) {
    if (err) {
      // if this directory is not found this could just be because it's not plugged in
      if (err.errno === 34) {
        return callback(null, []);
      }
      return console.log(err);
    }

    var dirName = "/dev/serial/by-id";
    async.map(files, function (file, callback) {
      var fileName = path.join(dirName, file);
      fs.readlink(fileName, function (err, link) {
        if (err) {
          return callback(err);
        }
        link = path.resolve(dirName, link);
        callback(null, {
          comName: link,
          manufacturer: undefined,
          pnpId: file
        });
      });
    }, callback);
  });
}

function listOSX (callback) {
  child_process.exec('/usr/sbin/system_profiler SPUSBDataType', function (err, stdout, stderr) {
    if (err) {
      return callback(err);
    }

    stderr = stderr.trim();
    if (stderr.length > 0) {
      return callback(new Error(stderr));
    }

    var lines = stdout.split('\n');
    var items = [];
    var currentItem = {};
    lines.forEach(function (line) {
      line = line.trim();
      line = line.replace(/\s+/, ' ');
      var m;

      if (m = line.match(/^Serial Number: (.+)$/)) {
        currentItem['serialNumber'] = m[1];
      } else if (m = line.match(/^Location ID: (.+)$/)) {
        currentItem['locationId'] = m[1];
      } else if (m = line.match(/^Product ID: (.+)$/)) {
        currentItem['productId'] = m[1];
      } else if (m = line.match(/^Vendor ID: (.+)$/)) {
        currentItem['vendorId'] = m[1];
      } else if (m = line.match(/^Manufacturer: (.+)$/)) {
        currentItem['manufacturer'] = m[1];
      } else if (/^$/.test(line)) {
        if ('serialNumber' in currentItem) {
          currentItem['comName'] = "/dev/cu.usbserial-" + currentItem['serialNumber'];
          items.push(currentItem);
          currentItem = {};
        }
      }
    });
    callback(null, items);
  });
}

exports.list = function (callback) {
  if (process.platform === 'win32') {
    SerialPortBinding.list(callback);
  } else if (process.platform === 'darwin') {
    listOSX(callback);
  } else {
    listUnix(callback);
  }
};
