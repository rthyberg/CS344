#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef enum RoomType RoomType;
typedef struct Room Room;

// enum will add clarity to what type of room is being defined
enum RoomType {START_ROOM, END_ROOM, MID_ROOM};

struct Room {
     char* name;             // room name
     Room** connect;         // edge list of connections
     RoomType* rtype;        // enum for what kind of room it is

};
int generateFiles();
int createRooms();

int main() {
    generateFiles();
    //srand();
    return 1;
}

int generateFiles() {
    return 0;
}
