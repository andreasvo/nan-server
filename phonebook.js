var tableAlive = 0;

window.addEventListener("load", function() {
	document.getElementById("searchButton").addEventListener("click", searchFunct, false);
	document.getElementById("insertButton").addEventListener("click", insertFunct, false);
	document.getElementById("updateButton").addEventListener("click", updateFunct, false);
	document.getElementById("deleteButton").addEventListener("click", deleteFunct, false);
	document.getElementById("resetButton").addEventListener("click", resetFunct, false);
});

function resetFunct() {
	document.getElementById("form").reset();
	document.getElementById("response").innerHTML="";
	document.getElementById("outputTable").innerHTML="";
	
	if (tableAlive == 1) {
		document.getElementById("outputTable").innerHTML="";
		tableAlive = 0;
	}
}

function tableFunct(xml) {
	var i;
	var xml_string = xml.responseText;
	
	if (tableAlive == 1) {
		document.getElementById("outputTable").textContent="";
		tableAlive = 0;
	}

	if (xml_string.length > 23) {
		var parser = new DOMParser();

		var doc = parser.parseFromString(xml_string, "application/xml");

		var table = "<tr><th>ID</th><th>TLF</th><th>Name</th></tr>";

	  	var x = doc.getElementsByTagName("person");
	  	for (i = 0; i <x.length; i++) { 
		    table += "<tr><td>" +
		    x[i].getElementsByTagName("id")[0].childNodes[0].nodeValue +
		    "</td><td>" +
		    x[i].getElementsByTagName("tlf")[0].childNodes[0].nodeValue +
		    "</td><td>" +
		    x[i].getElementsByTagName("name")[0].childNodes[0].nodeValue +
		    "</td></tr>";
		}

		document.getElementById("outputTable").innerHTML = table;
		document.getElementById("response").innerHTML = "Database output:";
		tableAlive = 1;

 	} else {
 		document.getElementById("response").innerHTML = "Empty database!";
 	}
}

function searchFunct() {
	var id = document.getElementsByName("ID")[0].value;
	var tlf = document.getElementsByName("TLF")[0].value;
	var name = document.getElementsByName("NAME")[0].value;
	var path;
	xml_req = new XMLHttpRequest();

	if (tableAlive == 1) {
		document.getElementById("outputTable").textContent="";
		tableAlive = 0;
	}


	if(id) {
		path = "/webroot/incoming/"+id;
	} else {
		path = "/webroot/incoming/";
	}

	xml_req.open("GET", path, true);

	xml_req.send(xml_req);

	xml_req.onreadystatechange = function() {
		if (xml_req.readyState == XMLHttpRequest.DONE) {
			tableFunct(this);		
			if(!xml_req.responseText) {
				document.getElementById("response").innerHTML="Empty response, sorry!";
			}
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
		alert('ID cannot be empty!');
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

	xml_req.open("PUT", "/webroot/incoming/"+id, true);

	xml_req.send(xml_doc);

	xml_req.onreadystatechange = function() {
		if (xml_req.readyState == XMLHttpRequest.DONE) {

			document.getElementById("response").textContent=xml_req.responseText;

			if(!xml_req.responseText){
				document.getElementById("response").textContent="Empty response, sorry!";
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

			document.getElementById("response").textContent=xml_req.responseText;

			if(!xml_req.responseText){
				document.getElementById("response").textContent="Empty response, sorry!";
			}
		}
	}
}