window.addEventListener("load", function() {

	document.getElementById("searchButton").addEventListener("click", searchFunct, false);
	document.getElementById("insertButton").addEventListener("click", insertFunct, false);
	document.getElementById("updateButton").addEventListener("click", updateFunct, false);
	document.getElementById("deleteButton").addEventListener("click", deleteFunct, false);

});

function searchFunct() {
	var id = document.getElementsByName("ID")[0].value;
	var tlf = document.getElementsByName("TLF")[0].value;
	var name = document.getElementsByName("NAME")[0].value;
	var path;
	xml_req = new XMLHttpRequest();

	if(id) {
		path = "/webroot/incoming/"+id;
	} else {
		path = "/webroot/incoming/";
	}

	xml_req.open("GET", path, true);

	xml_req.send(xml_req);

	xml_req.onreadystatechange = function() {
		if (xml_req.readyState == XMLHttpRequest.DONE) {
			//document.getElementById("response").innerHTML=xml_req.responseText;			
			if(!xml_req.responseText) {
				document.getElementById("response").innerHTML="Empty response, sorry!";
			}

			var xml_response = xml_req.responseText;
			var i;
			var table="<tr><th>ID</th><th>TLF</th><</tr>";
		  	var x = xml_response.getElementsByTagName("person");
		  	
		  	for (i = 0; i < x.length; i++) { 
			    table += "<tr><td>" +
			    x[i].getElementsByTagName("id")[0].childNodes[0].nodeValue +
			    "</td><td>" +
			    x[i].getElementsByTagName("tlf")[0].childNodes[0].nodeValue +
			    "</td><td>" +
			    x[i].getElementsByTagName("name")[0].childNodes[0].nodeValue +
			    "</td></tr>";
		  	}

		  	document.getElementById("outputTable").innerHTML = table;
		}
	}
}

function insertFunct() {
	var id = document.getElementsByName("ID")[0].value;
	var tlf = document.getElementsByName("TLF")[0].value;
	var name = document.getElementsByName("NAME")[0].value;

	if (!id && !tlf) {
		alert("ID and/or TLF cannot be empty!");
		return;
	}

	if (!id) {
		alert('ID cannot be empty!')
		return;
	}

	if (!tlf) {
		alert("TLF cannot be empty!");
		return;
	}

	var xml_doc = "<phonebook><person><id>"+id+"</id><tlf>"+tlf+"</tlf><name>"+name+"</name></person></phonebook>";
	xml_req = new XMLHttpRequest();

	xml_req.open("POST", "/webroot/incoming", true);

	xml_req.send(xml_doc);

	xml_req.onreadystatechange = function(){
		if (xml_req.readyState == XMLHttpRequest.DONE) {
			document.getElementById("response").innerHTML=xml_req.responseText;
			if(!xml_req.responseText){
				document.getElementById("response").innerHTML="Empty response, sorry!";
			}
		}
	}
}

function updateFunct() {
	var id = document.getElementsByName("ID")[0].value;
	var tlf = document.getElementsByName("TLF")[0].value;
	var name = document.getElementsByName("NAME")[0].value;

	if (!id && !tlf) {
		alert("ID and/or TLF cannot be empty!");
		return;
	}

	if (!id) {
		alert('ID cannot be empty!')
		return;
	}

	if (!tlf) {
		alert("TLF cannot be empty!");
		return;
	}

	var xml_doc = "<phonebook><person><id>"+id+"</id><tlf>"+tlf+"</tlf><name>"+name+"</name></person></phonebook>";
	xml_req = new XMLHttpRequest();

	xml_req.open("PUT", "/webroot/incoming", true);

	xml_req.send(xml_doc);

	xml_req.onreadystatechange = function(){
		if (xml_req.readyState == XMLHttpRequest.DONE) {
			document.getElementById("response").innerHTML=xml_req.responseText;
			if(!xml_req.responseText){
				document.getElementById("response").innerHTML="Empty response, sorry!";
			}
		}
	}
}

function deleteFunct() {
	var id = document.getElementsByName("ID")[0].value;
	var tlf = document.getElementsByName("TLF")[0].value;
	var name = document.getElementsByName("NAME")[0].value;
	var path;
	xml_req = new XMLHttpRequest();

	if(id) {
		path = "/webroot/incoming/"+id;
	} else {
		path = "/webroot/incoming/";
	}

	xml_req.open("DELETE", path, true);

	xml_req.send(xml_req);

	xml_req.onreadystatechange = function() {
		if (xml_req.readyState == XMLHttpRequest.DONE) {
			document.getElementById("response").innerHTML=xml_req.responseText;
			if(!xml_req.responseText){
				document.getElementById("response").innerHTML="Empty response, sorry! lol";
			}
		}
	}
}