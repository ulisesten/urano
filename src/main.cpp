#include "file_explorer.h"
#include "file_reader.h"
#include "terminal_emulator.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
//#include <vte/vte.h>
#include <unistd.h>

//#define UNUSED(x) (void)(x)

static void on_quit_btn_clicked(          GtkWidget          *widget,           gpointer           data);
static void on_toggle_search_clicked(     GtkToggleButton    *source,           gpointer           data);
static void on_terminal_button_clicked(   GtkToggleButton    *widget,           gpointer           data);
static void on_toggle_sidebar_clicked(    GtkToggleButton    *source,           gpointer           data);

/**to reallocate functions*/
GtkWidget* create_window_controls(GtkWidget* window);


int main( int argc, char** argv ){

    GtkBuilder*      builder;
    GtkWidget*       window;
    GtkWidget*       label1;
    GtkCssProvider*  css_provider;
    GdkDisplay*      display;
    GtkWidget*       quit_btn;
    GtkWidget*       toggle_search;
    GtkWidget*       search_bar;
    GtkWidget*       terminal_button;
    GtkWidget*       terminal_revealer;
    GtkWidget*       terminal_container;
    GtkWidget*       sidebar_button;
    GtkWidget*       notebook_container;
    GtkWidget*       notebook;
    GtkWidget*       sidebar;
    GError*          err          = NULL;
    GFile*           theme_file   = NULL;
    const gchar*     stylefile    = "./css/theme.css";


    gtk_init(&argc, &argv);

    builder      = gtk_builder_new();
    css_provider = gtk_css_provider_new();
    display      = gdk_display_get_default();
    theme_file   = g_file_new_for_path("../css/theme.css");


    gtk_builder_add_from_file ( builder, "../glade/ui.glade", &err );

    gtk_css_provider_load_from_file (css_provider, theme_file, &err);
    if(err){
        fprintf(stderr, ("Failed to init GConf: %s\n"), err->message);
        g_error_free(err);
        err = NULL;
    }

    gtk_style_context_add_provider_for_screen(gdk_display_get_default_screen(display), GTK_STYLE_PROVIDER( css_provider ), GTK_STYLE_PROVIDER_PRIORITY_APPLICATION);
    
    g_object_unref(css_provider);

    window            = GTK_WIDGET(gtk_builder_get_object(builder, "window"              ));
    toggle_search     = GTK_WIDGET(gtk_builder_get_object(builder, "search"              ));
    search_bar        = GTK_WIDGET(gtk_builder_get_object(builder, "searchbar"           ));
    terminal_button   = GTK_WIDGET(gtk_builder_get_object(builder, "terminalbutton"      ));
    terminal_revealer = GTK_WIDGET(gtk_builder_get_object(builder, "terminalrevealer"    ));
    terminal_container= GTK_WIDGET(gtk_builder_get_object(builder, "terminal_container"  ));
    sidebar_button    = GTK_WIDGET(gtk_builder_get_object(builder, "sidebarbutton"       ));
    sidebar           = GTK_WIDGET(gtk_builder_get_object(builder, "sidebar_revealer"    ));
    notebook_container= GTK_WIDGET(gtk_builder_get_object(builder, "notebook_container"  ));

    notebook = gtk_notebook_new();
    GtkWidget* window_controls;
    gtk_notebook_set_scrollable ((GtkNotebook *)notebook, true);
    window_controls = create_window_controls(window);
    gtk_notebook_set_action_widget ((GtkNotebook *)notebook, window_controls, GTK_PACK_END);

    /**SIGNALS*/
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL );
    g_signal_connect(toggle_search, "toggled", G_CALLBACK(on_toggle_search_clicked), search_bar);
    g_signal_connect(terminal_button, "toggled", G_CALLBACK(on_terminal_button_clicked), terminal_revealer);
    g_signal_connect(sidebar_button, "toggled", G_CALLBACK(on_toggle_sidebar_clicked), sidebar);


    create_terminal(terminal_container);
    GtkTreeSelection* selection = create_file_explorer(builder);
    
    gtk_tree_selection_set_select_function(selection, view_selection_func, notebook, NULL);

    gtk_builder_connect_signals(builder, NULL);

    gtk_container_add(GTK_CONTAINER(notebook_container), notebook);

    gtk_widget_show(window_controls);
    gtk_widget_show(notebook);
    gtk_widget_show(window);
    g_object_unref(builder);
    gtk_main();

    return 0;
}


GtkWidget* create_window_controls(GtkWidget* window){
    
    GtkWidget *bar = gtk_header_bar_new ();

    gtk_header_bar_set_show_close_button ((GtkHeaderBar *)bar, true);

    return bar;
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