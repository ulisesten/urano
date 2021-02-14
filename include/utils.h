#ifndef __UTILS__
#define __UTILS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
//#define _POSIX_SOURCE
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>
//#undef _POSIX_SOURCE

typedef struct {
    void** folders;
    void** files;
} FOLDER_CONTENTS;

typedef struct path
{
  gchar* dir;
  gchar* path;
  gint type;

  struct path *next, *branch;
}*PATH;

enum {
  FILE_TYPE,
  FOLDER_TYPE
};


void sorting_files(PATH* paths, GDir* contents, gchar* cwd);
void push(gchar* directory, PATH* sorted_files, gchar* aux);
int  filling_list(PATH* paths, char* file_name);
int  insertionSorted(PATH* node, gchar* data, int type);
int  createNode(PATH *p, gchar* dato, gint type);
void walkList(PATH p, char* fn);

void traverse(char *fn, int indent);


#endif