#include "project_handler.h"

void get_project_path(char** project_path) {
    FILE *fptr = NULL;

    fptr = fopen("../.urano/project_info.txt", "r");

    if (fptr == NULL) {
        printf("Error opening project info\n");
        
    } else {

        //printf("File open\n");
        *project_path = (char*)malloc(sizeof(char*));
        fscanf(fptr, "%s", *project_path);


        fclose(fptr);
    }
        

}




void  set_project_path(char* path_name) {
    FILE *fptr = NULL;

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