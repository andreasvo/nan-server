#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sqlite3.h>

// Handles xml parsing and sql actions for server.c via handle.c that calls it
// Andreas Vestgarden Olsen (March 2017)

void xml_parse(char* buf, int action, int flag);
static int sql_handle (char* db_path, int action, int flag, int id, int tlf, char* name);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);

void xml_parse(char* buf, int action, int flag) {

    int i, j, k, input_size, id, tlf;
    char* name;
    char* temp1;
    char* temp2;

    // input extraction
    i = 0;
    while(buf[i] != '\0') {
        
        if (buf[i] == '<' && buf[i+1] == '/' && buf[i+2] == 'p' && buf[i+3] == 'h') {
            break;
        }
        
        // id extraction
        if (buf[i] == '<' && buf[i+1] == 'i' && buf[i+2] == 'd' && buf[i+3] == '>') {
            j = i+4;
            input_size = 0;
            while (buf[j] != '<' && buf[j+1] != '/' && buf[j+2] != 'i' && input_size < 25) {
                input_size++;
                j++;
            }
            temp1 = (char*) malloc(input_size*2);
            memcpy(temp1, buf+(i+4), input_size);
            id = atoi(temp1);
            free(temp1);
        }

        // tlf extraction
        if (buf[i] == '<' && buf[i+1] == 't' && buf[i+2] == 'l' && buf[i+3] == 'f' && buf[i+4] == '>') {
            j = i+5;
            input_size = 0;
            while (buf[j] != '<' && buf[j+1] != '/' && buf[j+2] != 't' && input_size < 25) {
                input_size++;
                j++;
            }
            temp2 = (char*) malloc(input_size);
            memcpy(temp2, buf+(i+5), input_size*2);
            tlf = atoi(temp2);
            free(temp2);
        }

        /* name extraction */ 
        if (buf[i] == '<' && buf[i+1] == 'n' && buf[i+2] == 'a' && buf[i+3] == 'm' && buf[i+4] == 'e' && buf[i+5] == '>') {
            j = i+6;
            input_size = 0;
            while (buf[j] != '<' && buf[j+1] != '/' && buf[j+2] != 'n' && input_size < 25) {
                input_size++;
                j++;
            }

            name = (char*) malloc(input_size*2);
            
            if (0 < input_size) {
                memcpy(name, buf+(i+6), input_size);
            } else {
                flag = -1;
            }
        } i++;
    }

    sql_handle("/db/database.db", action, flag, id, tlf, name);
}

static int sql_handle (char* db_path, int action, int flag, int id, int tlf, char* name) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;

    sql = (char*) malloc(99);
    rc = sqlite3_open(db_path, &db);

    action = 1;
    if(rc == 0) {
        printf("Successfully connected to database\n");
    }
    /* Choose sql action */
    if (0 == action) {

    }

    else if (1 == action) {
        if (-1 == flag) {
            snprintf(sql, 99, "INSERT OR REPLACE INTO Phonebook(id, tlf) VALUES(%d, %d);", id, tlf);
        } else {
            snprintf(sql, 99, "INSERT OR REPLACE INTO Phonebook(id, tlf, name) VALUES(%d, %d, '%s');", id, tlf, name);
        }
    }

    else if (2 == action) {

    }

    /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if ( rc != SQLITE_OK ) {
      fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   } else {
      fprintf(stdout, "Inserted Successfully\n");
   }
   
   sqlite3_close(db);
   return 0;
}

static int callback(void *NotUsed, int argc, char **argv, char **azColName){
   int i;
   for(i=0; i<argc; i++){
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}