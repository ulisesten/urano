#ifndef __TERMINAL_EMULATOR__
#define __TERMINAL_EMULATOR__

//#include <gtk/gtk.h>
#include <vte/vte.h>
#include <gdk/gdk.h>

#define UNUSED(x) (void)(x)

/*#define CLR_R(x)   (((x) & 0xff0000) >> 16)
#define CLR_G(x)   (((x) & 0x00ff00) >>  8)
#define CLR_B(x)   (((x) & 0x0000ff) >>  0)
#define CLR_16(x)  ((double)(x) / 0xff)
#define CLR_GDK(x) (const GdkRGBA){ .red = CLR_16(CLR_R(x)), \
                                    .green = CLR_16(CLR_G(x)), \
                                    .blue = CLR_16(CLR_B(x)), \
                                    .alpha = 0 }
*/
void create_terminal(         GtkWidget      *container, char* project_path);
static void on_terminal_get_entry(   VteTerminal    *terminal,     gchar      *text,      guint     size,      gpointer  user_data);
static void child_ready(             VteTerminal    *terminal,     GPid        pid,       GError   *error,     gpointer  user_data);

#endif