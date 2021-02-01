#include "file_explorer.h"
#include "file_reader.h"

#include <stdio.h>
#include <stdlib.h>
#include <gtk/gtk.h>
#include <vte/vte.h>
#include <unistd.h>

#define UNUSED(x) (void)(x)

static void on_quit_btn_clicked(          GtkWidget          *widget,           gpointer           data);
static void on_toggle_search_clicked(     GtkToggleButton    *source,           gpointer           data);
static void on_terminal_button_clicked(   GtkToggleButton    *widget,           gpointer           data);
static void on_toggle_sidebar_clicked(    GtkToggleButton    *source,           gpointer           data);
static void create_terminal(              GtkWidget          *container);
static void on_terminal_get_entry(        VteTerminal        *terminal,         gchar             *text,                guint     size,      gpointer  user_data);
static void child_ready(                  VteTerminal        *terminal,         GPid               pid,                 GError   *error,     gpointer  user_data);



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
    notebook_container       = GTK_WIDGET(gtk_builder_get_object(builder, "notebook_container"    ));


    //stack             = GTK_WIDGET(gtk_builder_get_object(builder, "notebook"           ));
    notebook = gtk_notebook_new();

    /**SIGNALS*/
    g_signal_connect(window, "destroy", G_CALLBACK(gtk_main_quit), NULL );
    g_signal_connect(toggle_search, "toggled", G_CALLBACK(on_toggle_search_clicked), search_bar);
    g_signal_connect(terminal_button, "toggled", G_CALLBACK(on_terminal_button_clicked), terminal_revealer);
    g_signal_connect(sidebar_button, "toggled", G_CALLBACK(on_toggle_sidebar_clicked), sidebar);


    create_terminal(terminal_container);
    GtkTreeSelection* selection = create_file_explorer(builder);
    
    
    gtk_widget_show(notebook);
    
    gtk_tree_selection_set_select_function(selection, view_selection_func, notebook, NULL);

    gtk_builder_connect_signals(builder, NULL);

    gtk_container_add(GTK_CONTAINER(notebook_container), notebook);

    g_object_unref(builder);
    gtk_widget_show(window);
    gtk_main();

    return 0;
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

static void create_terminal(GtkWidget* container){

    GtkWidget   * terminal = vte_terminal_new ();
    gchar       * cwd      = g_get_current_dir();

    /* Start a new shell */
    gchar       **envp      = g_get_environ();
    gchar       * arr[]     = {g_strdup(g_environ_getenv(envp, "SHELL")), NULL };
    gchar       **command   = arr;

    g_strfreev(envp);
    
    vte_terminal_spawn_async(VTE_TERMINAL(terminal),
        VTE_PTY_DEFAULT,
        cwd,         /* working directory  */
        command,      /* command */
        NULL,         /* environment */
        (GSpawnFlags)0, /* spawn flags */
        NULL, NULL,   /* child setup */
        NULL,         /* child pid */
        -1,           /* timeout */
        NULL,         /* cancellable */
        child_ready,  /* callback */
        NULL);        /* user_data */

    vte_terminal_set_size ((VteTerminal *)terminal, 256, 7);
    gtk_widget_show(terminal);
    gtk_container_add(GTK_CONTAINER(container), terminal);

}

static void on_terminal_get_entry(VteTerminal *terminal,gchar *text, guint size, gpointer user_data){
    
    vte_terminal_feed (terminal, (const char*)text, size );

}


static void child_ready(VteTerminal *terminal, GPid pid, GError *error, gpointer user_data)
{
    if (!terminal) return;
    if (pid == -1) gtk_main_quit();
}

