RC-Control-Station
==================
Control Quad from PC/Webinterface</h1>
<h3>Also Flysky TX module implemented for Arduino </h3>
<h4>Note: All credit for Flysky TX module of arduino goes to midelic on RCGroups.com</h4>

Usage:    
<ol>
<li>Install Arduino/FlySky_Tx_Arduino.ino  on arduino</li> 
<li>Arduino code is specially for TX/RX systems which work on Flysky protocol</li> 
<li>Run sockServer.js on nodejs like this     C:\> node sockServer.js    # this is for socket which interacts with serial port and web ui </li>
<li>Run server.js on nodejs like this     C:\> node server.js  # this one is for web ui of the transmitter controls, this polls the control data every half second to socket server</li>
<li>Run sockServer.js on nodejs like this     C:\> node sockServer.js  </li>
<li>goto   http://localhost:4000 on browser</li>
<li>No more steps, you are ready!</li>
</ol>

Libraries used:
<ol>
<li> npm install serialport2 </li>
<li> nodejs serialport2 # this one gave me hard time installing it so make sure you have python 2.7.*  installed (no higher versions) before installing serialport2</li>
<li> npm install express </li>
<li> npm install ejs </li>
<li> npm install socket.io </li>
</ol>

Demo snapshot:

<img src="https://raw.github.com/debianmaster/RC-Control-Station/master/static/img/rc+control_center.png"/>


