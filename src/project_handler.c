#include "project_handler.h"

char* get_project_path() {
    FILE *fptr = NULL;
    char* project_path;

    fptr = fopen("../.urano/project_info.txt", "r");

    if (fptr == NULL) {
        printf("Error opening project info\n");
        
    } else {

        project_path = (char*)malloc(sizeof(char*)* 200);
        fscanf(fptr, "%s", project_path);
        fclose(fptr);

    }

    return project_path;
        
}


void  set_project_path(char* path_name) {
    FILE *fptr = NULL;

    /**this will change to home folder*/
    int result = mkdir("../.urano", 0777);
    fptr = fopen("../.urano/project_info.txt", "w");

    // exiting program 
    if (fptr == NULL) {
        printf("Error writing project info\n");
        
    } else {

        fprintf(fptr, "%s", path_name);
        fclose(fptr);

    }
        
}