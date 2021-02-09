

#include "file_reader.h"
#include <gtk/gtk.h>


void open_file(GtkWidget* stack, char* file_name){

    GtkWidget* scrolled;
    GtkWidget* view;
    gchar*     basename;
    gchar*     contents;
    gsize      length;
    GFile*     file;

    file = g_file_new_for_path(file_name);
    basename = g_file_get_basename(file);

    scrolled = gtk_scrolled_window_new(NULL,NULL);
    gtk_widget_show(scrolled);
    gtk_widget_set_hexpand(scrolled,TRUE);
    gtk_widget_set_vexpand(scrolled,TRUE);

    view = gtk_text_view_new();

    gtk_text_view_set_editable(GTK_TEXT_VIEW(view), true);
    gtk_text_view_set_cursor_visible(GTK_TEXT_VIEW(view), true);
    gtk_widget_show(view);

    gtk_container_add(GTK_CONTAINER(scrolled), view);
    gtk_stack_add_titled(GTK_STACK(stack),scrolled, basename, basename);

    if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL))
    {
      GtkTextBuffer *buffer;

      buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
      gtk_text_buffer_set_text (buffer, contents, length);
      g_free (contents);
    }

    g_free (basename);

}

/**page position unused*/
void set_notebook(GtkWidget* notebook, char* file_name, gint page_position){

    GtkWidget* scrolled;
    GtkWidget* view;
    GtkWidget* label;
    gchar*     basename;
    gchar*     contents;
    gsize      length;
    GFile*     file;
    

    file = g_file_new_for_path(file_name);
    basename = g_file_get_basename(file);

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

    if(is_added < 0)
    {
        
        label = gtk_label_new(basename);
        gint res = gtk_notebook_append_page (GTK_NOTEBOOK(notebook), scrolled, label);
        if(res < 0){
            g_printf("notebook fails.\n");
        }

        gtk_notebook_set_current_page ((GtkNotebook *)notebook, is_added);

        if (g_file_load_contents (file, NULL, &contents, &length, NULL, NULL))
        {
          GtkTextBuffer *buffer;

          buffer = gtk_text_view_get_buffer (GTK_TEXT_VIEW (view));
          gtk_text_buffer_set_text (buffer, contents, length);
          g_free (contents);

        }
    }
    
    g_free (basename);

}