#ifndef __UTILS__
#define __UTILS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <glib.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>
#include <sys/types.h>

typedef struct path
{
  char* dir;
  char* path;
  int type;

  struct path *next, *previous, *branch;
}*PATH;

enum {
  FILE_TYPE,
  FOLDER_TYPE
};

int  filling_list(PATH* paths, char* file_name);
int  insertionSorted(PATH* node, PATH* branch, gchar* data, char* path, int type);
int  createNode(PATH *p, gchar* dato, char* path, gint type);
void walkList(PATH p, char* fn);

//void traverse(char *fn, int indent);


#endif