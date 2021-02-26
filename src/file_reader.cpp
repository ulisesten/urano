

#include "file_reader.h"
//#include <gtk/gtk.h>


/**page position unused*/
void set_notebook(GtkWidget* notebook, char* file_name, char* path_name, gint page_position) {
    GtkWidget* scrolled;
    GtkWidget* source_view;
    GtkSourceBuffer *buffer;
    GtkWidget* tab;
    NotebookTab notebook_tab;
    gchar*     basename;
    gchar*     contents;
    gsize      length;
    GFile*     file;
    GError*    gErr = NULL;
    bool       contents_loaded = false;
    char       location[1024];

    strcpy(location, path_name);
    strcat(location,file_name);

    file = g_file_new_for_path(location);
    if(file){
        basename = g_path_get_basename(file_name);

        scrolled = gtk_scrolled_window_new(NULL,NULL);
        gtk_widget_show(scrolled);
        gtk_widget_set_hexpand(scrolled,TRUE);
        gtk_widget_set_vexpand(scrolled,TRUE);

        int is_added = gtk_notebook_page_num((GtkNotebook*)notebook, scrolled);

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

            gtk_container_add(GTK_CONTAINER(scrolled), source_view);
            
            //notebook_tab->notebook = (GtkNotebook*)notebook;
            tab = create_tab(basename, (GtkNotebook*)notebook);
            gint res = gtk_notebook_append_page (GTK_NOTEBOOK(notebook), scrolled, tab);
            if(res < 0){
                g_printf("notebook fails.\n");
            }

            gtk_notebook_set_current_page ((GtkNotebook *)notebook, is_added);

            g_free (contents);
            
        }
        
        g_free (basename);

    } else {
        printf("set_notebook(): error opening file\n");
    }

}

void setting_buffer(GtkSourceBuffer* buffer, GtkWidget* source_view, char* location) {
    GtkSourceLanguage *lang = NULL;
    GtkSourceLanguageManager* manager = gtk_source_language_manager_new ();
    GtkSourceStyleSchemeManager * styleSchemeMng = gtk_source_style_scheme_manager_new ();
    gchar* dirs[] = {(gchar*)"../assets/styles/", NULL};

    gboolean result_uncertain;
    gchar *content_type;

    content_type = g_content_type_guess (location, NULL, 0, &result_uncertain);
    if (result_uncertain)
    {
        g_free (content_type);
        content_type = NULL;
    }

    lang = gtk_source_language_manager_guess_language (manager, location, content_type);
    gtk_source_buffer_set_language (buffer, lang);

    gtk_source_style_scheme_manager_set_search_path( styleSchemeMng ,  dirs);
    gtk_source_buffer_set_style_scheme (buffer, gtk_source_style_scheme_manager_get_scheme(styleSchemeMng, "oblivion2"));
    gtk_source_view_set_show_line_marks ((GtkSourceView *)source_view,TRUE);
    
    gtk_source_view_set_show_line_numbers ((GtkSourceView*)source_view, TRUE);
    gtk_source_view_set_highlight_current_line ((GtkSourceView*)source_view, TRUE);

    g_free(content_type);

}

GtkWidget* create_tab(const gchar* title, GtkNotebook* notebook){
    GtkWidget* box, *title_label, *close_label;

    close_label = gtk_button_new_from_icon_name ("window-close-symbolic", GTK_ICON_SIZE_MENU);
    title_label = gtk_label_new(title);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

    gtk_widget_show(close_label);
    gtk_widget_show(title_label);

    g_signal_connect(close_label, "clicked", G_CALLBACK(close_tab), notebook );

    gtk_container_add((GtkContainer*)box, title_label);
    gtk_container_add((GtkContainer*)box, close_label);

    gtk_widget_show(box);

    return box;
}

void close_tab (GtkButton *button, gpointer   user_data) {

    GtkWidget* parent = gtk_widget_get_parent((GtkWidget*)button);
    int num = gtk_notebook_page_num ((GtkNotebook*)user_data, parent);
    gtk_notebook_remove_page ((GtkNotebook*)user_data, num);

}