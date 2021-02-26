

#include "file_reader.h"
#include <gtk/gtk.h>


/**page position unused*/
void set_notebook(GtkWidget* notebook, char* file_name, char* path_name, gint page_position){

    GtkWidget* scrolled;
    GtkWidget* view;
    GtkWidget* tab;
    gchar*     basename;
    gchar*     contents;
    gsize      length;
    GFile*     file;
    GError*    gErr = NULL;
    bool       contents_loaded = false;
    char       location[1024];
    
    //printf("%s\n", path_name);
    //printf("%s\n", file_name);

    strcpy(location, path_name);
    strcat(location,file_name);

    file = g_file_new_for_path(location);
    if(file){
        basename = g_path_get_basename(file_name);

        scrolled = gtk_scrolled_window_new(NULL,NULL);
        gtk_widget_show(scrolled);
        gtk_widget_set_hexpand(scrolled,TRUE);
        gtk_widget_set_vexpand(scrolled,TRUE);

        view = gtk_text_view_new();
        gtk_text_view_set_editable(GTK_TEXT_VIEW(view), true);
        gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), true);
        gtk_widget_show(view);

        gtk_container_add(GTK_CONTAINER(scrolled), view);

        int is_added = gtk_notebook_page_num((GtkNotebook*)notebook, scrolled);

        contents_loaded = g_file_load_contents (file, NULL, &contents, &length, NULL, &gErr);
        if (gErr) {
            printf ("set_notebook(): Could not load contents: %s\n", gErr->message);
            g_error_free(gErr);
            gErr = NULL;
        }

        else if(contents_loaded) {
            
            tab = create_tab(basename);
            gint res = gtk_notebook_append_page (GTK_NOTEBOOK(notebook), scrolled, tab);
            if(res < 0){
                g_printf("notebook fails.\n");
            }

            gtk_notebook_set_current_page ((GtkNotebook *)notebook, is_added);

            GtkTextBuffer *buffer;

            buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
            gtk_text_buffer_set_text (buffer, contents, length);
            g_free (contents);

        }
        
        g_free (basename);

    } else {
        printf("set_notebook(): error opening file\n");
    }

}

GtkWidget* create_tab(const gchar* title){
    GtkWidget* box, *title_label, *close_label;

    close_label = gtk_button_new_from_icon_name ("window-close-symbolic", GTK_ICON_SIZE_MENU);
    title_label = gtk_label_new(title);
    box = gtk_box_new(GTK_ORIENTATION_HORIZONTAL, 1);

    gtk_widget_show(close_label);
    gtk_widget_show(title_label);

    g_signal_connect(close_label, "clicked", G_CALLBACK(close_tab), NULL );

    gtk_container_add((GtkContainer*)box, title_label);
    gtk_container_add((GtkContainer*)box, close_label);

    gtk_widget_show(box);

    return box;
}

void close_tab (GtkButton *button, gpointer   user_data) {

    printf("to close tab\n");

}