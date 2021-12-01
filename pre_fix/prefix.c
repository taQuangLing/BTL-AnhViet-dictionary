#include"llist.h"
#include"prefix.h"
#include"../sqlite/sqlite3.h"
#include"../libfdr/jrb.h"
#include<stdio.h>
#include<string.h>
int read_database(sqlite3 **db)
{
    int rc;
    rc = sqlite3_open("../test.db", db);
    if (rc)
    {
        printf("Can't open database: %s\n", sqlite3_errmsg(*db));
        return 0;
    }
    else
    {
        printf("Opend database successfully\n");
        return 1;
    }
}
int  input_jrb(sqlite3 *db, JRB *dic, jrb_type **cur)
{
    sqlite3_stmt *stmt;
    Jval jval;
    const unsigned char *eng;
    const unsigned char *vie;
    *dic = make_jrb();
    sqlite3_prepare_v2(db, "SELECT ID, ENGLISH, VIETNAM FROM COMPANY", -1, &stmt, 0);
    while(sqlite3_step(stmt) != SQLITE_DONE)
    {
            jval.i = sqlite3_column_int(stmt, 0);
            eng = sqlite3_column_text(stmt, 1);
            list_insert(cur, (char*)eng);
            jrb_insert_str(*dic, (*cur)->eng, jval);
    }
	sqlite3_finalize(stmt);
    return jval_i(jval);
}
