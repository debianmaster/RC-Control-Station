var cValues=[1000,1000,1000,1000,1000];
var socket;
$(function() {
    socket = io.connect("http://localhost:3000/");        
	$('.span2').slider()
		.on('slide', function(ev){     
			cValues[parseInt($(ev.target).attr('id').replace('c',''))]=parseInt(ev.value);
			console.log(cValues.toString());
			//socket.emit('data',cValues.toString());
	});
	setInterval(function(){
		$("[id^='c']").each(function(index){
			var val=1000;
			if($(this).val().length>0)
			val=$(this).val();
			cValues[index]=val;
		});
		socket.emit('data',cValues.toString());		
	},500);
	/*
	socket.on("data",function(data){
		console.log(data);
	});    
	*/
});
function fnStart(){
	$(".label-warning").removeClass('label-warning').addClass("label-success");
	socket.emit('data',"start");
	socket.emit('data',cValues.toString());
}
function fnStop(){	
	$(".label-success").removeClass('label-success').addClass("label-warning");
	socket.emit('data',"stop");
}