#include "utils.h"

void open_file(char* path){
    DIR* FD;
    FILE    *entry_buffer;
    FOLDER_CONTENTS contents;
    char** folders = NULL;
    char    buffer[BUFSIZ];
    struct dirent* in_file;
    struct stat sb;

    /* Scanning the in directory */
    if (NULL == (FD = opendir (path))) 
    {
        fprintf(stderr, "Error : Failed to open input directory\n");
    }

    while ((in_file = readdir(FD))) 
    {
        /* On linux/Unix we don't want current and parent directories
         * If you're on Windows machine remove this two lines
         */
        if (!strcmp (in_file->d_name, "."))
            continue;
        if (!strcmp (in_file->d_name, ".."))    
            continue;
        /* Open directory entry file for common operation */
        /* TODO : change permissions to meet your need! */
        entry_buffer = fopen(in_file->d_name, "r");
        if (entry_buffer == NULL)
        {
            fprintf(stderr, "Error : Failed to open entry file\n");
        }

        //printf("reading file %s\n", in_file->d_name);
        if (stat(in_file->d_name, &sb) == 0 && S_ISDIR(sb.st_mode)) {
            //printf("Is directory.\n");
            //*contents.folders = in_file->d_name;
            //push(*folders, in_file->d_name);
            //strcpy(*folders, in_file->d_name);
            //printf("folder: %s\n", *folders);
            //*folders++;

        } else {

            //*contents.files = in_file->d_name;
            //push(contents.files, in_file->d_name);
            //*contents.files++;

        }

        //push(contents.folders, in_file->d_name);

        fclose(entry_buffer);
    }

    //printf("size %ld\n", strlen(folders));
}

void push(char** arr, const char* val){

    while(*arr){

        *arr = (char*)malloc(strlen(val) + 1);
        *arr++;
    }

    for ( char *p = *arr; ( *p = *val ) != '\0'; ++p, ++val )
    {
        ;
    }
}