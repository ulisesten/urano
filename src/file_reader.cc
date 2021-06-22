#include "file_reader.h"

GtkWidget* create_notebook(GtkWidget** header) {

    GtkWidget* notebook = gtk_notebook_new();

    *header = create_window_controls();
    
    //gtk_notebook_set_scrollable ((GtkNotebook *)notebook, true);
    //gtk_notebook_set_action_widget ((GtkNotebook *)notebook, *header, GTK_PACK_END);
    set_notebook(notebook, (char*)"welcome.md", (char*)"../assets/pages/");

    return notebook;

}

GtkWidget* create_window_controls() {
    
    //GtkWidget *box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);
    GtkWidget *bar = gtk_header_bar_new ();
    gtk_header_bar_set_show_close_button ((GtkHeaderBar *)bar, true);
    gtk_header_bar_set_has_subtitle((GtkHeaderBar *)bar, false);

    //gtk_container_add((GtkContainer*)box, bar);

    gtk_widget_show(bar);
    //gtk_widget_show(box);
    return bar;
    
}

void set_notebook(GtkWidget* notebook, char* file_name, char* path_name) {
    GtkWidget*       scrolled;
    GtkWidget*       source_view;
    GtkWidget*       title;
    GtkSourceBuffer* buffer;
    GtkWidget*       tab;
    gchar*           basename;
    gchar*           contents;
    gsize            length;
    GFile*           file;
    GString*         path;
    GError*          gErr = NULL;
    char             location[1024];
    bool             contents_loaded = false;

    strcpy(location, path_name);
    strcat(location,"/");
    strcat(location,file_name);

    file = g_file_new_for_path(location);
    
    if(file){

        basename = g_path_get_basename(file_name);
        scrolled = gtk_scrolled_window_new(NULL,NULL);
        gtk_widget_show(scrolled);
        gtk_widget_set_hexpand(scrolled,TRUE);
        gtk_widget_set_vexpand(scrolled,TRUE);

        contents_loaded = g_file_load_contents (file, NULL, &contents, &length, NULL, &gErr);
        if (gErr) {
            printf ("set_notebook(): Could not load content: %s\n", gErr->message);
            g_error_free(gErr);
            gErr = NULL;
        }

        else if(contents_loaded) {

            buffer = gtk_source_buffer_new (NULL);
            gtk_text_buffer_set_text ((GtkTextBuffer*)buffer, contents, length);
            source_view = gtk_source_view_new_with_buffer(buffer);
            gtk_widget_show(source_view);

            setting_buffer(buffer, source_view, location);

            path = g_string_new(location);

            gtk_container_add(GTK_CONTAINER(scrolled), source_view);
            title = create_tab(basename, (GtkNotebook*)notebook, scrolled);

            g_signal_connect(source_view, "key-press-event",   G_CALLBACK(on_key_press),        path  );
            g_signal_connect(buffer,      "changed",           G_CALLBACK(on_modifying_file),   title   );
            g_signal_connect(buffer,      "modified-changed",  G_CALLBACK(on_file_saved),       title   );
            //g_signal_connect(buffer,      "begin-user-action", G_CALLBACK(save_file),           path    );
            
            g_free (contents);
            
        }
        
        g_free (basename);

    } else {
        printf("set_notebook(): error opening file\n");
    }

}

gboolean on_key_press(GtkWidget* widget, GdkEventKey *event, gpointer user_data){

    GtkTextBuffer* buffer = gtk_text_view_get_buffer ((GtkTextView *)widget);    
    
    switch (event->keyval) {

        case GDK_KEY_s:

            if (event->state & GDK_CONTROL_MASK) {

                save_file(buffer, user_data); 
                //gtk_text_buffer_end_user_action((GtkTextBuffer*)user_data);   

            } 

        break;

    }

    return FALSE;
}

void on_modifying_file (GtkTextBuffer *buffer, gpointer data) {
    
    const char* name = gtk_label_get_text((GtkLabel*)data);
    char new_name[100] = "*";
    if(name[0] != '*'){
        strcat(new_name, name );
        gtk_label_set_text((GtkLabel*)data,new_name);
    }

}

