#include "utils.h"


void sorting_files(PATH* paths, GDir* contents, gchar* cwd){
    gchar* aux;
    gchar* dir = (gchar*)g_dir_read_name(contents);
    if(dir){

        push(dir, paths, aux);
        sorting_files(&(*paths)->next, contents, aux);
    }
}

void push(gchar* directory, PATH* sorted_files, gchar* aux) {

    *sorted_files = (PATH)malloc(sizeof(struct path));

    if (*sorted_files) {
        struct stat sb;

        if(stat(directory, &sb) == 0 && S_ISDIR(sb.st_mode)){
            (*sorted_files)->type = FOLDER_TYPE;
        } else {
            (*sorted_files)->type = FILE_TYPE;
        }

        (*sorted_files)->dir = directory;
        (*sorted_files)->next = NULL;

    }

}

int filling_list(PATH* paths, char* file_name){

    DIR* dir;
    struct dirent* entry;
    struct stat sb;
    //char* path;
    char path[1025];
    int res = -1;

    if( (dir = opendir(file_name)) == NULL)
        perror("opendir() error");
    else {

        while((entry = readdir(dir)) != NULL ){

            if(strcmp (entry->d_name, "..") == 0)
                continue;

            if (entry->d_name[0] == '.')
                continue;

            strcpy(path, file_name);
            strcat(path, "/");
            strcat(path, entry->d_name);

            if (stat(path, &sb) != 0) 
                fprintf(stderr, "stat() error on %s: %s\n", path, strerror(errno));
            else {
                if( S_ISDIR(sb.st_mode) ){

                    res = insertionSorted(paths, entry->d_name, FOLDER_TYPE);
                    if(res < 0){

                        printf("filling_list(): error al insertar elemento folder: %s.\n", entry->d_name);

                    } else {

                        filling_list( &(*paths)->branch, path );
                        
                    }

                } else {

                    res = insertionSorted(paths, entry->d_name, FILE_TYPE);
                    if( res < 0 ){
                        printf("filling_list(): error al insertar elemento file: %s.\n", entry->d_name);
                    }

                }
            }
            

        }
        
        closedir(dir);

    }

    return res;
}

int insertionSorted(PATH* node, gchar* data, int type){
    PATH aux = *node;
    PATH last = *node;
    PATH newNode;
    int res = -1;

    while( aux ){

        if(last->type == FOLDER_TYPE && type == FOLDER_TYPE || aux->type == FILE_TYPE) break;
        last = aux;
        aux = aux->next;

    }

    res = createNode(&newNode, data, type);

    if(res == 0)
        if( aux == *node || !*node){

            newNode->next = aux;
            *node = newNode;
            res = 0;

        } else {
            newNode->next = aux;
            last->next = newNode;
            res = 0;
        }
    else
        printf("error al crear nodo.\n");

    return res;

}

int createNode(PATH *p, gchar* data, gint type){
    int res = -1;

    *p = (PATH) malloc(sizeof(struct path));
    if(*p){

        res = 0;
        (*p)->dir = data;
        (*p)->type = type;
        (*p)->next = NULL;
        (*p)->branch = NULL;

    }

    return res;

}

void walkList(PATH list, char* fn){
    char path[1025];

    while(list) {
        strcpy(path, fn);
        strcat(path, "/");
        strcat(path, list->dir);

        printf("%s, type: %d\n", path, list->type);
        if(list->branch)
            walkList(list->branch, path);
        list = list->next;
    }

}


void traverse(char *fn, int indent) {
  DIR *dir;
  struct dirent *entry;
  int count;
  char path[1025];
  struct stat info;

  for (count=0; count<indent; count++) printf("  ");
  printf("%s\n", fn);

  if ((dir = opendir(fn)) == NULL)
    perror("opendir() error");
  else {
    while ((entry = readdir(dir)) != NULL) {
      if (entry->d_name[0] != '.') {
        strcpy(path, fn);
        strcat(path, "/");
        strcat(path, entry->d_name);
        if (stat(path, &info) != 0)
          fprintf(stderr, "stat() error on %s: %s\n", path,
                  strerror(errno));
        else if (S_ISDIR(info.st_mode))
               traverse(path, indent+1);
      }
    }
    closedir(dir);
  }
}