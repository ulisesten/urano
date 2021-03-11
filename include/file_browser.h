#ifndef __FILE_BROWSER__
#define __FILE_BROWSER__

#include <stdio.h>
#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <glib.h>
#include "file_reader.h"
#include "utils.h"

typedef struct {
    GtkTreeView            * tree_view = NULL;
    GtkListStore           * list_store;
    GtkTreeStore           * tree_store;
    GtkTreeViewColumn      * column    = NULL;
    GtkCellRenderer        * renderer;
    GtkTreeSelection       * selection;
    GdkPixbuf              * pixbuf_folder;
    GdkPixbuf              * pixbuf_text_file;
} File_browser;

enum {
  COLUMN_PIXBUF,
  COLUMN_STRING,
  COLUMN_PATH
};

GtkTreeSelection*
     create_file_explorer(   GtkTreeView*        tree_view, char* working_dir);
void setup_file_explorer(    GtkTreeViewColumn*  column,      File_browser fb, char* working_dir);
void initialize_tree_store(  File_browser*       fb,          GtkTreeIter* iter, char* project_name);
void fill_tree_store(        PATH                paths,       GtkTreeIter* iter, GtkTreeIter* parent, File_browser fb, int indent);
void set_tree_view(          File_browser        fb);
gboolean
     attach_notebook_to_selection (   GtkTreeSelection   *selection,
                             GtkTreeModel       *model,
                             GtkTreePath        *path,
                             gboolean            path_currently_selected,
                             gpointer            userdata );


#endif
