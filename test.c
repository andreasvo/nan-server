//#include "handler.c"
#include "sql.c"
int main() {

	
	//sql_handle("./database.db", 0, 0, 1, 90790741, "Andreas");
	//char* buffer = malloc(700);
	//read(0, buffer, 700);
	int fd = open("./m4.txt", O_RDONLY);
	char* buf = (char*) malloc(99);
	int bytes = read(fd, buf, 99);
	buf[bytes] = '\0';
	xml_parse(buf, 1);
/*
	int fd = open ("./sample1.txt", O_RDONLY);
	read (fd, buffer, 700);

	xml_parse(buffer, 0);
*/}