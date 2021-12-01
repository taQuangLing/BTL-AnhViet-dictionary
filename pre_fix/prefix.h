#ifndef _PREFIX_
#define _PREFIX_

#include"../libfdr/jrb.h"
#include"../sqlite/sqlite3.h"
#include"llist.h"

int read_database(sqlite3 **db);
int input_jrb(sqlite3 *db, JRB *dic, jrb_type **cur);
#endif
