#include "file_explorer.h"

GtkTreeSelection* create_file_explorer(GtkBuilder* builder){
    GError       *error = NULL;
    file_browser  fb;
    GtkIconTheme *icon_theme;

    fb.tree_view  = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tree_view"));
    fb.selection  = gtk_tree_view_get_selection (GTK_TREE_VIEW(fb.tree_view));
    fb.column0    = gtk_tree_view_column_new    ();

    fb.renderer   = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start             (fb.column0, fb.renderer, FALSE);
    gtk_tree_view_column_set_attributes         (fb.column0, fb.renderer, "pixbuf", COLUMN_PIXBUF, NULL);
    
    fb.renderer   = gtk_cell_renderer_text_new  ();
    gtk_tree_view_column_pack_start             (fb.column0, fb.renderer, TRUE);
    gtk_tree_view_column_set_attributes         (fb.column0, fb.renderer, "text", COLUMN_STRING, NULL);

    fb.renderer   = gtk_cell_renderer_text_new  ();
    gtk_tree_view_column_pack_start             (fb.column0, fb.renderer, TRUE);
    gtk_tree_view_column_set_attributes         (fb.column0, fb.renderer, "text", COLUMN_PATH, NULL);
    gtk_cell_renderer_set_visible               (fb.renderer, FALSE);


    icon_theme = gtk_icon_theme_get_default ();
    fb.pixbuf_folder            = gtk_icon_theme_load_icon (icon_theme, "folder", 16, GTK_ICON_LOOKUP_NO_SVG, &error);
    fb.pixbuf_text_file         = gtk_icon_theme_load_icon (icon_theme, "text-x-generic", 16, GTK_ICON_LOOKUP_NO_SVG, &error);

    if (error)
    {
        g_critical ("Could not load pixbuf: %s\n", error->message);
        g_error_free(error);
        error = NULL;
    }

    fb.tree_store = gtk_tree_store_new( 3, GDK_TYPE_PIXBUF, G_TYPE_STRING, G_TYPE_STRING );

    setup_file_explorer(NULL, fb);
    
    g_object_unref(fb.pixbuf_folder);
    g_object_unref(fb.pixbuf_text_file);

    return (fb.selection);

}

void setup_file_explorer(GtkTreeViewColumn* column, file_browser fBrowser){

    GError* gerr;
    gint pos = 0;
    GtkTreeIter iter, iter_child;
    gchar* folder_name;
    char* cwd = g_get_current_dir();
    PATH paths = NULL;
    int list_err = -1;
    //PATH aux = paths;

    printf("cwd %s\n", cwd);
    
    list_err = filling_list(&paths, cwd);
    if(list_err < 0 )
        printf("error al llenar lista.\n");

    //walkList(paths, cwd);

    folder_name = g_path_get_basename(cwd);
    gtk_tree_view_column_set_title(fBrowser.column0, folder_name);

    initialize_tree_store(&fBrowser, &iter);
    fill_tree_store(paths, &iter, NULL, fBrowser, 0);
    
    set_tree_view(fBrowser);
    free(paths);

}


void set_tree_view(file_browser fb){
                                                     
    gtk_tree_view_set_model(fb.tree_view, GTK_TREE_MODEL( fb.tree_store ));
    gtk_tree_view_append_column(fb.tree_view, fb.column0);
    
}


void initialize_tree_store(  file_browser* fb, GtkTreeIter * iter){
    gtk_tree_store_append(fb->tree_store, &(*iter), NULL);
}


void fill_tree_store(PATH paths, GtkTreeIter* iter, GtkTreeIter* parent, file_browser fb, int indent){

    GtkTreeIter new_iter;
        
    while(paths){

        if(paths->type == FOLDER_TYPE){

            gtk_tree_store_set (fb.tree_store, iter, COLUMN_PIXBUF, fb.pixbuf_folder, COLUMN_STRING, paths->dir, COLUMN_PATH, paths->path, -1);

            bool branched = false;
            if( paths->branch ) {
                //printf("branch in %s\n", paths->dir);
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


gboolean
  view_selection_func (GtkTreeSelection *selection
                     , GtkTreeModel *model
                     , GtkTreePath *path
                     , gboolean path_currently_selected
                     , gpointer userdata) {

    GtkTreeIter iter;
    GtkWidget* notebook = (GtkWidget*) userdata;

    if (gtk_tree_model_get_iter(model, &iter, path))
    {
        gchar *name;
        gchar *location = NULL;

        gtk_tree_model_get(model, &iter, COLUMN_STRING, &name, COLUMN_PATH, &location, -1);
        //gtk_tree_model_get(model, &iter, COLUMN_PATH, &location, -1);

        //GtkTreePath* row_path = gtk_tree_model_get_path(model, &iter);

        if(location){
            if(!path_currently_selected){

                strcat(location,"/");

                //gint pos = gtk_notebook_get_n_pages ((GtkNotebook *)notebook);
                set_notebook(notebook, name, location);

            }
        } else {
            printf("no location\n");
        }

        //gtk_tree_path_free(row_path);
        g_free(name);

    }

    return TRUE;
}



