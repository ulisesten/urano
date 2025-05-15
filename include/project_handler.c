#include "project_handler.h"

/// Unused

char* get_project_path() {
    FILE *fptr = NULL;
    char* project_path;

    fptr = fopen(g_strconcat(g_get_user_data_dir (), "/urano/project_info.txt", NULL), "r");

    if (fptr == NULL) {
        printf("Error opening project info\n");
        
    } else {

        project_path = (char*)malloc(sizeof(char*)* 1024);
        fscanf(fptr, "%s", project_path);
        fclose(fptr);
        
    }

    return project_path;
        
}


void  set_project_path(char* path_name) {
    FILE *fptr = NULL;

    int result = mkdir(g_strconcat(g_get_user_data_dir (), "/urano", NULL), 0777);
    fptr = fopen(g_strconcat(g_get_user_data_dir (), "/urano/project_info.txt", NULL), "w");

    // exiting program 
    if (fptr == NULL) {
        printf("Error writing project info\n");
        
    } else {

        fprintf(fptr, "%s", path_name);
        fclose(fptr);

    }
        
}