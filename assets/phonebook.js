var tableAlive = 0;

window.addEventListener("load", function() {
	document.getElementById("searchButton").addEventListener("click", searchFunct, false);
	document.getElementById("insertButton").addEventListener("click", insertFunct, false);
	document.getElementById("updateButton").addEventListener("click", updateFunct, false);
	document.getElementById("deleteButton").addEventListener("click", deleteFunct, false);
	document.getElementById("testButton").addEventListener("click", testFunct, false);
	document.getElementById("resetButton").addEventListener("click", resetFunct, false);
});

function testFunct() {
    var blyat = javascriptGrensesnitt.getContacts("");
}

function resetFunct() {
	document.getElementById("form").reset();
	document.getElementById("response").innerHTML="";
	document.getElementById("outputTable").innerHTML="";

	if (tableAlive == 1) {
		document.getElementById("outputTable").innerHTML="";
		tableAlive = 0;
	}
}

function tableFunct(xml, identifier) {
	var xml_string = xml.responseText;
	var emptyReply = "<phonebook></phonebook>";

	if (tableAlive == 1) {
		document.getElementById("outputTable").textContent="";
		tableAlive = 0;
	}

	if (xml_string.length > emptyReply.length) {

		var parser = new DOMParser();

		var doc = parser.parseFromString(xml_string, "application/xml");

		var table = "<tr><th>ID</th><th>TLF</th><th>Name</th></tr>";
	  	var x = doc.getElementsByTagName("person");

	  	for (var i = 0; i < x.length; i++) {

	  		var name = x[i].getElementsByTagName("name")[0].childNodes[0];

	  		if (!name) {
	  			name = "";
	  		} else {
	  			name = x[i].getElementsByTagName("name")[0].childNodes[0].nodeValue;
	  		}

		    table += "<tr><td>" +
		    x[i].getElementsByTagName("id")[0].childNodes[0].nodeValue +
		    "</td><td>" +
		    x[i].getElementsByTagName("tlf")[0].childNodes[0].nodeValue +
		    "</td><td>" +
		    name +
		    "</td></tr>";

		}

        //if (identifier == "server") {
            document.getElementById("outputTable").innerHTML = table;
            document.getElementById("response").innerHTML = "Database output:";
        /*}

        else if (identifier == "phone") {
            document.getElementById("phoneTable").innerHTML = table;
        }*/

 	} else {
 		document.getElementById("response").innerHTML = "Not available";
 	}
}

function searchFunct() {

	var id = document.getElementsByName("oldID")[0].value;
	var path;
	xml_req = new XMLHttpRequest();

	if (tableAlive == 1) {
		document.getElementById("outputTable").textContent="";
		tableAlive = 0;
	}


	if(id) {
		path = "http://192.168.42.116/webroot/incoming/"+id;
	} else {
		path = "http://192.168.42.116/webroot/incoming/";
	}

	xml_req.open("GET", path, true);

	xml_req.send(xml_req);

	xml_req.onreadystatechange = function() {
		if (xml_req.readyState == XMLHttpRequest.DONE) {
			tableFunct(this, "server");
		}
	}

	phone_xml = "";

	if (id) {
	    tableFunct(javascriptGrensesnitt.getContacts(), "phone");
	} else {
	    tableFunct(javascriptGrensesnitt.getContacts(), "phone");
	}

	var t = javascriptGrensesnitt.getContacts();

    document.getElementById("response").innerHTML=t;
}

function insertFunct() {
	var id = document.getElementsByName("ID")[0].value;
	var tlf = document.getElementsByName("TLF")[0].value;
	var name = document.getElementsByName("NAME")[0].value;

	if (!id && !tlf) {
		javascriptGrensesnitt.visToast("ID and/or TLF cannot be empty!");
		return;
	}

	if (!id) {
		javascriptGrensesnitt.visToast('ID cannot be empty!');
		return;
	}

	if (!tlf) {
		javascriptGrensesnitt.visToast("TLF cannot be empty!");
		return;
	}

	var xml_doc = "<phonebook><person><id>"+id+"</id><tlf>"+tlf+"</tlf><name>"+name+"</name></person></phonebook>";
	xml_req = new XMLHttpRequest();

	xml_req.open("POST", "http://192.168.42.116/webroot/incoming/", true);

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
	var oldId = document.getElementsByName("oldID")[0].value;
	var id = document.getElementsByName("ID")[0].value;
	var tlf = document.getElementsByName("TLF")[0].value;
	var name = document.getElementsByName("NAME")[0].value;

	if (!oldId && !tlf) {
		javascriptGrensesnitt.visToast("Select ID and/or TLF cannot be empty!");
		return;
	}

	if (!oldId) {
		javascriptGrensesnitt.visToast('Existing ID cannot be empty!')
		return;
	}

	if (!tlf) {
		javascriptGrensesnitt.visToast("TLF cannot be empty!");
		return;
	}

	if(!id) {
		id = "-1";
	}

	var xml_doc = "<phonebook><person><id>"+id+"</id><tlf>"+tlf+"</tlf><name>"+name+"</name></person></phonebook>";
	xml_req = new XMLHttpRequest();

	xml_req.open("PUT", "http://192.168.42.116/webroot/incoming/"+oldId, true);

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
	var id = document.getElementsByName("oldID")[0].value;
	var path;
	xml_req = new XMLHttpRequest();

	if(id) {
		path = "http://192.168.42.116/webroot/incoming/"+id;
	} else {
		path = "http://192.168.42.116/webroot/incoming/";
	}

	xml_req.open("DELETE", path, true);

	xml_req.send(xml_req);

	xml_req.onreadystatechange = function() {
		if (xml_req.readyState == XMLHttpRequest.DONE) {

			document.getElementById("response").textContent=xml_req.responseText;

			if(!xml_req.responseText) {
				document.getElementById("response").textContent="Empty response, sorry!";
			}
		}
	}
}