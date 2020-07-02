// ---------------------------------------------------------------------
//              SERVER.C - SERVER PROGRAM
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

#define WINNING_SCORE 100

void servicePlayers(int,int );

int main(int argc, char *argv[])
{
    int sd,portNumber;
    // client room
    int client[2]= {0,0};
    socklen_t len;
    // current waiting client
    int client_count=0;

    struct sockaddr_in servAdd;

    // arg validation
    if(argc != 2)
    {
        printf("Call model: %s <Port #>\n", argv[0]);
        exit(0);
    }

    // create server socket
    if ((sd=socket(AF_INET,SOCK_STREAM,0))<0)
    {
        fprintf(stderr, "Cannot create socket\n");
        exit(1);
    }

    // configure socket parameters
    servAdd.sin_family = AF_INET;
    servAdd.sin_addr.s_addr = htonl(INADDR_ANY);
    sscanf(argv[1], "%d", &portNumber);
    servAdd.sin_port = htons((uint16_t)portNumber);
    bind(sd,(struct sockaddr*)&servAdd,sizeof(servAdd));
    listen(sd, 5);

    // waiting for connections
    while(1)
    {
        client[client_count]=accept(sd,(struct sockaddr*)NULL,NULL);
        client_count++;

        printf("\nCLIENT %d CONNECTED.\n", client_count);

        // fork a process when a room gets two client
        if(client_count==2)
        {
            // reset the waiting clients variable
            client_count=0;
            printf("--------------------------------------------------------------------\n");
            printf("\tROOM CREATED WITH 2 CLIENTS. FORKING THE CHILD PROCESS.\n");
            printf("--------------------------------------------------------------------\n");

            // Client handler child process
            if(!fork()) 
                servicePlayers(client[0],client[1]);
        }
    }
}

void servicePlayers(int TOTO, int TITI)
{
    // message format
    char *startingMsg="You can now play";
    char *winnerMsg="Game over: you won the game";
    char *loserMsg="Game over:you lost the game";

    // client room of 2 players
    int playerArry[]= {TOTO, TITI};
    char *playerNames[]= {"TOTO","TITI"};
    int n;

    // scoreboard
    int playerScores[2]= {0,0};

    // act as a refree between two clients in a room
    while(1)
    {
        // select players one by one
        for(int i=0; i<2; i++)
        {
            // send message to play
            write(playerArry[i],startingMsg, strlen(startingMsg)+1);
            int received_int = 0;

            if(n=read(playerArry[i], &received_int, sizeof(received_int)))
            {
                // read score returned from client and add to total score
                playerScores[i] += ntohl(received_int);

                // printf("TOTAL SCORE OF %s: %d\n", playerNames[i], playerScores[i]);

                // End the game if a client reaches the WINNING SCORE
                if(playerScores[i] >= WINNING_SCORE)
                {
                    // Send winning message 
                    printf("--------------------------------------------------------------------\n");
                    printf("\tMATCH OVER! HERE IS THE RESULT:\n");
                    printf("\tWINNER: %s WITH TOTAL SCORE: %d\n", playerNames[i], playerScores[i]);
                    printf("--------------------------------------------------------------------\n");
                    write(playerArry[i], winnerMsg, strlen(winnerMsg)+1);
                    // Send losing message
                    write(playerArry[((i+1)%2)], loserMsg, strlen(loserMsg)+1);

                    // end the connection
                    close(TOTO);
                    close(TITI);

                    exit(0);
                }
            }
        }
    }

}
