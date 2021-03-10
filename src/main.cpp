#include "file_browser.h"
#include "file_reader.h"
#include "terminal_emulator.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <unistd.h>

static void on_quit_btn_clicked(          GtkWidget          *widget,    gpointer     data);
static void on_toggle_search_clicked(     GtkToggleButton    *source,    gpointer     data);
static void on_terminal_button_clicked(   GtkToggleButton    *widget,    gpointer     data);
static void on_toggle_sidebar_clicked(    GtkToggleButton    *source,    gpointer     data);

/**to reallocate functions*/
gboolean on_drag_window (GtkWidget *widget, GdkDragContext *context,
                         int        x,      int             y,
                         guint      time,   gpointer        user_data );

void on_open_folder_button_clicked(GtkWidget* widget, gpointer data);


int main( int argc, char** argv ){

    GtkBuilder*       builder;
    GtkWidget*        header;
    GtkWidget*        window;
    GtkWidget*        window_controls;
    GtkCssProvider*   css_provider;
    GdkDisplay*       display;
    GtkWidget*        main_box;
    GtkWidget*        save_button;
    GtkWidget*        open_folder_button;
    GtkWidget*        toggle_search;
    GtkWidget*        search_bar;
    GtkWidget*        terminal_button;
    GtkWidget*        terminal_revealer;
    GtkWidget*        terminal_container;
    GtkWidget*        sidebar_button;
    GtkWidget*        notebook_container;
    GtkWidget*        notebook;
    GtkWidget*        tree_view;
    GtkTreeSelection* selection;
    GtkWidget*        sidebar;
    GError*           err          = NULL;
    GFile*            theme_file   = NULL;

    const gchar*      stylefile    = "../css/theme.css";
    const gchar*      formfile     = "../glade/ui.glade";
    const gchar*      iconfile     = "../assets/urano_icon2.jpg";

    gtk_init(&argc, &argv);

    builder      = gtk_builder_new();
    css_provider = gtk_css_provider_new();
    display      = gdk_display_get_default();
    theme_file   = g_file_new_for_path(stylefile);


    gtk_builder_add_from_file ( builder, formfile, &err );
    if(err){
        fprintf(stderr, ("Failed to load builder: %s\n"), err->message);
        g_error_free(err);
        err = NULL;
    }

    gtk_css_provider_load_from_file (css_provider, theme_file, &err);
    if(err){
        fprintf(stderr, ("Failed to load css_provider: %s\n"), err->message);
        g_error_free(err);
        err = NULL;
    }

    gtk_style_context_add_provider_for_screen(gdk_display_get_default_screen(display), (GtkStyleProvider*)css_provider, GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    

    window             = GTK_WIDGET(gtk_builder_get_object(builder, "window"              ));
    main_box           = GTK_WIDGET(gtk_builder_get_object(builder, "main_box"            ));
    toggle_search      = GTK_WIDGET(gtk_builder_get_object(builder, "search"              ));
    search_bar         = GTK_WIDGET(gtk_builder_get_object(builder, "searchbar"           ));
    terminal_button    = GTK_WIDGET(gtk_builder_get_object(builder, "terminalbutton"      ));
    terminal_revealer  = GTK_WIDGET(gtk_builder_get_object(builder, "terminalrevealer"    ));
    terminal_container = GTK_WIDGET(gtk_builder_get_object(builder, "terminal_container"  ));
    sidebar_button     = GTK_WIDGET(gtk_builder_get_object(builder, "sidebarbutton"       ));
    sidebar            = GTK_WIDGET(gtk_builder_get_object(builder, "sidebar_revealer"    ));
    notebook_container = GTK_WIDGET(gtk_builder_get_object(builder, "notebook_container"  ));
    save_button        = GTK_WIDGET(gtk_builder_get_object(builder, "save_btn"            ));
    open_folder_button = GTK_WIDGET(gtk_builder_get_object(builder, "open_folder_btn"     ));
    tree_view          = GTK_WIDGET(gtk_builder_get_object(builder, "tree_view"           ));


    selection  = gtk_tree_view_get_selection ( (GtkTreeView*)tree_view);
    notebook           = create_notebook(      &header);
                         create_terminal(       terminal_container);

    gtk_tree_selection_set_select_function(selection, attach_notebook_to_selection, notebook, NULL);
    gtk_container_add(GTK_CONTAINER(notebook_container), notebook);

    /**SIGNALS*/
    g_signal_connect(window,             "destroy", G_CALLBACK(gtk_main_quit), NULL );
    g_signal_connect(toggle_search,      "toggled", G_CALLBACK(on_toggle_search_clicked),       search_bar);
    g_signal_connect(terminal_button,    "toggled", G_CALLBACK(on_terminal_button_clicked),     terminal_revealer);
    g_signal_connect(sidebar_button,     "toggled", G_CALLBACK(on_toggle_sidebar_clicked),      sidebar);
    g_signal_connect(open_folder_button, "clicked", G_CALLBACK(on_open_folder_button_clicked),  tree_view);

    //gtk_window_set_titlebar((GtkWindow*)window, header);
    gtk_window_set_icon_from_file((GtkWindow*)window, iconfile, NULL);

    gtk_builder_connect_signals( builder, NULL);
    gtk_widget_show(notebook);
    gtk_widget_show(window);

    g_object_unref(builder);
    g_object_unref(theme_file);
    g_object_unref(css_provider);

    gtk_main();

    return 0;

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

    //gtk_widget_show(folder_chooser);
    res = gtk_dialog_run((GtkDialog*)folder_chooser);
    if (res)
    {

        char* working_dir = gtk_file_chooser_get_filename((GtkFileChooser*)folder_chooser);
        printf("open folder %s\n", working_dir);
        
        create_file_explorer((GtkTreeView*)data, working_dir);

        g_free (working_dir);

    } else {
        printf("open folder: something went wrong\n");
    }

    gtk_widget_destroy (folder_chooser);

}

void get_window_position(GtkWindow *window, GtkWidget* header){
    gtk_window_set_position (window, GTK_WIN_POS_MOUSE);
}

gboolean on_drag_window (GtkWidget *widget, GdkDragContext *context,
                         int        x,      int             y,
                         guint      time,   gpointer        user_data ) {

    printf("motion\n");
    return FALSE;

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