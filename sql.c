#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sqlite3.h>

// Handles xml parsing and sql actions for server.c via handle.c that calls it
// Andreas Vestgarden Olsen (March 2017)

void xml_parse(char* buf, int action);
static int sql_handle (char* db_path, int action, int flag, int id, int tlf, char* name);
static int callback(void *NotUsed, int argc, char **argv, char **azColName);
static int select_callback(void *NotUsed, int argc, char **argv, char **azColName);

void xml_parse(char* buf, int action) {

    int i, j, k, input_size, id, tlf;
    int flag = 0;
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
            while (buf[j] != '<' && buf[j+1] != '/' && buf[j+2] != 'i') {
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
            while (buf[j] != '<' && buf[j+1] != '/' && buf[j+2] != 't') {
                input_size++;
                j++;
            }
            temp2 = (char*) malloc(input_size);
            memcpy(temp2, buf+(i+5), input_size*2);
            tlf = atoi(temp2);
            free(temp2);
        }

        // name extraction
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
    free(name);
}

static int sql_handle (char* db_path, int action, int flag, int id, int tlf, char* name) {
    sqlite3* db;
    char* zErrMsg = 0;
    int rc;
    char* sql;
    char* actionType = malloc(15);

    sql = (char*) malloc(99);
    rc = sqlite3_open(db_path, &db);

    if(rc != 0) {
        printf("sql.c error: Could not connect to database!\n");
        exit(1);
    }

    /* Choose sql action */
    if (0 == action) {
        if (-1 == id) {
            snprintf(sql, 99, "SELECT * FROM Phonebook;");
        } else {
            snprintf(sql, 99, "SELECT * FROM Phonebook WHERE id=%d", id);
        }
        actionType = "search";
    }

    else if (1 == action) {
        if (-1 == flag) {
            snprintf(sql, 99, "INSERT INTO Phonebook(id, tlf) VALUES(%d, %d);", id, tlf);
        } else {
            snprintf(sql, 99, "INSERT INTO Phonebook(id, tlf, name) VALUES(%d, %d, '%s');", id, tlf, name);
        }

        actionType = "insert";
    }

    else if (2 == action) {
        if (-1 == flag) {
            snprintf(sql, 99, "UPDATE Phonebook SET tlf=%d, name='' WHERE id=%d;", tlf, id);
        } else {
            snprintf(sql, 99, "UPDATE Phonebook SET tlf=%d, name='%s' WHERE id=%d;", tlf, name, id);
        }

        actionType = "update";
    }

    else if (3 == action) {
        if (-1 == id) {
            snprintf(sql, 99, "DELETE FROM Phonebook;");
        } else {
            snprintf(sql, 99, "DELETE FROM Phonebook WHERE id=%d", id);
        }

        actionType = "delete";
    }

    /* Execute SQL statement */


    if (0 == action) {
        printf("HTTP/1.1 200 OK\n");
        printf("Content-Type: application/xml\n\n");
        printf("<phonebook>");
        rc = sqlite3_exec(db, sql, select_callback, 0, &zErrMsg);
        printf("</phonebook>");
    } else {
        printf("HTTP/1.1 200 OK\n");
        printf("Content-Type: text/plain\n\n");
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
    }

    if ( rc != SQLITE_OK ) {
       fprintf(stdout, "SQL error: %s\n", zErrMsg);
       sqlite3_free(zErrMsg);
    } 
    
    else if (0 != action) {
       fprintf(stdout, "Successful %s\n", actionType);
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

static int select_callback(void *NotUsed, int argc, char **argv, char **azColName){
   
   //printf("<phonebook>");
   
   int i;
   int person_count = 0;
   for(i=0; i<argc; i++) {

       if (0 == person_count) {
           printf("<person>");
           printf("<id>%s</id>", argv[i] ? argv[i] : "");
       }

       else if (1 == person_count) {
           printf("<tlf>%s</tlf>", argv[i] ? argv[i] : "");
       }

       else if(2 == person_count) {
           printf("<name>%s</name>", argv[i] ? argv[i] : "");
       }

       if (2 == person_count) {
           printf("</person>");
           person_count = 0;
       } else {
           person_count++;
       }
   }
   return 0;
}

