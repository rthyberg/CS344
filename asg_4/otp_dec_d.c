#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>

void error(const char *msg) { perror(msg); exit(1); } // Error function used for reporting issues
void decrypt(char*, char*, char*, int);

int main(int argc, char *argv[])
{
    int listenSocketFD, establishedConnectionFD, portNumber, charsRead;
    socklen_t sizeOfClientInfo;
    size_t max = 77000;
    char buffer[max];
    char key[max];
    struct sockaddr_in serverAddress, clientAddress;

    if (argc < 2) { fprintf(stderr,"USAGE: %s port\n", argv[0]); exit(1); } // Check usage & args

    // Set up the address struct for this process (the server)
    memset((char *)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[1]); // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET; // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber); // Store the port number
    serverAddress.sin_addr.s_addr = INADDR_ANY; // Any address is allowed for connection to this process

    // Set up the socket
    listenSocketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (listenSocketFD < 0) error("ERROR opening socket");

    // Enable the socket to begin listening
    if (bind(listenSocketFD, (struct sockaddr *)&serverAddress, sizeof(serverAddress)) < 0) // Connect socket to port
        error("ERROR on binding");
    listen(listenSocketFD, 5); // Flip the socket on - it can now receive up to 5 connections
    // multi
    while(1) {
        // Accept a connection, blocking if one is not available until one connects
        sizeOfClientInfo = sizeof(clientAddress); // Get the size of the address for the client that will connect
        establishedConnectionFD = accept(listenSocketFD, (struct sockaddr *)&clientAddress, &sizeOfClientInfo); // Accept
        if (establishedConnectionFD < 0) error("ERROR on accept");

        int pid = fork();

        if(pid == 0) {
            // Get the message from the client and display it
            memset(buffer, '\0', 77000);
            memset(key, '\0', 77000);
            charsRead = recv(establishedConnectionFD, buffer, 77000, 0); // Read the client's message from the socket
            if (charsRead < 0) error("ERROR reading from socket");

            if(strcmp(buffer, "otp_dec") != 0) {
                send(establishedConnectionFD, "otp_dec_d", 10, 0); // Send the name of server back
                close(establishedConnectionFD);
            } else {
                send(establishedConnectionFD, "otp_dec_d", 10, 0); // Send success back
                memset(buffer, '\0', 77000);
                charsRead = recv(establishedConnectionFD, buffer, 77000, 0); // Read the client's message from the socket
                if (charsRead < 0) error("ERROR reading from socket");
                send(establishedConnectionFD, "success", 8, 0); // Send success back
                if (charsRead < 0) error("ERROR writing to socket");

                memset(key, '\0', 77000);
                charsRead = recv(establishedConnectionFD, key, 77000, 0); // Read the client's message from the socket
                if (charsRead < 0) error("ERROR reading from socket");
                send(establishedConnectionFD, "success", 8, 0); // Send success back
                if (charsRead < 0) error("ERROR writing to socket");

                char* crypt = (char*)calloc(strlen(buffer)+1, sizeof(char));
                decrypt(buffer, key, crypt, strlen(buffer));
//                printf("crypt:%s\n", crypt);
                send(establishedConnectionFD, crypt, strlen(crypt),0);
                free(crypt);
                close(establishedConnectionFD); // Close the existing socket which is connected to the client
            }
        }
    }
    close(listenSocketFD); // Close the listening socket
    return 0;
}

/*
   int main(int argc, char** arcv) {
   char* word = "EQNVZ";
   char* key= "XMCKL";
   char* crypt = (char*)calloc(6, sizeof(char));
   decrypt(word, key, crypt, 5);
   printf("word:%s key:%s crypt:%s\n", word, key, crypt);
   return 0;
   }
*/
void decrypt(char* word, char* key, char* crypt, int length) {
    int i;
    int w = 0;
    int k = 0;
    int total = 0;
    for(i = 0; i < length; i++) {
        if(word[i] > 64 && word[i] < 91) {
            w = word[i] - 65;
        } else if(word[i] == 32) {
            w = 26;
        }
        if(key[i] > 64 && key[i] < 91) {
            k = key[i] - 65;
        } else if(key[i] == 32) {
            k = 26;
        }
        total = (w - k)+27;
       // if(total < 0) {
        //    total = (total + 27); // make positive
       // }
//        printf("w:%d k:%d\n", w,k);
        total = total%27;
            if(total == 26) {
                crypt[i] = 32;
            } else {
                crypt[i] = total + 65;
            }
    }
}
