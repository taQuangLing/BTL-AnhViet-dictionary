#include<gtk/gtk.h>
#include<string.h>
#include<stdio.h>
#include"sqlite/sqlite3.h"
#include"libfdr/jrb.h"
#include"pre_fix/prefix.h"
#include"pre_fix/llist.h"
#define LENGHT_ENGLISH 200
#define LENGHT_VIETNAM 5000
typedef struct anh_viet
{
    Jval id;
    char eng[LENGHT_ENGLISH];
    char viet[LENGHT_VIETNAM];
}avType;


typedef struct 
{
    GtkWidget * w_sbtn_sea;
    GtkWidget * w_btn_sea;
    GtkWidget * w_txt_main;
    GtkWidget * w_btn_edt;
    GtkWidget * w_btn_del;
    GtkWidget * w_btn_inf;
    GtkTextBuffer * w_txt_buf_main;


    GtkWidget * w_btn_add;
    GtkWidget * w_etr_eng;
    GtkWidget * w_txt_vie;
    GtkTextBuffer * w_txt_buf_vie;
    GtkWidget * w_btn_add_add;
    GtkWidget * w_btn_add_cancel;
    GtkWidget * w_dia_add;

    GtkWidget * w_sbtn_edit_sea;
    GtkWidget * w_btn_edit_sea;
    GtkWidget * w_btn_edit_ok;
    GtkWidget * w_btn_edit_cancel;
    GtkWidget * w_dia_edit;
    GtkWidget * w_txt_edit;
    GtkTextBuffer * w_txt_buf_edit;

    GtkWidget * w_sbtn_del_sea;
    GtkWidget * w_btn_del_sea;
    GtkWidget * w_btn_del_cancel;
    GtkWidget * w_btn_del_del;
    GtkWidget * w_dia_del;
    GtkWidget * w_lbl_dia_del;

    GtkWidget * w_mdia;
    GtkWidget * w_btn_mdia;
    GtkWidget * w_lbl_mdia;

    GtkWidget * w_mdia_edit;
    GtkWidget * w_btn_mdia_edit;
    GtkWidget * w_lbl_mdia_edit;

    GtkWidget * w_mdia_del;
    GtkWidget * w_btn_mdia_del;
    GtkWidget * w_lbl_mdia_del;

    GtkWidget * w_mdia_error;
    GtkWidget * w_btn_mdia_error;
    GtkWidget * w_lbl_mdia_error;
    
    GtkWidget * about_dia;

}appWidgets;

    sqlite3 *db;
    jrb_type *cur = NULL;
    jrb_type *root = NULL;
    JRB dic;
    int num_word;
    avType tmp;
    JRB p_sea_main = NULL;
    JRB p_edit_sea = NULL;
    JRB p_del_sea = NULL;

// Lam moi
void refresh(avType *tmp)
{
    memset(tmp->eng, 0, LENGHT_ENGLISH);
    memset(tmp->viet, 0, LENGHT_VIETNAM);
}
void refresh_entry(GtkWidget * p)
{
    gtk_entry_set_text(GTK_ENTRY(p), "");
}
void refresh_text(GtkTextBuffer * p)
{
    gtk_text_buffer_set_text(GTK_TEXT_BUFFER(p), "", -1);
}
void refresh_label(GtkWidget *lbl)
{
    gtk_label_set_text(GTK_LABEL(lbl), "");
}
//Thoat
void quit_mdia_add(appWidgets *widgts)
{
    refresh_entry(widgts->w_etr_eng);
    refresh_text(widgts->w_txt_buf_vie);
    gtk_widget_hide(widgts->w_mdia);
    gtk_widget_hide(widgts->w_dia_add);
}
void quit_dia_add(appWidgets *widgts)
{
    refresh_entry(widgts->w_etr_eng);
    refresh_text(widgts->w_txt_buf_vie);
    gtk_widget_hide(widgts->w_dia_add);
}
void quit_dia_edit(appWidgets *widgts)
{
    p_edit_sea = NULL;
    refresh_entry(widgts->w_sbtn_edit_sea);
    refresh_text(widgts->w_txt_buf_edit);
    gtk_widget_hide(widgts->w_dia_edit);
}
void quit_mdia_edit(appWidgets *widgts)
{
    p_edit_sea = NULL;
    refresh_entry(widgts->w_sbtn_edit_sea);
    refresh_text(widgts->w_txt_buf_edit);
    gtk_widget_hide(widgts->w_mdia_edit);
    gtk_widget_hide(widgts->w_dia_edit);
}
void quit_dia_del(appWidgets *widgts)
{
    p_del_sea = NULL;
    refresh_label(widgts->w_lbl_dia_del);
    refresh_entry(widgts->w_sbtn_del_sea);
    gtk_widget_hide(widgts->w_dia_del);
}
void quit_mdia_del(appWidgets * widgts)
{
    gtk_widget_hide(widgts->w_mdia_del);
    quit_dia_del(widgts);
}

