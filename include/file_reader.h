#ifndef __FILE_READER__
#define __FILE_READER__

#include <gtk/gtk.h>
#include <glib/gprintf.h>


void open_file( GtkWidget          *stack,            char             *file_name);
void set_notebook(GtkWidget* notebook, char* filename, gint page_position);
GtkWidget* create_tab(const gchar* title);

#endif