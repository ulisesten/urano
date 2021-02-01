#include "file_explorer.h"
#include "file_reader.h"
#include <stdio.h>


GtkTreeSelection* create_file_explorer(GtkBuilder* builder){
    GError *error = NULL;
    file_browser fb;


    fb.tree_view         = GTK_TREE_VIEW(gtk_builder_get_object(builder, "tree_view"));
    fb.selection         = gtk_tree_view_get_selection(GTK_TREE_VIEW(fb.tree_view));
    fb.column0           = gtk_tree_view_column_new();


    fb.renderer   = gtk_cell_renderer_pixbuf_new();
    gtk_tree_view_column_pack_start(fb.column0, fb.renderer, FALSE);
    gtk_tree_view_column_set_attributes(fb.column0, fb.renderer,
                                        "pixbuf", COLUMN_PIXBUF,
                                        NULL);
    
    fb.renderer   = gtk_cell_renderer_text_new() ;
    gtk_tree_view_column_pack_start(fb.column0, fb.renderer, TRUE);
    gtk_tree_view_column_set_attributes(fb.column0, fb.renderer,
                                        "text", COLUMN_STRING,
                                        NULL);


    fb.pixbuf            = gdk_pixbuf_new_from_file("../assets/folder2_32.png", &error);
    fb.pixbuf_text       = gdk_pixbuf_new_from_file("../assets/text3_32.png", &error);

    if (error)
    {
        g_critical ("Could not load pixbuf: %s\n", error->message);
        g_error_free(error);
    }

    fb.list_store = gtk_list_store_new( 2, GDK_TYPE_PIXBUF, G_TYPE_STRING );

    setup_file_explorer(NULL, fb);
    
    g_object_unref(fb.pixbuf);

    return (fb.selection);

}

void setup_file_explorer(GtkTreeViewColumn* column, file_browser fBrowser){
    
    GDir* dir_contents;
    GError* gerr;
    gint pos = 0;
    GtkTreeIter iter;
    gchar* folder_name;
    gchar* cwd = g_get_current_dir();

    printf("cwd %s\n", cwd);

    dir_contents = g_dir_open(cwd, 0, &gerr);

    folder_name = get_project_folder_name(cwd);
    
    gtk_tree_view_column_set_title(fBrowser.column0, folder_name);

    walk_dir(dir_contents, &iter, fBrowser);
    
    set_tree_view(fBrowser);

    g_free(dir_contents);
}


void set_tree_view(file_browser fb){
                                                     
    //g_object_set (fb.renderer, "xalign", 0.0, NULL);
    gtk_tree_view_append_column(fb.tree_view, fb.column0);
    gtk_tree_view_set_model(fb.tree_view, GTK_TREE_MODEL( fb.list_store ));
    //GtkTreeViewColumn* exp_col = gtk_tree_view_column_new();
    //gtk_tree_view_set_expander_column (fb.tree_view, exp_col);
    
}


void walk_dir(GDir* contents, GtkTreeIter* iter, file_browser fb){

    gchar* dir = (char*)g_dir_read_name(contents);
    struct stat sb;

    if(dir){
        //printf("%s\n", dir);
        gtk_list_store_append(fb.list_store, iter);

        if(stat(dir, &sb) == 0 && S_ISDIR(sb.st_mode)){
        //if(g_file_test(dir, G_FILE_TEST_IS_DIR)){

            gtk_list_store_set (fb.list_store, iter, COLUMN_PIXBUF, fb.pixbuf, COLUMN_STRING, dir, -1);

            GDir* new_dir_contents;
            GtkTreeIter iter_child;
            gchar* next_dir;
            GError* gerr = NULL;
            
            next_dir = g_strconcat(g_get_current_dir(), "/", dir, NULL);
            //printf("%s\n", dir);
            
            new_dir_contents = g_dir_open(next_dir, 0, &gerr);
            if(gerr != NULL){
                fprintf(stderr, "gerror %s.\n", gerr->message);
                g_error_free(gerr);
            }

            walk_dir(new_dir_contents, &iter_child, fb);
            g_free(new_dir_contents);
            g_free(next_dir);

        } else {

            gtk_list_store_set (fb.list_store, iter, COLUMN_PIXBUF, fb.pixbuf_text, COLUMN_STRING, dir, -1);

        }
        
        
        walk_dir(contents, iter, fb);

    }

}


gboolean
  view_selection_func (GtkTreeSelection *selection,
                       GtkTreeModel     *model,
                       GtkTreePath      *path,
                       gboolean          path_currently_selected,
                       gpointer          userdata) {

    GtkTreeIter iter;
    GtkWidget* notebook = (GtkWidget*) userdata;

    
    bool toggle = false;

    if (gtk_tree_model_get_iter(model, &iter, path))
    {
        gchar *name;

        gtk_tree_model_get(model, &iter, COLUMN_STRING, &name, -1);

        if (!path_currently_selected) {

            toggle = true;

        } else {

            toggle = false;
        }

        if(toggle){
        
            set_notebook(notebook, name);

        }
    
        g_free(name);

    }

    
    return TRUE; /* allow selection state to change */

  }


gchar* get_project_folder_name(gchar* path){
    gchar** elements = g_strsplit (path, "/", -1);

    gchar* folder_name = elements[sizeof(elements)];

    return folder_name;
}


void sorting_files(GDir* contents, PATH* sorted_files, gchar* aux){

    gchar* dir = (char*)g_dir_read_name(contents);

    if (dir) {

        sorted_files->dir = dir;

        if(g_file_test(dir, G_FILE_TEST_IS_DIR)){

            sorted_files->type = FOLDER_TYPE;

        } else {

            sorted_files->type = FILE_TYPE;

        }

    }

}


