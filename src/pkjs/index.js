// Listen for when watchface is opened
Pebble.addEventListener('ready', function(e) {
	console.log('PebbleKit JS ready!');
	getJSON();
});

function getJSON() {
	var url = "http://backend-server-sid.herokuapp.com/course/long";
	var xhr = new XMLHttpRequest();
	var obj = {};
	xhr.onload = function () {
		// console.log(JSON.stringify(xhr.response, null, 2));
		var json = JSON.parse(xhr.response);
		obj["name"] = json[0].name;
		obj["fees"] = json[0].fees;
		console.log("fees: " + obj["fees"]);
		Pebble.sendAppMessage(obj, function(e) {
			console.log("Info sent successfully!");
		},
		function(e) {
			console.log("Error sending info.");
		});
	};
	xhr.open("GET", url);
	xhr.responseType = "text";
	console.log("Sending Request");
	xhr.send();

}

// Listen for when AppMessage is received
Pebble.addEventListener('appmessage', function(e) {
	console.log('AppMessage received!');
});
