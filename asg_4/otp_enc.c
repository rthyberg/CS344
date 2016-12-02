#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>

void error(const char *msg) { perror(msg); exit(0); } // Error function used for reporting issues
int fileToString(char*, FILE*);
int main(int argc, char *argv[])
{
    int socketFD, portNumber, charsWritten;
    struct sockaddr_in serverAddress;
    struct hostent* serverHostInfo;
    char* buffer;
    char* key;
    char check[255];
    char* data;

    if (argc < 4) { fprintf(stderr,"USAGE: %s inputfile key port\n", argv[0]); exit(0); } // Check usage & args

    // Set up the server address struct
    memset((char*)&serverAddress, '\0', sizeof(serverAddress)); // Clear out the address struct
    portNumber = atoi(argv[3]); // Get the port number, convert to an integer from a string
    serverAddress.sin_family = AF_INET; // Create a network-capable socket
    serverAddress.sin_port = htons(portNumber); // Store the port number
    serverHostInfo = gethostbyname("localhost"); // Convert the machine name into a special form of address
    if (serverHostInfo == NULL) { fprintf(stderr, "CLIENT: ERROR, no such host\n"); exit(2); }
    memcpy((char*)&serverAddress.sin_addr.s_addr, (char*)serverHostInfo->h_addr, serverHostInfo->h_length); // Copy in the address

    // parsefile into char*
    FILE* file = fopen(argv[1], "r");
    fseek(file, 0, SEEK_END);
    int fsize = ftell(file);
    FILE* file2 = fopen(argv[2], "r");
    fseek(file2, 0, SEEK_END);
    int fsize2 = ftell(file2);
    buffer = malloc(fsize);
    key = malloc(fsize2);
    data = malloc(fsize);
    int valid= fileToString(buffer, file);
    if(valid == 1 ) {
        fprintf(stderr, "invalid charcters in file\n");
        free(buffer);
        free(key);
        free(data);
        exit(1);
    }
    valid = fileToString(key, file2);
    if(valid == 1) {
        fprintf(stderr, "Invalid charcters in key\n");
        free(buffer);
        free(key);
        free(data);
        exit(1);
    }
    if(strlen(buffer) > strlen(key)) {
        fprintf(stderr, "Key is too small\n");
        free(buffer);
        free(key);
        free(data);
        exit(1);
    }


    // Set up the socket
    socketFD = socket(AF_INET, SOCK_STREAM, 0); // Create the socket
    if (socketFD < 0) {
        error("CLIENT: ERROR opening socket");
        free(buffer);
        free(key);
        free(data);
        exit(2);
    }

    // Connect to server
    if (connect(socketFD, (struct sockaddr*)&serverAddress, sizeof(serverAddress)) < 0) {// Connect socket to address
        error("CLIENT: ERROR connecting");
        free(buffer);
        free(key);
        free(data);
        exit(2);
    }
    // handshake
    charsWritten = send(socketFD, "otp_enc", 8, 0); // Write to the server

    if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(check)) fprintf(stderr, "CLIENT: WARNING: Not all data written to socket!\n");

    memset(check, '\0', 255);
    recv(socketFD, check, 255-1, 0);
    if(strcmp(check, "otp_enc_d") != 0) {
        fprintf(stderr, "This program can only access otp_enc_d.. got:%s\n",check);
        free(buffer);
        free(key);
        free(data);
        exit(2);
    }


    // Send message to server
    charsWritten = send(socketFD, buffer, strlen(buffer), 0); // Write to the server
    if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(buffer)) fprintf(stderr, "CLIENT: WARNING: Not all data written to socket!\n");
    recv(socketFD, check, 255-1, 0);
    if(strcmp(check, "success") != 0) {
        return 1;
    }
    //   printf("received:%s\n",check);

    charsWritten = send(socketFD, key, strlen(key), 0); // Write to the server
    if (charsWritten < 0) error("CLIENT: ERROR writing to socket");
    if (charsWritten < strlen(key)) fprintf(stderr, "CLIENT: WARNING: Not all data written to socket!\n");
    recv(socketFD, check, 255-1, 0);
    if(strcmp(check, "success") != 0) {
        return 1;
    }
    //    printf("received:%s\n",check);
    recv(socketFD, data, 77000, 0);
    printf("%s\n", data);

    close(socketFD); // Close the socket
    return 0;
}

// Remove newlines and push file into the buffer
int fileToString(char* word, FILE* fp) {
    rewind(fp);
    char next = fgetc(fp);
    int i = 0;
    while(next != EOF) {
        if(next == '\n') {
            // dont do anything
        } else if(next== ' ' || (next >= 'A' && next <= 'Z'))  {
            word[i] = next; // add char to buffer
        } else {
            return 1;
        }
        i++;
        next = fgetc(fp); // increment while loop
    }
    word[i-1]='\0';
    return 0;
}
