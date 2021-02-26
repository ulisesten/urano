#include "terminal_emulator.h"

void create_terminal(GtkWidget* container){

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