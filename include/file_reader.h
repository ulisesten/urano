#ifndef __FILE_READER__
#define __FILE_READER__

#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <gtksourceview/gtksource.h>

typedef struct {
    GtkNotebook* notebook;
    GtkWidget* label;
} *NotebookTab;

void set_notebook(GtkWidget* notebook, char* filename, char* path, gint page_position);
GtkWidget* create_tab(const gchar* title, GtkNotebook* page);
void close_tab (GtkButton *button, gpointer   user_data);
void setting_buffer(GtkSourceBuffer* buffer, GtkWidget* source_view, char* location);
void on_buffer_change (GtkTextBuffer *textbuffer, gpointer user_data);

#endif