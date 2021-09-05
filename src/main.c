#include "file_tree_view.h"
#include "notebook.h"
#include "terminal_emulator.h"
#include "project_handler.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <unistd.h>
#include <stdbool.h>

typedef struct {
    GtkBuilder*       builder;
    GtkHeaderBar*     header;
    GtkWidget*        window;
    GtkCssProvider*   css_provider;
    GdkDisplay*       display;
} Window;

typedef struct {
    GtkWidget*        save;
    GtkWidget*        open_folder;
    GtkWidget*        terminal;
    GtkWidget*        sidebar;
} Button;

typedef struct {
    GtkWidget*        revealer;
    GtkWidget*        container;
} Term;

typedef struct {
    GtkWidget*        container;
    GtkWidget*        notebook;
} Notebook;

void configure_project(GtkWidget* tree_view, char* path);

static void on_quit_btn_clicked(          GtkWidget          *widget,    gpointer     data);
static void on_toggle_search_clicked(     GtkToggleButton    *source,    gpointer     data);
static void on_terminal_button_clicked(   GtkToggleButton    *widget,    gpointer     data);
static void on_toggle_sidebar_clicked(    GtkToggleButton    *source,    gpointer     data);

/**to reallocate functions*/
void on_open_folder_button_clicked(GtkWidget* widget, gpointer data);


int main( int argc, char** argv ){

    GtkHeaderBar* header_bar;

    Window win;
    Button button;
    Term   terminal;
    Notebook notebook;
    
    GtkWidget*        toggle_search;
    GtkWidget*        search_bar;
    GtkWidget*        tree_view;
    GtkTreeSelection* selection;
    GtkWidget*        sidebar;
    GError*           err          = NULL;
    GFile*            theme_file   = NULL;
    
    const gchar*      style_file    = "../css/theme.css"  ;
    const gchar*      form_file     = "../glade/ui.glade" ;
    const gchar*      icon_file     = "../assets/icon.png";
    
    //printf("path %s\n", g_get_user_config_dir ());
    //printf("user data dir %s\n", g_get_user_data_dir ());

    gtk_init(&argc, &argv);
    {

        win.builder      = gtk_builder_new();
        win.css_provider = gtk_css_provider_new();
        win.display      = gdk_display_get_default();
        theme_file       = g_file_new_for_path(style_file);


        gtk_builder_add_from_file ( win.builder, form_file, &err );
        if( err ) {
            fprintf(stderr, ("Failed to load builder: %s\n"), err->message);
            g_error_free(err);
            err = NULL;
        }

        gtk_css_provider_load_from_file (win.css_provider, theme_file, &err);
        if( err ) {
            fprintf(stderr, ("Failed to load css_provider: %s\n"), err->message);
            g_error_free(err);
            err = NULL;
        }

        gtk_style_context_add_provider_for_screen(gdk_display_get_default_screen(win.display), (GtkStyleProvider*)win.css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
        

        win.window         = GTK_WIDGET(gtk_builder_get_object(win.builder, "window"              ));
        toggle_search      = GTK_WIDGET(gtk_builder_get_object(win.builder, "search"              ));
        search_bar         = GTK_WIDGET(gtk_builder_get_object(win.builder, "searchbar"           ));
        button.terminal    = GTK_WIDGET(gtk_builder_get_object(win.builder, "terminalbutton"      ));
        terminal.revealer  = GTK_WIDGET(gtk_builder_get_object(win.builder, "terminalrevealer"    ));
        terminal.container = GTK_WIDGET(gtk_builder_get_object(win.builder, "terminal_container"  ));
        button.sidebar     = GTK_WIDGET(gtk_builder_get_object(win.builder, "sidebarbutton"       ));
        sidebar            = GTK_WIDGET(gtk_builder_get_object(win.builder, "sidebar_revealer"    ));
        notebook.container = GTK_WIDGET(gtk_builder_get_object(win.builder, "notebook_container"  ));
        button.save        = GTK_WIDGET(gtk_builder_get_object(win.builder, "save_btn"            ));
        button.open_folder = GTK_WIDGET(gtk_builder_get_object(win.builder, "open_folder_btn"     ));
        tree_view          = GTK_WIDGET(gtk_builder_get_object(win.builder, "tree_view"           ));


        selection          = gtk_tree_view_get_selection ( (GtkTreeView*)tree_view   );
        notebook.notebook  = create_notebook();

        char* project_path = get_project_path();

        configure_project(  tree_view,          project_path);
        create_terminal(    terminal.container ,project_path);


        gtk_tree_selection_set_select_function(selection, attach_notebook_to_selection, notebook.notebook, NULL);
        gtk_container_add(GTK_CONTAINER(notebook.container), notebook.notebook);

        /**SIGNALS*/
        g_signal_connect(win.window,         "destroy", G_CALLBACK(gtk_main_quit), NULL );
        g_signal_connect(toggle_search,      "toggled", G_CALLBACK(on_toggle_search_clicked),       search_bar);
        g_signal_connect(button.terminal,    "toggled", G_CALLBACK(on_terminal_button_clicked),     terminal.revealer);
        g_signal_connect(button.sidebar,     "toggled", G_CALLBACK(on_toggle_sidebar_clicked),      sidebar);
        g_signal_connect(button.open_folder, "clicked", G_CALLBACK(on_open_folder_button_clicked),  tree_view);


        gtk_builder_connect_signals( win.builder, NULL);
        gtk_widget_show(notebook.notebook);
        gtk_widget_show(win.window);

        g_object_unref(win.builder);
        g_object_unref(theme_file);
        g_object_unref(win.css_provider);

    }
    gtk_main();

    return 0;

}

void configure_project(GtkWidget* tree_view, char* project_path){

    if( project_path ) {

        //printf("%s\n", project_path);
        create_file_explorer((GtkTreeView*)tree_view, project_path);

    } else {
        printf("No recent project\n");
    }

}



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


static void on_quit_btn_clicked(GtkWidget *widget, gpointer data){
    gtk_main_quit();
}

static void on_toggle_search_clicked(GtkToggleButton *source, gpointer data){
    
    GtkSearchBar* search_bar = (GtkSearchBar*) data;
    
    bool toggle = !gtk_search_bar_get_search_mode(search_bar);
    gtk_search_bar_set_search_mode(search_bar, toggle );

}

static void on_terminal_button_clicked(GtkToggleButton* widget, gpointer data){

    GtkRevealer* terminal = (GtkRevealer*) data;

    bool toggle = !gtk_revealer_get_reveal_child(terminal);
    gtk_revealer_set_reveal_child(terminal, toggle );

}

static void on_toggle_sidebar_clicked(GtkToggleButton *source, gpointer data){

    GtkRevealer* side_bar = (GtkRevealer*) data;

    bool toggle = !gtk_revealer_get_reveal_child(side_bar);
    gtk_revealer_set_reveal_child(side_bar, toggle );

}