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
     Room* edge[7];         // edge list of connections
     RoomType* rtype;        // enum for what kind of room it is

};
int generateFiles(int[][7]);
int createConnections();
Room* roomBuilder(FILE*);
FILE* myOpen(char*, char*, char*);
void print_graph(int[][7]);

int main() {
    srand(time(0)); // seed the rand function with current time
    createConnections();
//    generateFiles(pid);
    return 1;
}
/*
Room* buildRoom(char* input){
   Room *new_room =  (Room*)malloc(sizeof Room);
   for(i=0; i < 7; i++;) {
      new_room->edge[i] = NULL;
   }
   fp = myOpen(input, "r", "buildRoom");
   char buffer[100]
   while(fget 
}
*/
int generateFiles(int matrix[][7]) {
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
   int start = rand()%(7-0);
   int end = rand()%(7-0);
   int rooms_taken [7] = {-1,-1,-1,-1,-1,-1,-1};
   int taken = 0; // flag indicating if room has been used
   int count = -1;
   char list_of_rooms[7][100];
   // generate 6 rooms. count starts at -1
    while(count < 6) {
        int randomRoom = (int)(rand()%(10)+0);
        // check if taken
        for(i=0; i< 7; i++) {
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
        taken=0;
    }
        int base_con;
        int num_of_con;
        for(count=0; count < 7; count++) {
            FILE* current = myOpen(list_of_rooms[count], "a", "generateFiles()");
            num_of_con=0;
            for(base_con=0; base_con < 7; base_con++) {
                if(matrix[count][base_con] == 1) {
                    num_of_con++;
                    fprintf(current, "CONNECTION %d: %s\n", // print the connections to the file
                            num_of_con, room_name[rooms_taken[base_con]]);
                }
        // assign start and end to random which we declared earlier.
                if(base_con == 6) { // if we are done adding connections add type
                    if(count == start) {
                        fprintf(current, "ROOM TYPE:START_ROOM\n");
                    } else if(count == end) {
                        fprintf(current, "ROOM TYPE:END_ROOM\n");
                    } else {
                        fprintf(current, "ROOM TYPE:MID_ROOM\n");
                    }
                }
            }
            fclose(current);
        }
    return 0;
}

// function that will semi-randomly generate connections to write to our files
int createConnections() {
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

// random bonus connection
   while(maxConnections>0) {
       int rc=rand()%(7-0);
       int rc2=rand()%(7-0);
       if(connections[rc] < 7
         && connections[rc2] < 7
         && matrix[rc][rc2] == 0
         && matrix[rc2][rc] == 0
         && rc != rc2) {
           matrix[rc][rc2] = 1;
           matrix[rc2][rc] = 1;
           connections[rc]++;
           connections[rc2]++;
           maxConnections--;
       }
  }

    generateFiles(matrix);
    return 0;
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
    for(i=0; i < 7; i++) {
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
