#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/wait.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>

int shell();
char** get_arguments();
int free_get_arguments(char**);
int cd(char**);
int createChild(char**);
char** createChar();
int main() {
    int ret = shell();
    return ret;
}

int shell() {
     int status = 0;
     while(1) {
       printf(":");  // print prompt
       fflush(stdout);
       char** command = get_arguments(); // parse into an array of strings
      if(strcmp(command[0],"exit") == 0) {
           if(command[1] != '\0') {
               return atoi(command[1]);
           } else {
               return 0;
           }
       } else if(strcmp(command[0],"status") == 0) { // get status of previous command
           printf("%d\n", status);
           fflush(stdout);
           status = 0;
       } else if(strcmp(command[0],"cd") == 0) {   // calls cd command if cd is found
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
               fflush(stdout);
               status = 0;
           }
       } else if(command[0][0] == '#' || command[0][0] =='\0') {
       } else {
           int ret = createChild(command);
           if(ret == 1) {
               printf("command not found\n");
               fflush(stdout);
           }
       }
       free_get_arguments(command);
    }
    return 0;
}

int createChild(char** argv) {
    pid_t spawnPid;
    int exitstatus = 0;
    char* outputfile;
    char* inputfile;
    int rdbool = 0;
    int redirect = 0;
    int fd = 0;;
    char** args = createChar();
    //int redirect = 0;
    int i;
    for(i = 0; i <512; i++) {
        if(strcmp(argv[i], ">") != 0 && strcmp(argv[i], "<") != 0 && strcmp(argv[i], "&") != 0) {
            strcpy(args[i], argv[i]);
        } else if(strcmp(argv[i], ">") == 0){
            if(i+1 > 512){
                return 1;
            }
            outputfile = argv[i+1];
            rdbool = 1;
        } else if(strcmp(argv[i], "<") == 0){
            if(i+1 > 512) {
                return 1;
            }
            inputfile = argv[i+1];
            rdbool = 2;
            printf("< test stub\n");
        } else if(strcmp(argv[i], "&") == 0){
            printf("& test stub\n");
        }
        if(strcmp(args[i], "\0") == 0) {
            free(args[i]);
            args[i] = NULL;
        }
    }
    pid_t cpid;
    spawnPid = fork();
    switch (spawnPid) {
        case -1:
            perror("Something bad happened");
            exit(1);
            break;
        case 0:
            if(rdbool == 1) {
                fd = open(outputfile, O_WRONLY | O_CREAT | O_TRUNC, 0644);
                redirect = dup2(fd, 1);
            } else if(rdbool == 2) {
                fd = open(inputfile, O_RDONLY);
                redirect = dup2(fd, 0);
                close(fd);
            }
            if(redirect != -1) {
                execvp(args[0], args);
                perror("exec failed");
                exit(1);
            }
        default:
            cpid = waitpid(spawnPid, &exitstatus, 0);
            if(cpid == -1) {
                perror ("wait failed\n");
                exit(1);
            }
            if(WIFEXITED(exitstatus)) {
                printf("exit normally\n");
                free_get_arguments(args);
                return 0;
            }
    }
    free_get_arguments(args);
    return 1;
}
// function that will return an array of strings to be used as arguments
char** get_arguments() {
    int i;
    size_t size = 2048;
    char* buffer;
    buffer = malloc(size);
    char** arguments = createChar();
    getline(&buffer, &size, stdin);  // retrieve line
    fflush(stdin);
    buffer[strcspn(buffer, "\n")] = 0;  // rid new line char
    char* token = strtok(buffer, " ");
    for(i=0; i < 512; i++) {  // parse all tokens into new char**
        if(token != NULL) {
            strcpy(arguments[i],token);
            token = strtok(NULL," ");
        } else {
            arguments[i][0] = '\0';
        }
    }
    free(buffer);;
    return arguments;
}

// Change directory
// modifiys dir to be a fullpath if it is not already
int cd(char** dir) {
    size_t size = 2048;
    char* currentdir = calloc(size, sizeof(char));
    char* fullpath;
    getcwd(currentdir, size);
    // check if we are a full path or relative path
    if(dir[1][0] != '/') {
        if(dir[1][0] == '\0' || dir[1][0] == ' ') { // default to home dir
            fullpath = getenv("HOME");
        } else {
        strcat(currentdir, "/");
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

char** createChar() {
    size_t size = 2048;
    char** arguments = (char**)calloc(513, sizeof (char*)); // alloacate a new char**
    int i = 0;
    for(i = 0; i < 512; i++) {
        arguments[i] = calloc(size, sizeof (char));
    }
    arguments[512] = NULL;
    return arguments;
}
int free_get_arguments(char** list) {
    int i;
    for(i=0; i < 512; i++) {
        if(list[i] != NULL) {
            free(list[i]);
        }
    }
    free(list);
    return 0;
}
