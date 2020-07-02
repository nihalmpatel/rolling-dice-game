// ---------------------------------------------------------------------
//              CLIENT.C - CLIENT PROGRAM
// ---------------------------------------------------------------------

#include <fcntl.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>
#include <netinet/in.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/signal.h>
#include <sys/wait.h>
#include <arpa/inet.h>
#include <time.h>


int main(int argc, char *argv[])
{
    
    // Messege format sent by server
    char *startingMsg="You can now play";
    char *winnerMsg="Game over: you won the game";
    char *loserMsg="Game over:you lost the game";

    struct sockaddr_in servAdd;
    char message[255];
    int server, portNumber, n;
    socklen_t len;
    int dice;
    time_t t;

    // arg validation
    if(argc != 3)
    {
        printf("Call model: %s <IP> <Port #>\n", argv[0]);
        exit(0);
    }

    // create a socket
    if((server=socket(AF_INET, SOCK_STREAM, 0))<0)
    {
        fprintf(stderr, "Cannot create socket\n");
        exit(1);
    }

    // configure server socket address parameters
    servAdd.sin_family = AF_INET;
    sscanf(argv[2], "%d", &portNumber);
    servAdd.sin_port = htons((uint16_t)portNumber);

    if(inet_pton(AF_INET, argv[1],&servAdd.sin_addr)<0)
    {
        fprintf(stderr, " inet_pton() has failed\n");
        exit(2);
    }

    // establish connection to server
    if(connect(server, (struct sockaddr *) &servAdd,
               sizeof(servAdd))<0)
    {
        fprintf(stderr, "connect() has failed, exiting\n");
        exit(3);
    }


    // listing to server's messages
    while(1)
    {
        // clear message buffer
        memset(&message[0], 0, sizeof(message));

        // read message from server
        if(n=read(server, message, 255))
        {
            // append null at the end of message
            message[n]='\0';
            // dice score
            dice=0;
            srand ( time(&t) );
            sleep(1);

            // print message received from server
            fprintf(stderr, "MESSAGE RECEIVED: %s\n", message);

            // play dice message from server
            if(strcasecmp(message,startingMsg)==0)
            {
                dice = (int)rand()%10 + 1;
                printf("GOT %d POINTS\n",dice);
                int converted_number = htonl(dice);
                write(server, &converted_number, sizeof(converted_number));
            }

            // winning message from server
            else if(strcasecmp(message,winnerMsg )==0)
            {
                printf("\n I WON THE GAME \n");
                close(server);
                exit(0);
            }

            //  losing message from server
            else if(strcasecmp(message,loserMsg )==0)
            {
                printf("\n I LOST THE GAME \n");
                close(server);
                exit(0);
            }
        }
    }

}
