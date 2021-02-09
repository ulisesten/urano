#ifndef __FILE_EXPLORER__
#define __FILE_EXPLORER__

#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <glib.h>
#include "utils.h"

typedef struct {
    GtkTreeView            * tree_view = NULL;
    GtkListStore           * list_store;
    GtkTreeStore           * tree_store;
    GtkTreeViewColumn      * column0;
    GtkTreeViewColumn      * column1;
    GtkCellRenderer        * renderer;
    GtkTreeSelection       * selection;
    GdkPixbuf              * pixbuf_folder;
    GdkPixbuf              * pixbuf_text_file;
} file_browser;

enum {
  COLUMN_PIXBUF,
  COLUMN_STRING
};

GtkTreeSelection*
     create_file_explorer(   GtkBuilder         *builder);
void setup_file_explorer(    GtkTreeViewColumn  *column,      file_browser fBrowser);
void initialize_tree_store(  file_browser* fb, GtkTreeIter * iter);
void fill_tree_store(        PATH paths,    GtkTreeIter* iter, GtkTreeIter* parent, file_browser fb, int indent);
void set_tree_view(          file_browser        f_browser);
gboolean
     view_selection_func (   GtkTreeSelection   *selection,
                             GtkTreeModel       *model,
                             GtkTreePath        *path,
                             gboolean            path_currently_selected,
                             gpointer            userdata );


#endif