// In ra thong tin tim kiem
void jrb_set_text(char *vie, int key)
{
    char viet[15000];
    sqlite3_stmt *stmt;
    if (key == 0){
        strcpy(vie, "\nRất tiếc, từ này hiện không có trong từ điển."
		                  "\n\nGợi ý:\t\t-Nhấn nút \"Thêm từ\", để bổ sung vào từ điển.");
        return;
    }
    sqlite3_prepare_v2(db, "SELECT ID, VIETNAM FROM COMPANY", -1, &stmt, 0);
    while(sqlite3_step(stmt) != SQLITE_DONE){
        tmp.id = new_jval_i(sqlite3_column_int(stmt, 0));
        if (jval_i(tmp.id) == key)
        {
            strcpy(viet, (char*)sqlite3_column_text(stmt, 1));
            strncpy(vie,viet, 4500);
            break;
        }
    }
    sqlite3_finalize(stmt);
}

void search_main(appWidgets *widgts)
{
    avType tmp;
    refresh(&tmp);
    strcpy(tmp.eng, gtk_entry_get_text(GTK_ENTRY(widgts->w_sbtn_sea)));
    p_sea_main = jrb_find_str(dic, tmp.eng);
    if (p_sea_main == NULL){
        jrb_set_text(tmp.viet, 0);
        gtk_text_buffer_set_text(widgts->w_txt_buf_main, tmp.viet, -1);
    }
    else
    {
        jrb_set_text(tmp.viet, jval_i(p_sea_main->val));
        gtk_text_buffer_set_text(widgts->w_txt_buf_main, tmp.viet, -1);
        
    }
}
int check_space(char ch)
{
    if (ch == ' ' || ch == '\n' || ch == '\t')return 1;
    return 0;
}
void dele_first_space(char *str)
{
    int i = 0;
    while(check_space(str[i]))i++;
    strcpy(str, str+i);
}
void dele_mid_space(char *str)
{
    int i = 0;
    while(i <= strlen(str)){
        if (check_space(str[i]) && check_space(str[i+1]))strcpy(str+i, str+i+1);
        else
            i++;
    }
}
void dele_last_space(char *str)
{
    int i = strlen(str);
    while(check_space(str[i]))i--;
    str[i+1] = '\0';
}
void dele_space(char *str)
{
    dele_first_space(str);
    dele_last_space(str);
    dele_mid_space(str);
}

