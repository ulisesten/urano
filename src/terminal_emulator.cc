#include "terminal_emulator.h"

void create_terminal(GtkWidget* container, char* project_path){

    GtkWidget   * terminal = vte_terminal_new ();
    //gchar       * cwd      = g_get_current_dir();
    GdkRGBA       foreground;
    GdkRGBA       background;
    //GdkRGBA       bold;

    /* Start a new shell */
    gchar       **envp      = g_get_environ();
    gchar       * arr[]     = {g_strdup(g_environ_getenv(envp, "SHELL")), NULL };
    gchar       **command   = arr;

    g_strfreev(envp);
    
    vte_terminal_spawn_async(VTE_TERMINAL(terminal),
        VTE_PTY_DEFAULT,
        project_path, /* working directory  */
        command,      /* command */
        NULL,         /* environment */
        (GSpawnFlags)0, /* spawn flags */
        NULL, NULL,   /* child setup */
        NULL,         /* child pid */
        -1,           /* timeout */
        NULL,         /* cancellable */
        child_ready,  /* callback */
        NULL);        /* user_data */

    vte_terminal_set_size ((VteTerminal *)terminal, 512, 5);


    gdk_rgba_parse (&foreground, "#babdb6");
    gdk_rgba_parse (&background, "#242424");
    //vte_terminal_set_color_background ((VteTerminal *)terminal, &background);

    //gdk_rgba_parse (&bold, "#2b7ca0");
    //vte_terminal_set_color_bold ((VteTerminal *)terminal, &bold);
    GdkRGBA color; 
    GdkRGBA palette[16];

    gdk_rgba_parse (&color, "#073642");
    palette[0] = color;
    gdk_rgba_parse (&color, "#dc322f");
    palette[1] = color;
    gdk_rgba_parse (&color, "#859900");
    palette[2] = color;
    gdk_rgba_parse (&color, "#b58900");
    palette[3] = color;
    gdk_rgba_parse (&color, "#268bd2");
    palette[4] = color;
    gdk_rgba_parse (&color, "#d33682");
    palette[5] = color;
    gdk_rgba_parse (&color, "#2aa198");
    palette[6] = color;
    gdk_rgba_parse (&color, "#eee8d5");
    palette[7] = color;
    gdk_rgba_parse (&color, "#002b36");
    palette[8] = color;
    gdk_rgba_parse (&color, "#cb4b16");
    palette[9] = color;
    gdk_rgba_parse (&color, "#586e75");
    palette[10] = color;
    gdk_rgba_parse (&color, "#657b83");
    palette[11] = color;
    gdk_rgba_parse (&color, "#839496");
    palette[12] = color;
    gdk_rgba_parse (&color, "#6c71c4");
    palette[13] = color;
    gdk_rgba_parse (&color, "#93a1a1");
    palette[14] = color;
    gdk_rgba_parse (&color, "#fdf6e3");
    palette[15] = color;

    vte_terminal_set_colors((VteTerminal *)terminal
                            , &foreground
                            , &background
                            , palette
                            , 16 );

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