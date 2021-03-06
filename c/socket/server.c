#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/select.h>
#include <arpa/inet.h>
#include <dirent.h>

#define BUFFER_SIZE 1024

#define PORTNUM 2001    // port to listen to
#define MAX_CLIENT_NUM 3    // maximum number of clients allowed to connect

int errorCheck(int retval, const char * message){
    if(retval == -1){
		perror(message);  
		exit(EXIT_FAILURE); 
	}
    return retval;
}


int main(){

    int serverSocket;
    struct sockaddr_in serverAddr, clientAddr;
    char buffer[BUFFER_SIZE];
    // create server socket
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    errorCheck(serverSocket , "Failed create server");

    // init address struct
    serverAddr.sin_family = AF_INET;
    serverAddr.sin_addr.s_addr = INADDR_ANY;
    serverAddr.sin_port = htons(PORTNUM);

    // binding
    errorCheck(bind(serverSocket, (struct sockaddr *)&serverAddr, sizeof(serverAddr)), 
                "Error binding");

    // listening
    errorCheck(listen(serverSocket, 2), "Error listening");
    printf("Server listening on port %d\n", PORTNUM);

    // // accept connections
    int addr_len = sizeof(clientAddr);
    char helloMsg[BUFFER_SIZE] = "Server: welcome, you are now connected\n";
    int connectedClientNum = 0;
    int new_socket;

    // using select to handle multi-connection
    fd_set masterSet, readySet;
    FD_ZERO(&masterSet); // clear current socket set
    FD_SET(serverSocket, &masterSet); // add serverSocket to current socket set

    int fdmax = serverSocket;
    int nbytes;

    int rejectedFd;
    char * rejectMsg = "Server is full\n";
    memset(buffer, 0, BUFFER_SIZE); // clear buffer

    while(1){
        readySet = masterSet;
        memset(buffer, 0, BUFFER_SIZE); // clear buffer

        // using select() to check for fd ready to read
        if(select(fdmax+1, &readySet, NULL, NULL, NULL) < 0 ){
            perror("Error in select()");
            exit(EXIT_FAILURE);
        }
        
        for(int i=0; i<= fdmax; i++){
            if(FD_ISSET(i, &readySet)){ // check for ready fd 
                if(i == serverSocket){ // if it's serverSocket fd, then it's a new connection
                    if(connectedClientNum < MAX_CLIENT_NUM){
                        // accept new connection if less than 2 clients connected
                        printf("New connection\n");
                        new_socket = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_len);
                        errorCheck(new_socket, "Error accepting connection");
                        FD_SET(new_socket, &masterSet); // add new client socket to current socket set
                        if (new_socket > fdmax){
                            fdmax = new_socket;
                        }
                        // send helle message
                        send(new_socket, helloMsg, strlen(helloMsg)+1, 0 );
                        connectedClientNum++;
                        
                    } else {
                        // maximum client connected, reject new connection
                        printf("New connection but server is full\n");
                        rejectedFd = accept(serverSocket, (struct sockaddr *)&clientAddr, &addr_len);
                        send(rejectedFd, rejectMsg, strlen(rejectMsg)+1, 0 );
                        close(rejectedFd);
                    }
                }   // end accept new connection 
                
                else { // if it's not serverSocket fd, then it's client fd
                    // read data from a client and put into buffer
                    if ((nbytes = recv(i, buffer, sizeof(buffer), 0)) <= 0) {
                        // got error or connection closed by client
                        if (nbytes == 0) {
                            // connection closed
                            printf("INFO: client at fd %d closed\n", i);
                        } else {
                            perror("recv");
                        }
                        close(i); // bye!
                        connectedClientNum--;
                        FD_CLR(i, &masterSet); // remove from master set
                    } else {
                        // check client's command
                        if (strncmp("/close", buffer, 6) == 0){
                            // close connection
                            printf("INFO: client at fd %d closed\n", i);
                            close(i); // close fd
                            connectedClientNum--;
                            FD_CLR(i, &masterSet); // remove from master set
                        }
                        else { 
                            // normal message
                            char msg[BUFFER_SIZE] = {0};
                            printf(msg, "From client (%d): ", i);
                            strcat(msg, buffer);
                            for(int j = 0; j <= fdmax; j++) {
                                // send (forward) msg to other client
                                if (FD_ISSET(j, &masterSet)) {
                                    // exclude the server listener and sender
                                    if (j != serverSocket && j != i) {
                                        if (send(j, msg, strlen(msg)+1, 0) == -1) {
                                            perror("send");
                                        }
                                    }
                                }
                            }
                        }
                    }
                }   // end handle client's message
            }   // end check for ready fd
        }   // end for loop through fds
    }   // end while(1)
    close(serverSocket);
    return 0;
}
