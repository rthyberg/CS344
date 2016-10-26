#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>
#include <errno.h>

typedef enum RoomType RoomType;
typedef struct Room Room;
extern int errno;

// enum will add clarity to what type of room is being defined
enum RoomType {START_ROOM, END_ROOM, MID_ROOM};

struct Room {
     char* name;             // room name
     Room** edge;         // edge list of connections
     RoomType rtype;        // enum for what kind of room it is

};
// Function declarations
char**  generateFiles(int[][7]);
char** createConnections();
Room* buildRoom(char*);
int freeRoom(Room* cell);
void makeEdgesRoom(Room**, char**);
void gameLoop(Room**);
void printGameRoom(Room*);
int getInputGame(char*, Room*);
FILE* myOpen(char*, char*, char*);
void print_graph(int[][7]);
void print_room(Room*);

int main() {
    srand(time(0)); // seed the rand function with current time
    char** files = createConnections();
    int i;
    Room* room_list[7];
    for(i = 0; i<7; i++) {   // build the rooms
        room_list[i] = buildRoom(files[i]);
    }
    makeEdgesRoom(room_list, files); // build the game edges and roomss
    gameLoop(room_list); // starts the game
    for(i = 0; i <7; i++) {
        freeRoom(room_list[i]);  // free everything
        free(files[i]);
    }
    return 0;
}

void gameLoop(Room** cells) {
    int i = 0;
    Room* current=NULL;
    char path[100][100]; // really hope it doesnt take you a hundred tries
    int path_count = 0;
    // find the starting room before the game begins
    int end_room = 0;
    for(i = 0; i < 7; i++) {
        if(cells[i]->rtype == START_ROOM) {
            current = cells[i];
        }
        if(cells[i]->rtype == END_ROOM) {
            end_room = 1;
        }
    }
    if(end_room == 0) {  // extra precautions to make sure we have an end room
        fprintf(stderr, "No end room was found");
        exit(1);
    }
    while(current->rtype != END_ROOM) {  // while we arnt at the end
        int valid_room = -1; // hold boolean for if we chosee a valid room
        while(valid_room == -1) {
            printGameRoom(current);  // calls helper function print room
            char buffer[100];
            valid_room = getInputGame(buffer, current);  // calls helper function get input
            if(valid_room == -1) {
                printf("\nHUH? I DON'T UNDERSTAND THAT ROOM, TRY AGAIN.\n\n");
            } else {
                current = current->edge[valid_room]; // set current room to the edge in the list index by valid_room
                strcpy(path[path_count], current->name); // add to path
                path_count++; // update our path count
                printf("\n");
            }
        }
    }
    printf("YOU HAVE FOUND THE END ROOM. CONGRATULATIONS!\n"); // victory
    printf("YOU TOOK %d STEPS. YOUR PATH TO VICTORY WAS:\n", path_count);
    for(i = 0; i < path_count + 1; i++) {
        printf("%s\n", path[i]);
    }
}

// print current rooms according to required format
void printGameRoom(Room* cell) {
    printf("CURRENT LOCATION: %s\n", cell->name);
    printf("POSSIBLE CONNECTIONS: ");
    int i = 0;
    while(cell->edge[i] != NULL) {
        if(cell->edge[i+1] == NULL) { // if last element use a .
            printf("%s.\n",cell->edge[i]->name);
        } else {
            printf("%s, ", cell->edge[i]->name); // print the list
        }
        i++;
    }
    printf("WHERE TO? >");
 }

// abstract gettin user input to give more clarity to game loop
// return value will be where in the edgelist the input is or -1
int getInputGame(char* buffer, Room* cell) {
    int valid = -1; // return value whether the input is in the cell edgelist
    int i = 0;
    fgets(buffer, 100, stdin); // get input form user
    buffer[strcspn(buffer, "\n")] = 0; //set the \n char to null byte;
    while(cell->edge[i] != NULL) { // go through edgelist comparing buffer
        if(strcmp(buffer, cell->edge[i]->name) == 0) {
            valid = i;
            break; // if found we can break early
        }
        i++;
    }
    return valid;
}

