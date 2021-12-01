#include"../sqlite/sqlite3.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
typedef struct 
{
    char eng[200];
    char viet[6000];
}data_dic;

void filter_string(char *vie)
{
    char *p = strstr(vie, "\\n");
    while(p!=NULL){
        *p = '\n';
        *(p+1) = ' ';
        p = strstr(vie, "\\n");
    }
}



char *zErrMsg = 0;
static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
   int i;
   for(i = 0; i<argc; i++) {
      printf("%s = %s\n", azColName[i], argv[i] ? argv[i] : "NULL");
   }
   printf("\n");
   return 0;
}   


int inpData(sqlite3 *db, char *sql)
{
    int rc;
    sqlite3_stmt *stmt;
    if (sqlite3_prepare(db, sql, strlen(sql), &stmt, NULL) == SQLITE_OK)
    {
        sqlite3_step(stmt);
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        return 1;
    }
    else
    {
        printf("%s\n", sqlite3_errmsg(db));
        rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
        return 0;
    }
    
}
void restart(data_dic *tmp)
{
    memset(tmp->eng, 0, 100);
    memset(tmp->viet, 0, 7000);
}
int readFile_inpData(sqlite3 * db, char *sql)
{
    FILE * f;
    data_dic tmp;
    int stt = 0;
    restart(&tmp);
    if ((f = fopen("../input.txt", "r")) == NULL)
    {
        printf("ERROR OPENING FILE!!\n");
        return 0;
    }
    while(fscanf(f, "%[^\t]s", tmp.eng) != EOF && fscanf(f, "\t%[^\n]s", tmp.viet) != EOF)
    {
        fscanf(f, "\n");
       printf("%d\n", stt);
        sql = (char*)malloc(7000);
       memset(sql, 0, 7000);
        filter_string(tmp.eng);
        filter_string(tmp.viet);
        sql = sqlite3_mprintf("INSERT  INTO COMPANY(ID, ENGLISH, VIETNAM) \
        VALUES (%d, '%q', '%q');", ++stt, tmp.eng, tmp.viet);
        inpData(db, sql);
        sqlite3_free(sql);
        restart(&tmp);
    }
    fclose(f);
}
int main()
{
    sqlite3 *db;
    int rc;
    char *sql;
    data_dic tmp;
    rc = sqlite3_open("tudien.db", &db);
    if (rc)
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(db));
        return 0;
    }
    else
    {
        printf("Opend database successfully\n");
    }
    sql = "CREATE TABLE COMPANY("  \
      "ID INT PRIMARY KEY     NOT NULL," \
      "ENGLISH          CHAR(200) NOT NULL," \
       "VIETNAM         TEXT(5000) NOT NULL);";
   	rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   	inpData(db, sql);
    readFile_inpData(db, sql);
    sqlite3_close(db);
}
