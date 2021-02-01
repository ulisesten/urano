#ifndef __FILE_EXPLORER__
#define __FILE_EXPLORER__

#include <gtk/gtk.h>
#include <glib/gprintf.h>
#include <glib.h>
#include "utils.h"

typedef struct {
    GtkTreeView            * tree_view = NULL;
    GtkListStore           * list_store;
    GtkTreeViewColumn      * column0;
    GtkTreeViewColumn      * column1;
    GtkCellRenderer        * renderer;
    GtkTreeSelection       * selection;
    GdkPixbuf              * pixbuf;
    GdkPixbuf              * pixbuf_text;
} file_browser;

typedef struct
{
  gchar* dir;
  gint type;
  
}PATH;


enum {
  COLUMN_PIXBUF,
  COLUMN_STRING
};

enum {
  FILE_TYPE,
  FOLDER_TYPE
};

GtkTreeSelection*
     create_file_explorer(   GtkBuilder         *builder);
void setup_file_explorer(    GtkTreeViewColumn  *column,      file_browser fBrowser);
void walk_dir(               GDir*               contents,    GtkTreeIter* iter, file_browser fb);
void set_tree_view(          file_browser        f_browser);
gboolean
     view_selection_func (   GtkTreeSelection   *selection,
                             GtkTreeModel       *model,
                             GtkTreePath        *path,
                             gboolean            path_currently_selected,
                             gpointer            userdata );
gchar* get_project_folder_name(gchar* path);
void sorting_files(GDir* contents, PATH* sorted_files, gchar* aux);


#endif