// open file and create edge list connections to other Rooms
void makeEdgesRoom(Room** cells, char** filenames) {
    int i;
    for(i = 0; i < 7; i++) {
        FILE* fp = myOpen(filenames[i], "r", "makeEdgesRoom"); // open file
        char* buffer = NULL;
        size_t len = 0;
        int cflag = 0;  // true if next line has the connection
        int count = 0;
        while(getdelim(&buffer, &len, ':', fp) != -1) { // find the connections line
           if(cflag == 1) {
                char cpy[100];
                strcpy(cpy, buffer);
                char* token = strtok(cpy, "\n");
                int j;
                for(j=0; j<7; j++) { //find the cell pointer correlated to that connections
                    if(!(strstr(token, cells[j]->name)==0)) {
                       cells[i]->edge[count] = cells[j]; // push onto the array
                       count++;
                   }
                }
           }
           if(strstr(buffer,"CONNECTION")) { //if found the next line will contain a connection
                    cflag = 1;
           }
        }
        fclose(fp);
    }
}

// Builds a single room struct from string file name
Room* buildRoom(char* input){
   // allocate space for a room
   Room *new_room = (Room*)malloc(sizeof(Room));
   new_room->name = (char*)malloc(50*sizeof(char));
   new_room->edge = (Room**)malloc(7*sizeof(Room*));
   int i;
   // set the array to null
   for(i=0; i < 7; i++) {
      new_room->edge[i] = NULL;
   }
   FILE* fp = myOpen(input, "r", "buildRoom");
   char* buffer = NULL;
   size_t len = 0;
   int nflag = 0;
   int tflag = 0;
   while(getdelim(&buffer, &len, ':', fp) != -1) {
      if(nflag == 1) { // if name is found add it to the room
           char cpy[100];
           strcpy(cpy, buffer);
           char* token = strtok(cpy, "\n");
           strcpy(new_room->name, token);
           nflag = 0;
       } else if(tflag == 1) { // if type is found add it to the room
           if(strstr(buffer, "START_ROOM")) {
               new_room->rtype = START_ROOM;
           } else if(strstr(buffer, "MID_ROOM")) {
               new_room->rtype = MID_ROOM;
           } else if(strstr(buffer, "END_ROOM")) {
               new_room->rtype = END_ROOM;
           }
           tflag = 0;
       } else if(strstr(buffer,"ROOM NAME")) { // checks to see if next line conatins a room name
           nflag = 1;
       } else if(strstr(buffer,"ROOM TYPE")) { // check to see if next line contains a type
           tflag = 1;
       }
   }
   free(buffer);
   fclose(fp);
   return new_room;
}

char** generateFiles(int matrix[][7]) {
    // List of hardcoded rooms
    char id [50];
    int getPid = getpid(); // get the current process id
    sprintf(id, "%d", getPid); // convert it to a string
    char room_name [10][50] =  {"pool", "kitchen", "backyard", "basement", "hallway",
                                  "bathroom", "patio", "sauna", "garage", "driveway"};
    // creates dir with pid
    char dir_name[75] = "thybergr.rooms.";
    strcat(dir_name, id);
    mkdir(dir_name, S_IRWXU | S_IRWXG | S_IRWXO);
   // initialise vars
   int i;
   int start = 0;
   int end = 0;
   while(start == end) { // avoid case where start and end are the same
       start = rand()%(7-0);
       end = rand()%(7-0);
   }
   int rooms_taken [7] = {-1,-1,-1,-1,-1,-1,-1};
   int taken = 0; // flag indicating if room has been used
   int count = -1;
   char** list_of_rooms=(char**)malloc(7*sizeof(char*));
   for(i=0; i <7; i++) {
       list_of_rooms[i]=(char*)malloc(100*sizeof(char));
   }
   // generate 6 rooms. count starts at -1
    while(count < 6) {
        int randomRoom = (int)(rand()%(10)+0);
        // check if taken
        for(i = 0; i< 7; i++) {
            if(randomRoom == rooms_taken[i]) {
                taken = 1;
            }
        }
        // not taken create room file
        if(!taken) {
            count++;
            rooms_taken[count] = randomRoom;
            char file_name[500];
            sprintf(file_name, "%s/%s", dir_name, room_name[randomRoom]); // inputs the dir/roomname to be created
            FILE* current = myOpen(file_name, "w", "generateFiles()");
            fprintf(current, "ROOM NAME:%s\n", room_name[randomRoom]);
            fclose(current);
            strcpy(list_of_rooms[count], file_name); // keep a list of our path names for later use
        }
        taken = 0;
    }
        int base_con;
        int num_of_con;
        for(count = 0; count < 7; count++) {
            FILE* current = myOpen(list_of_rooms[count], "a", "generateFiles()");
            num_of_con = 0;
            for(base_con = 0; base_con < 7; base_con++) {
                if(matrix[count][base_con] == 1) {
                    num_of_con++;
                    fprintf(current, "CONNECTION %d: %s\n", // print the connections to the file
                            num_of_con, room_name[rooms_taken[base_con]]);
                }
        // assign start and end to random which we declared earlier.
                if(base_con == 6) { // if we are done adding connections add type
                    if(count == start) {
                        fprintf(current, "ROOM TYPE: START_ROOM\n");
                    } else if(count == end) {
                        fprintf(current, "ROOM TYPE: END_ROOM\n");
                    } else {
                        fprintf(current, "ROOM TYPE: MID_ROOM\n");
                    }
                }
            }
            fclose(current);
        }
    return list_of_rooms;
}

