#include "utils.h"


int filling_list(PATH* paths, char* file_name){

    DIR* dir;
    struct dirent* entry;
    struct stat sb;
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

            //printf("entry: %s\n", path);

            if (stat(path, &sb) != 0) 
                fprintf(stderr, "stat() error on %s: %s\n", path, strerror(errno));
            else {
                if( S_ISDIR(sb.st_mode) ) {
                    
                    PATH branched = NULL;
                    res = insertionSorted(paths, &branched, g_path_get_basename(path), g_path_get_dirname (path), FOLDER_TYPE);
                    if(res < 0){

                        printf("filling_list(): error while inserting folder: %s.\n", entry->d_name);

                    } else {

                        filling_list( &(branched)->branch, path );
                        
                    }

                } else {

                    res = insertionSorted(paths, NULL, g_path_get_basename(path), g_path_get_dirname (path), FILE_TYPE);
                    if( res < 0 ){
                        printf("filling_list(): error while inserting file: %s.\n", entry->d_name);
                    }

                }

            }
            
        }
        
        closedir(dir);

    }

    return res;
}

int insertionSorted(PATH* node, PATH* branched, gchar* data, char* path, int type){
    PATH aux = *node;
    PATH last = *node;
    PATH newNode;
    int res = -1;

    while( aux ){

        if(last->type == FOLDER_TYPE && type == FOLDER_TYPE || aux->type == FILE_TYPE) break;
        last = aux;
        aux = aux->next;

    }

    res = createNode(&newNode, data, path, type);

    if(res == 0){
        if( aux == *node || !*node){

            newNode->next = aux;
            *node = newNode;
            res = 0;
            
        } else {

            newNode->next = aux;
            last->next = newNode;
            res = 0;
            
        }

        if(type == FOLDER_TYPE)
            *branched = newNode;

    } else {
        printf("createNode returned non 0 code!.\n");
    }

    return res;

}

int createNode(PATH *p, gchar* data, char* path, gint type){
    int res = -1;

    *p = (PATH) malloc(sizeof(struct path));
    if(*p){

        res = 0;
        (*p)->dir = data;
        (*p)->path = path;
        (*p)->type = type;
        (*p)->previous = NULL;
        (*p)->next = NULL;
        (*p)->branch = NULL;

    } else {

        printf("Error while allocating mem!\n");

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


/*void traverse(char *fn, int indent) {
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
}*/