void on_file_saved (GtkTextBuffer *textbuffer, gpointer user_data) {

    const char* name = gtk_label_get_text((GtkLabel*)user_data);
    if(name[0] == '*'){
        *name++;
        gtk_label_set_text((GtkLabel*)user_data, name);
    }

}


void on_save_button_clicked(GtkWidget* widget, gpointer user_data) {
    //gtk_text_buffer_end_user_action((GtkTextBuffer*)user_data);
    printf("saving\n");
}


void save_file(GtkTextBuffer* buffer, gpointer user_data) {
    GtkTextIter start, end;
    GError* gerr = NULL;
    bool res;

    GString* text = (GString*)user_data;

    gtk_text_buffer_get_start_iter (buffer, &start);
    gtk_text_buffer_get_end_iter (buffer, &end);

    char* content = gtk_text_buffer_get_text (buffer, &start, &end, false);
    gtk_text_buffer_set_modified (buffer, false);
    res = g_file_set_contents( text->str, content, -1, &gerr);
    if (gerr) {
        g_critical ("Could not save file: %s\n", gerr->message);
        g_error_free(gerr);
        gerr = NULL;
    }
}


GtkWidget* create_tab(const gchar* title, GtkNotebook* notebook, GtkWidget* scrolled) {

    GtkWidget* box, *title_label, *close_label;

    close_label = gtk_button_new_from_icon_name ("window-close-symbolic", GTK_ICON_SIZE_MENU);
    title_label = gtk_label_new(title);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 0);

    gtk_widget_show(close_label);
    gtk_widget_show(title_label);

    gtk_container_add((GtkContainer*)box, title_label);
    gtk_container_add((GtkContainer*)box, close_label);

    gint pos = gtk_notebook_append_page (GTK_NOTEBOOK(notebook), scrolled, box);
    if(pos < 0){ g_printf("notebook fails.\n"); }

    gtk_notebook_set_current_page ((GtkNotebook *)notebook, pos);
    g_signal_connect(close_label, "clicked", G_CALLBACK(close_tab), scrolled );
    gtk_widget_show(box);

    return title_label;

}

void close_tab (GtkButton *button, gpointer notebook_child) {

    GtkWidget* notebook = gtk_widget_get_parent ((GtkWidget*)notebook_child);
    int num = gtk_notebook_page_num ((GtkNotebook*)notebook, (GtkWidget*)notebook_child);
    gtk_notebook_remove_page ((GtkNotebook*)notebook, num);

}

void setting_buffer(GtkSourceBuffer* buffer, GtkWidget* source_view, char* location) {
    GtkSourceLanguage*             lang             = NULL;
    GtkSourceLanguageManager*      manager          = gtk_source_language_manager_new ();
    GtkSourceStyleSchemeManager*   styleSchemeMng   = gtk_source_style_scheme_manager_new ();
    gchar*                         dirs[]           = {(gchar*)"../assets/styles/", NULL};
    gboolean                       result_uncertain;
    gchar*                         content_type;

    content_type = g_content_type_guess ((char*)location, NULL, 0, &result_uncertain);
    if (result_uncertain) {
        g_free (content_type);
        content_type = NULL;
    }

    lang = gtk_source_language_manager_guess_language (manager, (char*)location, content_type);
    gtk_source_buffer_set_language (buffer, lang);

    gtk_source_style_scheme_manager_set_search_path( styleSchemeMng ,  dirs);
    gtk_source_buffer_set_style_scheme (buffer, gtk_source_style_scheme_manager_get_scheme(styleSchemeMng, "oblivion2"));
    
    gtk_text_view_set_monospace (                 (GtkTextView*)  source_view, true);
    gtk_source_view_set_show_line_numbers (       (GtkSourceView*)source_view, true);
    gtk_source_view_set_highlight_current_line (  (GtkSourceView*)source_view, true);
    gtk_source_view_set_show_line_marks (         (GtkSourceView*)source_view, true);
    gtk_source_view_set_smart_backspace (         (GtkSourceView*)source_view, true);
    gtk_source_view_set_smart_home_end (          (GtkSourceView*)source_view, GTK_SOURCE_SMART_HOME_END_BEFORE);

    g_free(content_type);

}