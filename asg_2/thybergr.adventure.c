#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

typedef enum RoomType RoomType;
typedef struct Room Room;

// enum will add clarity to what type of room is being defined
enum RoomType {START_ROOM, END_ROOM, MID_ROOM};

struct Room {
     char* name;             // room name
     Room** connect;         // edge list of connections
     RoomType* rtype;        // enum for what kind of room it is

};
int generateFiles(char*);
int createRooms();
int cleanUp(char*, char**);

int main() {
    srand(time(0)); // seed the rand function with current time
    char pid [50];
    int getPid = getpid(); // get the current process id
    sprintf(pid, "%d", getPid); // convert it to a string
    generateFiles(pid);
    return 1;
}

int generateFiles(char* id) {
    // List of hardcoded rooms
    char room_name [10][50] =  {"pool", "kitchen", "backyard", "basement", "hallway",
                        "bathroom", "patio", "sauna", "garage", "driveway"};
    // creates dir with pid
    char dir_name[75] = "thybergr.rooms.";
    strcat(dir_name, id);
    mkdir(dir_name, S_IRWXU | S_IRWXG | S_IRWXO);
   // initialise vars
   int i;
   int rooms_taken [7] = {-1,-1,-1,-1,-1,-1,-1};
   int taken = 0;
   int count = -1;
   char* list_of_rooms[7];
   // generate 6 rooms. count starts at -1
    while (count <6) {
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
            list_of_rooms[count]= room_name[randomRoom];
            char file_name[500];
            sprintf(file_name, "%s/%s", dir_name, room_name[randomRoom]);
            FILE* current = fopen(file_name, "w");
            fclose(current);
        }
        taken = 0;
    }
    return 0;
}

int cleanUp(char* id, char** list) {
    int i;
    for(i = 0; i <7; i++) {
        remove(list[i]);
    }
    return 0;
}