int check_empty(char *str)
{
    int i, dem = 0;
    if (str[0] == '\0')return 0;
    for (i = 0; i < strlen(str); i++)
    if(str[i] == ' ' || str[i] =='\n' || str[i] == '\t');
    else{
        return 1;
    }
    return 0;
}
void btn_add_add(appWidgets * widgts)
{
    avType tmp;
    refresh(&tmp);
    GtkTextIter st_iter;
	GtkTextIter ed_iter;
    sqlite3_stmt * stmt;
    char *sql;
    strcpy(tmp.eng, gtk_entry_get_text(GTK_ENTRY(widgts->w_etr_eng)));

    gtk_text_buffer_get_start_iter (gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgts->w_txt_vie)), &st_iter);
	gtk_text_buffer_get_end_iter (gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgts->w_txt_vie)), &ed_iter);

    strcpy(tmp.viet, gtk_text_buffer_get_text(GTK_TEXT_BUFFER(widgts->w_txt_buf_vie), &st_iter, &ed_iter, FALSE));

    if (jrb_find_str(dic, tmp.eng))
    {
        gtk_widget_show(widgts->w_mdia_error);
        gtk_label_set_text(GTK_LABEL(widgts->w_lbl_mdia_error), "Đã có trong từ điển!");
    }
    else
    if (!check_empty(tmp.viet) || !check_empty(tmp.eng))
    {
        gtk_widget_show(widgts->w_mdia_error);
        gtk_label_set_text(GTK_LABEL(widgts->w_lbl_mdia_error), "Cảnh báo!\n Không được bỏ trống!");
    }
    else
    {
        dele_space(tmp.eng);
        dele_space(tmp.viet);
        tmp.id = new_jval_i(++num_word);
        list_insert(&cur, tmp.eng);

        jrb_insert_str(dic, cur->eng, tmp.id);

        sql = sqlite3_mprintf("INSERT  INTO COMPANY(ID, ENGLISH, VIETNAM) \
            VALUES (%d, '%q', '%q');", jval_i(tmp.id), tmp.eng, tmp.viet);
            
        if (sqlite3_prepare(db, sql, strlen(sql), &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_step(stmt);
            gtk_widget_show(widgts->w_mdia);
            gtk_label_set_text(GTK_LABEL(widgts->w_lbl_mdia), "Thêm thành công!");
        }
        else
        {
            gtk_widget_show(widgts->w_mdia);
            gtk_label_set_text(GTK_LABEL(widgts->w_lbl_mdia_error), "Thêm thất bại!");
        }
        sqlite3_finalize(stmt);
    }
}

void selection_word(appWidgets *widgts)
{
    avType tmp;
    refresh(&tmp);
    strcpy(tmp.eng, gtk_entry_get_text(GTK_ENTRY(widgts->w_sbtn_edit_sea)));
    p_edit_sea = jrb_find_str(dic, tmp.eng);
    if (p_edit_sea == NULL){
        jrb_set_text(tmp.viet, 0);
        gtk_text_buffer_set_text(widgts->w_txt_buf_edit, tmp.viet, strlen(tmp.viet));
    }
    else
    {
        jrb_set_text(tmp.viet, jval_i(p_edit_sea->val));
        gtk_text_buffer_set_text(widgts->w_txt_buf_edit, tmp.viet, strlen(tmp.viet));
    }
}