// function that will semi-randomly generate connections to write to our files
char** createConnections() {
    // Current connections is odd 7*3=21. Must be even.
    // hard code one as 4 connections so we have 22 connections
    // we have 11 edges must maintain odd number of edges
    // Max Additional connections is ((7*3)-1)/2 =10  10 +11 = 21 connetions
    int maxConnections = (rand()%(11-0));
    int connections[7] = {3,3,3,3,3,4,3}; // currently all rooms can connect 3 ways
   // Since files are created randomly can assume these paths are random with at
   // least 3 connections. One must have 4 by vertices=2*edges
   int matrix [7][7]={{0,0,0,1,1,1,0}, //1
                        {0,0,0,1,0,1,1}, //2
                        {0,0,0,1,0,1,1}, //3
                        {1,1,1,0,0,0,0}, //4
                        {1,0,0,0,0,1,1}, //5
                        {1,1,1,0,1,0,0}, //6
                        {0,1,1,0,1,0,0}}; //7

// random bonus connections
   while(maxConnections>0) {
       int rc = rand()%(7-0); // get 2 rng numbers
       int rc2 = rand()%(7-0);
       if(connections[rc] < 7
         && connections[rc2] < 7
         && matrix[rc][rc2] == 0
         && matrix[rc2][rc] == 0
         && rc != rc2) { // if this is a valid connection
           matrix[rc][rc2] = 1;  // make connection
           matrix[rc2][rc] = 1;
           connections[rc]++;
           connections[rc2]++;
           maxConnections--;
       }
  }

    char** result = generateFiles(matrix);
    return result;
}

// abstract away file opening with error checking to make other errors more clear
FILE* myOpen(char* path, char* mode, char* function) {
    FILE* current=fopen(path, mode);
    if(current == NULL) {
        fprintf(stderr, "%s:%s\n", function, strerror(errno));
        exit(1);
    }
    return current;
}

// For visual debugging
void print_graph(int graph [7][7]) {
    int i;
    int count=0;;
    for(i = 0; i < 7; i++) {
        int j;
        printf("Node {%d}: ", i);
        for(j=0; j< 7; j++) {
            if(graph[i][j] == 1) {
                count++;
            }
            printf("[%d] ", graph[i][j]);
        }
        printf("\n");
    }
        printf("# of Vertex: %d\n # of Edges: %d\n", count, (count/2));
}

// print room struct
void print_room(Room* cell) {
    printf("ROOM NAME: %s\n", cell->name);
    int i = 0;
    while(cell->edge[i] != NULL) {
        printf("CONNECTION %d: %s\n", i, cell->edge[i]->name);
        i++;
    }
    if((cell->rtype) == START_ROOM) {
        printf("ROOM TYPE: %s\n","START_ROOM" );
    } else if((cell->rtype) == MID_ROOM) {
        printf("ROOM TYPE: %s\n","MID_ROOM" );
    } else if((cell->rtype) == END_ROOM) {
        printf("ROOM TYPE: %s\n","END_ROOM" );
    }
}
// frees Room
int freeRoom(Room* cell) {
    int i=0;
    // Since we are gonna free all nodes we dont free the pointers in the array here
    while(cell->edge[i] != NULL) {
        cell->edge[i]=NULL; // dont set free as we will be setting this free through the loop
        i++;
    }
    free(cell->edge);
    free(cell->name);
    cell->name = NULL;
    free(cell);
    cell=NULL;
    return 0;
}

