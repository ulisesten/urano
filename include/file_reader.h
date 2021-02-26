#ifndef __FILE_READER__
#define __FILE_READER__

#include <gtk/gtk.h>
#include <glib/gprintf.h>

void set_notebook(GtkWidget* notebook, char* filename, char* path, gint page_position);
GtkWidget* create_tab(const gchar* title);
void close_tab (GtkButton *button, gpointer   user_data);

#endif