void edit_word(appWidgets *widgts)
{
    GtkTextIter st_iter;
	GtkTextIter ed_iter;
    char *sql;
    JRB p;
    jrb_type *p_list = root;
    sqlite3_stmt *stmt;
    avType tmp;
    refresh(&tmp);

    gtk_text_buffer_get_start_iter (gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgts->w_txt_edit)), &st_iter);
	gtk_text_buffer_get_end_iter (gtk_text_view_get_buffer(GTK_TEXT_VIEW(widgts->w_txt_edit)), &ed_iter);//Lay chi so cuoi buffer

    strcpy(tmp.eng, gtk_entry_get_text(GTK_ENTRY(widgts->w_sbtn_edit_sea)));
    strcpy(tmp.viet, gtk_text_buffer_get_text(GTK_TEXT_BUFFER(widgts->w_txt_buf_edit), &st_iter, &ed_iter, FALSE));
    p = jrb_find_str(dic, tmp.eng);
    if (p_edit_sea == NULL);
    else
    if (p == NULL || p == p_edit_sea){

        sql = sqlite3_mprintf("UPDATE COMPANY set ENGLISH = '%q', VIETNAM = '%q' where ID = %d;",
            tmp.eng, tmp.viet, jval_i(p_edit_sea->val));
        if (sqlite3_prepare(db, sql, strlen(sql), &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_step(stmt);
            gtk_widget_show(widgts->w_mdia_edit);
            gtk_label_set_text(GTK_LABEL(widgts->w_lbl_mdia_edit), "Đã sửa!");
        }
        else
        {
            gtk_widget_show(widgts->w_mdia_error);
            gtk_label_set_text(GTK_LABEL(widgts->w_lbl_mdia_error), "Xảy ra lỗi!\nHãy kiểm tra lại");
        }
        sqlite3_finalize(stmt);
    }
    else
    if (p != p_edit_sea)
    {
        gtk_widget_show(widgts->w_mdia_error);
        gtk_label_set_text(GTK_LABEL(widgts->w_lbl_mdia_error), "Đã có trong từ điển!");
    }
}
void del_selection(appWidgets * widgts)
{
    avType tmp;
    strcpy(tmp.eng, gtk_entry_get_text(GTK_ENTRY(widgts->w_sbtn_del_sea)));
    p_del_sea = jrb_find_str(dic, tmp.eng);
    if(p_del_sea == NULL)
    {
        gtk_label_set_text(GTK_LABEL(widgts->w_lbl_dia_del), "Không có trong từ điển.\nHãy chọn lại!");
    }
    else
    {
        gtk_label_set_text(GTK_LABEL(widgts->w_lbl_dia_del), "Hợp lệ!");
    }
}
void delete_word(appWidgets *widgts)
{
    char *sql;
     sqlite3_stmt *stmt;    
    if (p_del_sea == NULL);
    else
    {
        sql = sqlite3_mprintf("DELETE from COMPANY where ID = %d;",
            jval_i(p_del_sea->val));
        if (sqlite3_prepare(db, sql, strlen(sql), &stmt, NULL) == SQLITE_OK)
        {
            sqlite3_step(stmt);
            gtk_widget_show(widgts->w_mdia);
            gtk_label_set_text(GTK_LABEL(widgts->w_lbl_mdia), "Đã xóa!");
        }
        else
        {
            gtk_widget_show(widgts->w_mdia);
            gtk_label_set_text(GTK_LABEL(widgts->w_lbl_mdia), "Xảy ra lỗi!\nHãy kiểm tra lại");
        }
        sqlite3_finalize(stmt);
        jrb_delete_node(p_del_sea);
    }
    p_del_sea = NULL;
}
int main(int argc, char *argv[])
{
    GtkWidget *window;
    GtkBuilder * builder;
    JRB p;
    appWidgets		*app_widgets = g_slice_new(appWidgets);
    gtk_init(&argc, &argv);

    builder = gtk_builder_new();
    gtk_builder_add_from_file (builder, "../glade/window_main.glade", NULL);
    window = GTK_WIDGET(gtk_builder_get_object(builder, "window_main"));


    if(!(read_database(&db)) == 0);
    root = list_make();
    cur = root;
    num_word = input_jrb(db, &dic, &cur);

    app_widgets->w_sbtn_sea = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_sea"));
    app_widgets->w_btn_sea = GTK_WIDGET(gtk_builder_get_object(builder, "btn_sea"));
    app_widgets->w_btn_edt = GTK_WIDGET(gtk_builder_get_object(builder, "btn_edit"));
    app_widgets->w_btn_del = GTK_WIDGET(gtk_builder_get_object(builder, "btn_del"));
    app_widgets->w_btn_inf = GTK_WIDGET(gtk_builder_get_object(builder, "btn_inf"));
    app_widgets->w_txt_main = GTK_WIDGET(gtk_builder_get_object(builder, "txt_main"));
    app_widgets->w_txt_buf_main = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "txt_buf_main"));

    app_widgets->w_btn_add = GTK_WIDGET(gtk_builder_get_object(builder, "btn_add"));
    app_widgets->w_dia_add = GTK_WIDGET(gtk_builder_get_object(builder, "dia_add"));
    app_widgets->w_etr_eng = GTK_WIDGET(gtk_builder_get_object(builder, "etr_eng"));
    app_widgets->w_txt_vie = GTK_WIDGET(gtk_builder_get_object(builder, "txt_vie"));
    app_widgets->w_txt_buf_vie = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "txt_buf_add"));
    app_widgets->w_btn_add_add = GTK_WIDGET(gtk_builder_get_object(builder, "btn_add_add"));
    app_widgets->w_btn_add_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_add_cancel"));

    app_widgets->w_sbtn_edit_sea = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_edit_sea"));
    app_widgets->w_btn_edit_sea = GTK_WIDGET(gtk_builder_get_object(builder, "btn_edit_sea"));
    app_widgets->w_btn_edit_ok = GTK_WIDGET(gtk_builder_get_object(builder, "btn_edit_ok"));
    app_widgets->w_txt_edit = GTK_WIDGET(gtk_builder_get_object(builder, "txt_edit"));
    app_widgets->w_dia_edit = GTK_WIDGET(gtk_builder_get_object(builder, "dia_edit"));
    app_widgets->w_btn_edit_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "dia_edit_cancel"));
    app_widgets->w_txt_buf_edit = GTK_TEXT_BUFFER(gtk_builder_get_object(builder, "txt_buf_edit"));

    app_widgets->w_sbtn_del_sea = GTK_WIDGET(gtk_builder_get_object(builder, "sbtn_del_sea"));
    app_widgets->w_btn_del_sea = GTK_WIDGET(gtk_builder_get_object(builder, "btn_del_sea"));
    app_widgets->w_btn_del_cancel = GTK_WIDGET(gtk_builder_get_object(builder, "btn_del_cancel"));
    app_widgets->w_btn_del_del = GTK_WIDGET(gtk_builder_get_object(builder, "btn_del"));
    app_widgets->w_dia_del = GTK_WIDGET(gtk_builder_get_object(builder, "dia_del"));
    app_widgets->w_lbl_dia_del = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_dia_del"));


    app_widgets->w_mdia = GTK_WIDGET(gtk_builder_get_object(builder, "mdia"));
    app_widgets->w_btn_mdia = GTK_WIDGET(gtk_builder_get_object(builder, "btn_mdia"));
    app_widgets->w_lbl_mdia = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_mdia"));
    
    app_widgets->w_mdia_edit = GTK_WIDGET(gtk_builder_get_object(builder, "mdia_edit"));
    app_widgets->w_btn_mdia_edit = GTK_WIDGET(gtk_builder_get_object(builder, "btn_mdia_edit"));
    app_widgets->w_lbl_mdia_edit = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_mdia_edit"));

    app_widgets->w_mdia_del = GTK_WIDGET(gtk_builder_get_object(builder, "mdia_del"));
    app_widgets->w_btn_mdia_del = GTK_WIDGET(gtk_builder_get_object(builder, "btn_mdia_del"));
    app_widgets->w_lbl_mdia_del = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_mdia_del"));


    app_widgets->w_mdia_error = GTK_WIDGET(gtk_builder_get_object(builder, "mdia_error"));
    app_widgets->w_btn_mdia_error = GTK_WIDGET(gtk_builder_get_object(builder, "btn_mdia_error"));
    app_widgets->w_lbl_mdia_error = GTK_WIDGET(gtk_builder_get_object(builder, "lbl_mdia_error"));

    app_widgets->about_dia = GTK_WIDGET(gtk_builder_get_object(builder, "about_dia"));

    gtk_builder_connect_signals(builder, app_widgets);
   
    g_object_unref(builder);
 
    gtk_widget_show(window);                
    gtk_main();
    g_slice_free(appWidgets, app_widgets);
 
    return 0;
}

