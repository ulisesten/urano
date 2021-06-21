#include "file_browser.h"


GtkTreeSelection* create_file_explorer(GtkTreeView* tv, char* working_dir){
    File_browser  fb;
    GtkIconTheme *icon_theme;
    GError       *error = NULL;

    if( gtk_tree_view_get_n_columns (tv) > 0) {
        gtk_tree_view_remove_column (tv, gtk_tree_view_get_column (tv,0));
    }

    fb.tree_view  = tv;
    fb.column     = gtk_tree_view_column_new    ();

    fb.renderer   = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start             (fb.column, fb.renderer, FALSE);
    gtk_tree_view_column_set_attributes         (fb.column, fb.renderer, "pixbuf", COLUMN_PIXBUF, NULL);
    
    fb.renderer   = gtk_cell_renderer_text_new  ();
    gtk_tree_view_column_pack_start             (fb.column, fb.renderer, TRUE);
    gtk_tree_view_column_set_attributes         (fb.column, fb.renderer, "text", COLUMN_STRING, NULL);

    fb.renderer   = gtk_cell_renderer_text_new  ();
    gtk_tree_view_column_pack_start             (fb.column, fb.renderer, TRUE);
    gtk_tree_view_column_set_attributes         (fb.column, fb.renderer, "text", COLUMN_PATH, NULL);
    gtk_cell_renderer_set_visible               (fb.renderer, FALSE);


    icon_theme             = gtk_icon_theme_get_default ();
    fb.pixbuf_folder       = gtk_icon_theme_load_icon (icon_theme, "folder", 16, GTK_ICON_LOOKUP_NO_SVG, &error);
    fb.pixbuf_text_file    = gtk_icon_theme_load_icon (icon_theme, "text-x-generic", 16, GTK_ICON_LOOKUP_NO_SVG, &error);

    if (error)
    {
        g_critical ("Could not load pixbuf: %s\n", error->message);
        g_error_free(error);
        error = NULL;
    }

    fb.tree_store = gtk_tree_store_new( 3, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING );

    setup_file_explorer(NULL, fb, working_dir);
    
    g_object_unref(fb.pixbuf_folder);
    g_object_unref(fb.pixbuf_text_file);

    /**We return selection so
     * we can attach it to another widget*/
    return (fb.selection);

}

void setup_file_explorer(GtkTreeViewColumn* column, File_browser fb, char* working_dir){

    GError* gerr;
    gint pos = 0;
    GtkTreeIter iter, iter_child;
    gchar* project_name;
    PATH paths = NULL;
    int list_err = -1;
    
    /**paths is a list that contains all the info
     * to fill tree_store as the model to treeview*/
    list_err = filling_list(&paths, working_dir);
    if(list_err < 0 ) {
        printf("error al llenar lista.\n");
    }

    project_name = g_path_get_basename(working_dir);

    initialize_tree_store(&fb, &iter, project_name);

    fill_tree_store(paths, &iter, NULL, fb, 0);
    
    set_tree_view(fb);

    if(paths)
        free(paths);

}


void set_tree_view(File_browser fb){
                                                     
    gtk_tree_view_set_model(fb.tree_view, GTK_TREE_MODEL( fb.tree_store ));
    gtk_tree_view_append_column(fb.tree_view, fb.column);
    
}


void initialize_tree_store(  File_browser* fb, GtkTreeIter * iter, char* project_name){
    /**This only adds a first empty row in the treeview
     * and the tree title */
    gtk_tree_view_column_set_title(fb->column, project_name);
    gtk_tree_store_append(fb->tree_store, &(*iter), NULL);
}


void fill_tree_store(PATH paths, GtkTreeIter* iter, GtkTreeIter* parent, File_browser fb, int indent){

    GtkTreeIter new_iter;
        
    while(paths){

        if(paths->type == FOLDER_TYPE){

            gtk_tree_store_set (fb.tree_store, iter, COLUMN_PIXBUF, fb.pixbuf_folder, COLUMN_STRING, paths->dir, COLUMN_PATH, paths->path, -1);

            bool branched = false;
            if( paths->branch ) {

                gtk_tree_store_append(fb.tree_store, &new_iter, iter);
                branched = true;
                fill_tree_store( paths->branch , &new_iter, iter, fb, indent+1);

            }
             
            if(paths->next)
                if(parent)
                    gtk_tree_store_append(fb.tree_store, iter, parent);
                else
                    gtk_tree_store_append(fb.tree_store, iter, NULL);            

        } else {
            
            gtk_tree_store_set (fb.tree_store, iter, COLUMN_PIXBUF, fb.pixbuf_text_file, COLUMN_STRING, paths->dir, COLUMN_PATH, paths->path, -1);

            if(paths->next)
                if(parent)
                    gtk_tree_store_append(fb.tree_store, iter, parent);
                else
                    gtk_tree_store_append(fb.tree_store, iter, NULL);
            
        }

        paths = paths->next;        

    }

}


gboolean attach_notebook_to_selection (
             GtkTreeSelection *selection
           , GtkTreeModel     *model
           , GtkTreePath      *path
           , gboolean          path_currently_selected
           , gpointer          userdata
        ) {

    GtkTreeIter iter;
    GtkWidget* notebook = (GtkWidget*) userdata;

    if (gtk_tree_model_get_iter(model, &iter, path)) {

        gchar *name     = NULL;
        gchar *location = NULL;
        gchar *type     = NULL;

        gtk_tree_model_get(model, &iter, COLUMN_STRING, &name, COLUMN_PATH, &location, -1);

        if(location){

            if(!path_currently_selected){

                if(g_str_has_suffix (name, ".png")) {

                    printf("PNG image\n");

                } else {

                    set_notebook(notebook, name, location);

                }


            }

        } else {

            printf("no location\n");

        }

        g_free(name);
        g_free(location);
        g_free(type);

    }

    return FALSE;
}



