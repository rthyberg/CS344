#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

int shell();
char** get_arguments();
int free_get_arguments(char**);
int cd(char**);
int main() {
    shell();
    return 0;
}

int shell() {
     int status = 0;
     while(1) {
       printf(":");  // print prompt
       fflush(stdout);
       char** command = get_arguments(); // parse into an array of strings
      if(strcmp(command[0],"exit") == 0) {
           return 0;
       }
       if(strcmp(command[0],"status") == 0) {
           printf("%d\n", status);
           fflush(stdout);
           status = 0;
       }
       if(strcmp(command[0],"cd") == 0) {
           int ret = cd(command);
           if(ret == -1) {
               printf("need a valid path for cd\n");
               fflush(stdout);
               status = 1;
           }
           else {
               char my_cwd[2048];
               getcwd(my_cwd,2048);
               printf("%s\n", my_cwd);  // tester to show cwd
               status = 0;
           }
       }
       free_get_arguments(command);
    }
    return 0;
}

// function that will return an array of strings to be used as arguments
char** get_arguments() {
    size_t size = 2048;
    char* buffer;
    buffer = malloc(size);
    char** arguments = (char**)calloc(512, sizeof (char*)); // alloacate a new char**
    int i = 0;
    for(i = 0; i < 512; i++) {
        arguments[i] = calloc(size, sizeof (char));
    }
    getline(&buffer, &size, stdin);  // retrieve line
    fflush(stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // rid new line char
    char* token = strtok(buffer, " ");
    i = 0;
    while(token != NULL) {  // parse all tokens into new char**
        strcpy(arguments[i],token);
        token = strtok(NULL," ");
        i++;
    }
    free(buffer);;
    return arguments;
}

// Change directory
// modifiys dir to be a fullpath if it is not already
int cd(char** dir) {
    size_t size = 2048;
    char* currentdir = malloc(size);
    char* fullpath;
    getcwd(currentdir, size);
    // check if we are a full path or relative path
    if(dir[1][0] != '/') {
        if(dir[1][0] == '\0' || dir[1][0] == ' ') { // default to home dir
            fullpath = getenv("HOME");
        } else {
        strcat(currentdir, dir[1]); // concat into a full path
        fullpath = currentdir;
        }
   } else {
        fullpath = dir[1]; // else its a full path
    }
    int ret;
    if(fullpath != NULL) {
        ret = chdir(fullpath);
    } else {
       ret = -1;
    }
    free(currentdir);
    return ret;
}

int free_get_arguments(char** list) {
    int i;
    for(i=0; i < 512; i++) {
        free(list[i]);
    }
    free(list);
    return 0;
}