// Tim kiem o man hinh chinh
void on_btn_sea_clicked(GtkButton *button, appWidgets *widgts)
{
    search_main(widgts);
}
void on_sbtn_sea_activate(GtkEntry * entry, appWidgets* widgts)
{
    search_main(widgts);
}

// Chuc nang them mot tu vao tu dien
void on_btn_add_clicked(GtkButton * btn_add, GtkWidget * dia_add)
{
    gtk_widget_show(dia_add);
}
void on_btn_add_add_clicked(GtkButton *button, appWidgets * widgts)
{
    btn_add_add(widgts);
}
void on_btn_add_add_activate(GtkButton *button, appWidgets *widgts)
{
    btn_add_add(widgts);
}
void on_dia_add_response(GtkDialog *dialog, gint response_id, appWidgets * widgts)
{
    quit_dia_add(widgts);
}
void on_btn_mdia_clicked(GtkButton *button, appWidgets *widgts)
{
    quit_mdia_add(widgts);
}   
void on_btn_mdia_activate(GtkButton *button, appWidgets *widgts)
{
    quit_mdia_add(widgts);
}
void on_btn_mdia_error_clicked(GtkButton *button, appWidgets *widgts)
{
    gtk_widget_hide(widgts->w_mdia_error);
}
void on_btn_mdia_error_activate(GtkButton *button, appWidgets *widgts)
{
    gtk_widget_hide(widgts->w_mdia_error);
}


