var connection="mongodb://admin:computer@dharma.mongohq.com:10016/store";
var Server = require('mongodb').Server,
    db;
var mongodb = require('mongodb');
db = new mongodb.Db('store', new mongodb.Server('dharma.mongohq.com', 10016, {auto_reconnect:true}), {});

db.open(function(err, p_client) {  
  db.authenticate('admin', 'computer', function(err) {   
   if (err) console.log(err);      
   console.log("logggedin");
  });
}); 

exports.findById = function(req, res) {
    console.log(req.params);
    var id = parseInt(req.params.id);
    console.log('findById: ' + id);
    db.collection('employees', function(err, collection) {
        collection.findOne({'id': id}, function(err, item) {
            console.log(item);
            res.jsonp(item);
        });
    });
};

exports.findByManager = function(req, res) {
    var id = parseInt(req.params.id);
    console.log('findByManager: ' + id);
    db.collection('employees', function(err, collection) {
        collection.find({'managerId': id}).toArray(function(err, items) {
            console.log(items);
            res.jsonp(items);
        });
    });
};

exports.findAll = function(req, res) {
    var name = req.query["name"];
    db.collection('employees', function(err, collection) {
        if (name) {
            collection.find({"fullName": new RegExp(name, "i")}).toArray(function(err, items) {
                res.jsonp(items);
            });
        } else {
            collection.find().toArray(function(err, items) {
                res.jsonp(items);
            });
        }
    });
};
exports.insertDummyData=function(){
	console.log("Populating employee database...");
	var categories=[{id:1,name:"Wireless"},{id:2,name:"Development Boards"},{id:3,name:"Sensors"}];
	var subCats=[{id:1,name:"RF",parent:1},{id:2,name:"XBEE",parent:1},{id:3,name:"Wifi",parent:1},{id:4,name:"Bluetooth",parent:1}];
	subCats.push({id:5,name:"Arduino",parent:2});
	subCats.push({id:6,name:"ARM",parent:2});
	subCats.push({id:7,name:"Ultrasonic",parent:3});
	subCats.push({id:8,name:"GPS",parent:3});
	subCats.push({id:9,name:"IR",parent:3});
	var p1=
	{
		name:"NRF24L01",
		title:"1PCS Arduino NRF24L01+ Wireless Transceiver Module (create star or mesh network)",	
		caption:"Easy, Cheap & long range wireless communication for MCU",
		condition:"New",
		cost:125,
		shipping:55,
		features:"We are the original importers of nordic devices, the first one to launch nRF24L01+ and nRF24LE01 in India. Please buy from us original and tested products and support innovation and research in India. When you buy from copycats, fake, and untested products, it hurts not only you in the long run, but everyone else",
		documents:"",
		contents:"<ul><li>One assembled and tested board</li><li>One 16x2 LCD</li><li>One DC Adapter (Not included in shipments out side India)</li><li>Set of 20 F-F Single Pin Connectors</li></ul>",	
		sku:"i63-NRF24L01",
		subCat:2,
		images:["test1.png","test2.png","test3.png","test4.png"]	
	}
    //db.collection('Categories', function(err, collection) {
        //collection.insert(categories, {safe:true}, function(err, result) {});		
		//db.collection('SubCats', function(err, collection) {
			//collection.insert(categories, {safe:true}, function(err, result) {});		
			db.collection('Products', function(err, collection) {
				for(var k=2;k<10;k++){
					p1.subCat=k;
					collection.insert(p1, {safe:true}, function(err, result) {});		
				}
			});
		//});
    //});	
};
var populateDB = function() {
 
    
 
};