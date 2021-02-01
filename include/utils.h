#ifndef __UTILS__
#define __UTILS__

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <sys/stat.h>

typedef struct {
    void** folders;
    void** files;
} FOLDER_CONTENTS;

void open_file(char* path);
void push(char** arr, const char* val);

#endif