//Chuc nang sua tu trong tu dien
void on_btn_edit_clicked(GtkButton *button, GtkWidget * dia_edit)
{
    gtk_widget_show(dia_edit);
}
void on_btn_edit_sea_clicked(GtkButton * button, appWidgets * widgts)
{
    selection_word(widgts);
}
void on_sbtn_edit_sea_activate(GtkButton *button, appWidgets *widgts)
{
    selection_word(widgts);
}
void on_btn_edit_ok_clicked(GtkButton * button, appWidgets * widgts)
{
    edit_word(widgts);
}
void on_dia_edit_response(GtkDialog *dialog, gint response_id, appWidgets *widgts)
{
    quit_dia_edit(widgts);
}
void on_btn_mdia_edit_clicked(GtkButton *button, appWidgets *widgts)
{
    quit_mdia_edit(widgts);
}
void on_btn_mdia_edit_activate(GtkButton *button, appWidgets *widgts)
{
    quit_mdia_edit(widgts);
}



void on_btn_del_clicked(GtkButton * button, appWidgets * widgts)
{
    gtk_widget_show(widgts->w_dia_del);
}
void on_btn_del_sel_clicked(GtkButton * button, appWidgets *widgts)
{
    del_selection(widgts);
}
void on_sbtn_del_sea_activate(GtkButton * button, appWidgets *widgts)
{
    del_selection(widgts);
}       
void on_btn_del_del_clicked(GtkWidget * button, appWidgets *widgts)
{
    delete_word(widgts);
}
void on_dia_del_response(GtkDialog *dialog, gint response_id, appWidgets *widgts)
{
    quit_dia_del(widgts);
}
void on_btn_mdia_del_clicked(GtkButton *button, appWidgets *widgts)
{
    quit_mdia_del(widgts);
}
void on_btn_mdia_del_activate(GtkButton *button, appWidgets *widgts)
{
    quit_mdia_del(widgts);
}





void on_btn_inf_clicked(GtkButton *button, appWidgets *widgts)
{
    gtk_widget_show(widgts->about_dia);
}




void on_window_main_destroy()
{
    list_free(&root);
    jrb_free_tree(dic);
    sqlite3_close(db);
    gtk_main_quit();
}
