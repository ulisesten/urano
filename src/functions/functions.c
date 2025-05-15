#include "functions.h"

/// @brief Handle the project data like folder path, opened tabs, etc. (functions.c)
/// @param tree_view 
/// @param project_path 
void configure_project(GtkWidget* tree_view, char* project_path){

    if( project_path ) {

        //printf("%s\n", project_path);
        create_file_explorer((GtkTreeView*)tree_view, project_path);

    } else {
        printf("No recent project\n");
    }

}


/// @brief Function to open a project folder (functions.c)
/// @param widget
/// @param data 
void on_open_folder_button_clicked(GtkWidget* widget, gpointer data) {
    GtkWidget* folder_chooser;
    bool res;

    folder_chooser = gtk_file_chooser_dialog_new ("Select folder",
                             NULL,
                             GTK_FILE_CHOOSER_ACTION_SELECT_FOLDER,
                             "_Cancel", GTK_RESPONSE_CANCEL,
                             "_Open", GTK_RESPONSE_ACCEPT,
                             NULL);

    res = gtk_dialog_run((GtkDialog*)folder_chooser);
    if (res)
    {
        char* working_dir = gtk_file_chooser_get_filename((GtkFileChooser*)folder_chooser);
        create_file_explorer((GtkTreeView*)data, working_dir);
        set_project_path(working_dir);

        g_free (working_dir);

    } else {
        printf("open folder: something went wrong\n");
    }

    gtk_widget_destroy (folder_chooser);
}

void get_window_position(GtkWindow *window, GtkWidget* header){
    gtk_window_set_position (window, GTK_WIN_POS_MOUSE);
}


void on_quit_btn_clicked(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

/// @brief Opens the search dialog (functions.c)
/// @param source 
/// @param data 
void on_toggle_search_clicked(GtkToggleButton *source, gpointer data){
    
    GtkSearchBar* search_bar = (GtkSearchBar*) data;
    
    bool toggle = !gtk_search_bar_get_search_mode(search_bar);
    gtk_search_bar_set_search_mode(search_bar, toggle );

}


/// @brief Shows and hide the Terminal (functions.c).
/// @param widget 
/// @param data 
void on_terminal_button_clicked(GtkToggleButton* widget, gpointer data){

    GtkRevealer* terminal = (GtkRevealer*) data;

    bool toggle = !gtk_revealer_get_reveal_child(terminal);
    gtk_revealer_set_reveal_child(terminal, toggle );

}


/// @brief Shows and hide the sidebar where file explorer is placed (functions.c).
/// @param source 
/// @param data 
void on_toggle_sidebar_clicked(GtkToggleButton *source, gpointer data){

    GtkRevealer* side_bar = (GtkRevealer*) data;

    bool toggle = !gtk_revealer_get_reveal_child(side_bar);
    gtk_revealer_set_reveal_child(side_bar, toggle );

}


char* get_project_path() {
    FILE *fptr = NULL;
    char* project_path;

    fptr = fopen(g_strconcat(g_get_user_data_dir (), "/urano/project_info.txt", NULL), "r");

    if (fptr == NULL) {
        printf("Error opening project info\n");
        
    } else {

        project_path = (char*)malloc(sizeof(char*)* 1024);
        fscanf(fptr, "%s", project_path);
        fclose(fptr);
        
    }

    return project_path;
        
}


void  set_project_path(char* path_name) {
    FILE *fptr = NULL;

    int result = mkdir(g_strconcat(g_get_user_data_dir (), "/urano", NULL), 0777);
    fptr = fopen(g_strconcat(g_get_user_data_dir (), "/urano/project_info.txt", NULL), "w");

    // exiting program 
    if (fptr == NULL) {
        printf("Error writing project info\n");
        
    } else {

        fprintf(fptr, "%s", path_name);
        fclose(fptr);

    }
        
}