#ifndef __FUNCTIONS__
#define __FUNCTIONS__

/**
 * @author Ulises Martínez Elías
 * @brief This file contains all the project logic functions
 * 
*/

#include <gtk/gtk.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <glib.h>
#include <string.h>
#include <sys/stat.h>

#include "file_tree_view.h"

char*  get_project_path();
void   set_project_path(char* path_name);

void configure_project(GtkWidget* tree_view, char* path);

/// Event Listeners
void on_quit_btn_clicked(          GtkWidget          *widget,    gpointer     data);
void on_toggle_search_clicked(     GtkToggleButton    *source,    gpointer     data);
void on_terminal_button_clicked(   GtkToggleButton    *widget,    gpointer     data);
void on_toggle_sidebar_clicked(    GtkToggleButton    *source,    gpointer     data);

void on_open_folder_button_clicked(GtkWidget* widget, gpointer data);

#endif //__FUNCTIONS__