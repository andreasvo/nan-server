#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <string.h>

// Personal include:
#include "sql.c"

// This program is a get/head/post/put/delete request handler for a the "server.c" webserver.
// to call the function in other programs, keep in same folder and use #include "handler.c" then call "handleRequest();"
// Andreas Vestgarden Olsen & Emil André Hansen (March 2017)

#define BUFFSIZE 8000

void handleRequest() {

	int i = 0;
	int j = 0;
	int a = 0;
	int k = 0;
	int fd = -2;
	int bytes;
	int size = BUFFSIZE;
	char* req;
	char* file;
	char* type;
	char* mime;
	char test;
	int byte_counter = 0;
	char* buffer = malloc(BUFFSIZE);
	byte_counter = read(0, buffer, BUFFSIZE);

	// Grabs request
	while (buffer[i] != ' ')
		i++;
		
	req = malloc(i+1);
	memcpy(req, buffer, i); 
	req[i+1] = '\0';

	// Grabs the full filename
	while (buffer[(i+j)+1] != ' ')
		j++;

	file = malloc(j+1);
	memcpy(file, buffer+i+1, j);
	file[j+1] = '\0';

	char* check = (char*) malloc(18);
	memcpy(check, file, 18);

	// If file is not specified in url, finds index.html as default
	if (1 == j) {
		file = "/index.html";
		type = "html";
	}


	// GET: SEARCH and DELETE request
	if((0 == strcmp(req, "DELETE") || 0 == strcmp(req, "GET")) && 0 == strcmp(check, "/webroot/incoming/")) {

		char* sw;
		sw = (char*) malloc((j)-18);
		memcpy(sw, file+18, (j)-18);
		int id = atoi(sw);

		if (18 != j) {
			if (0 == strcmp(req, "GET")) 
				sql_handle ("/db/database.db", 0, -1, id, -1, "");
			else if (0 == strcmp(req, "DELETE"))
				sql_handle ("/db/database.db", 3, -1, id, -1, "");
			
			exit(0);

		} else {
			if (0 == strcmp(req, "GET")) 
				sql_handle ("/db/database.db", 0, -1, -1, -1, "");
			else if (0 == strcmp(req, "DELETE"))
				sql_handle ("/db/database.db", 3, -1, -1, -1, "");

			exit(0);
		}
	}

	// POST or PUT request
	if (0 == strcmp(req, "POST") || 0 == strcmp(req, "PUT")) {
		int counter = 0;
		i = 0;
		while (i < byte_counter) {

			if (counter == 2) {
				break;
			}

			if (buffer[i] == '\n') {
				counter++;
			}

			if (buffer[i] != '\r' && buffer[i] != ' ' && buffer[i] != '\t' && buffer[i] != '\n') {
				counter = 0;
			}

			i++;	
		}

		char* xml_buf = malloc(byte_counter+1 - i);
		memcpy(xml_buf, buffer + i, byte_counter-i);
		xml_buf[i+1] = '\0';


		if (0 ==  strcmp(req, "POST")) {
			xml_parse(xml_buf, 1);
			exit(0);
		}

		else if (0 == strcmp(req, "PUT")) {
			xml_parse(xml_buf, 2);
			exit(0);
		} else {
			printf("Something went wrong!");
			exit(0);
		}
	}

	// tries to open the file, -1 returned if not found, else a filedescriptor referring to the file
	fd = open(file, O_RDONLY);

	if (-1 == fd && 0 != strcmp(check, "/webroot/incoming/") && 0 == strcmp(req, "GET")) {
		printf("HTTP/1.1 404 NOT FOUND\n");
		printf("Content-Type: text/html\n\n");
		fd = open("404.html", O_RDONLY);

		while (bytes = read(fd, buffer, BUFFSIZE)) {
			byte_counter = bytes;
			write(1, buffer, bytes);
		}

		exit(0);
	}
	
	if (j > 1) {
		// grabs the filetype
		while (buffer[(i+a)+1] != '.' ) {
			test = buffer[(i+a)+1];
			a++;
		}

		while (buffer[(i+a+2)+k] != ' ') {
			test = buffer[(i+a+2)+k];
			k++;
		}
		
		type = malloc(k+1);

	} else {
		type = malloc(5);
	}

	memcpy(type, buffer+i+a+2, k);
	type[k+1] = '\0';


	

	// Checks if image	
	if (0 == strcmp(type, "png") || 0 == strcmp(type, "jpg")) {
		mime = malloc(6);
		mime = "image";
	}

	// checks if xml and/or xsl
	else if (0 == strcmp(type, "xsl") || 0 == strcmp(type, "xml") || 0 == strcmp(type, "xml-dtd") || 0 == strcmp(type, "javascript")) {
		mime = malloc(12);
		mime = "application"; 

	// else sets texts as default	
	} else {
		mime = malloc(5);
		mime = "text";
	}

	printf("HTTP/1.1 200 OK\n");
	printf("Content-Type %s/%s\n\n", mime, type);

	if(0 == strcmp(req, "GET")) {
		bytes = 0;
		while (bytes = read(fd, buffer, BUFFSIZE)) {
			byte_counter = bytes;
			write(1, buffer, bytes);
		}
	}
}