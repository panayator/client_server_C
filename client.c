#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h> 
#include <pthread.h>

#define SIZE 256
#define MAX 1024


int main (int argc, char **argv){

    int serverfd, port, in, out, socketfd;
    struct sockaddr_in address;
    int loc, bed, i,x, j, prlim, number, room;
    char buffer[SIZE], season[SIZE], input[SIZE], *temp, *temp2, *hotel, choice[SIZE];
    char day1[SIZE], number_of_days[SIZE];
    
    serverfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    printf ("Give Server Port: ");
    scanf("%d", &port);
    address.sin_port = htons(port);

    address.sin_addr.s_addr = inet_addr("127.0.0.1");

    if(connect(serverfd, (struct sockaddr*) &address, sizeof(address)) < 0){
        
        perror("ERROR");
        exit(0);

    }
    else{
        printf("You are connected\n");
        
    }

    recv(serverfd, &x, sizeof(x), 0);

    recv(serverfd, buffer, SIZE, 0);    //Welcome Customer
    printf("%s", buffer);
    bzero(buffer, SIZE);
    

    recv(serverfd, buffer, SIZE, 0);    //1. Athens etc.
    printf("%s", buffer);
    bzero(buffer, SIZE);
    
    recv(serverfd, buffer, SIZE, 0);    //Please give us
    printf("%s", buffer);
    bzero(buffer, SIZE);

    scanf("%s", input);
    loc = atoi(input);
    bzero(input, SIZE);
    send(serverfd, &loc, sizeof(loc), 0);   //sends location choice




    while (loc > x || loc < 0)
    {
        recv(serverfd, buffer, SIZE, 0);    //Sorry this location
        printf("%s", buffer);
        bzero(buffer, SIZE);
        scanf("%s", input);
        loc = atoi(input);
        bzero(input, SIZE);
        send(serverfd, &loc, sizeof(loc), 0);   //sends location choice
    }

    recv(serverfd, buffer, SIZE, 0);        //What a nice choice
    printf("%s", buffer);
    bzero(buffer, SIZE);
    
    recv(serverfd, buffer, SIZE, 0);        //How many beds
    printf("%s", buffer);
    bzero(buffer, SIZE);
    scanf("%s", input);
    bed = atoi(input);
    bzero(input, SIZE);
    send(serverfd, &bed, sizeof(bed), 0);   //sends number of beds

    recv(serverfd, buffer, SIZE, 0);        //Which is the upper price
    printf("%s", buffer);
    bzero(buffer, SIZE);
    scanf("%s", input);
    //getchar();
    prlim = atoi(input);
    bzero(input, SIZE);
    send(serverfd, &prlim, sizeof(prlim), 0);   //sends upper price limit

    recv(serverfd, buffer, SIZE, 0);        //The room you want
    printf("%s", buffer);
    bzero(buffer, SIZE);
    
    recv(serverfd, buffer, SIZE, 0);        //If you want to book the room
    printf("%s", buffer);
    bzero(buffer, SIZE);
    scanf("%s", season);
    getchar();
    send(serverfd, &season, SIZE, 0);
    
    
    //while (strstr(buffer, "\nWrong input") != NULL)
    while (strcmp(season, "w") != 0 && strcmp(season,"W") != 0 && strcmp(season, "s") != 0 && strcmp(season,"S") != 0)
    {
        recv(serverfd, buffer, SIZE, 0); //Wrong input
        printf("%s", buffer);
        bzero(buffer, SIZE);
        bzero(season, SIZE);
        scanf("%s", season);
        getchar();
        send(serverfd, &season, SIZE, 0);
               
    }
    if (strcmp(season, "w") == 0 || strcmp(season,"W") == 0)
    {
        recv(serverfd, buffer, SIZE, 0);
        printf("%s", buffer);
        bzero(buffer, SIZE);

        recv(serverfd, buffer, SIZE, 0);        //1. Hotel: Room: ...
        printf("%s", buffer);
        temp = strtok(buffer, "\n");
        j = 0;
        while(temp != NULL){
            j++;
            temp = strtok(NULL, "\n");
        }
        bzero(buffer, SIZE);
    }
    else if (strcmp(season, "s") == 0 || strcmp(season,"S") == 0)
    {
        recv(serverfd, buffer, SIZE, 0);        //The available hotel..
        printf("%s", buffer);
        bzero(buffer, SIZE);

        recv(serverfd, buffer, SIZE, 0);        //1. Hotel: Room: ...
        printf("%s", buffer);
        temp = strtok(buffer, "\n");
        j = 0;
        while(temp != NULL){
            j++;
            temp = strtok(NULL, "\n");
        }
        bzero(buffer, SIZE);
    }
    
    recv(serverfd, buffer, SIZE, 0);        //Choose the hotel room
    printf("%s", buffer);
    bzero(buffer, SIZE);
    scanf("%s", input);    
    //getchar();
    number = atoi(input);
    bzero(input, SIZE);
    send(serverfd, &number, sizeof(number), 0);   //sends number choice
    while (number > j || number < 0)
    {
        recv(serverfd, buffer, SIZE, 0);    //Sorry this selection
        printf("%s", buffer);
        bzero(buffer, SIZE);
        scanf("%s", input);
        number = atoi(input);
        bzero(input, SIZE);
        send(serverfd, &number, sizeof(number), 0);   //sends number choice
    }

    recv(serverfd, buffer, SIZE, 0); // You have selected
    printf("%s", buffer);
    bzero(buffer, SIZE);
    scanf("%s", choice);
    getchar();
    send(serverfd, choice, SIZE, 0);
    

    while (strcmp(choice, "yes") != 0 && strcmp(choice,"YES") != 0 && strcmp(choice, "no") != 0 && strcmp(choice,"NO") != 0)
    {
        recv(serverfd, buffer, SIZE, 0); //Wrong input
        printf("%s", buffer);
        bzero(buffer, SIZE);
        bzero(choice, SIZE);
        scanf("%s", choice);
        getchar();
        send(serverfd, choice, SIZE, 0);  //sends choice
    }
    if (strcmp(choice, "yes") == 0 || strcmp(choice,"YES") == 0)
    {
        printf("\nMIA pasta\n");
        recv(serverfd, buffer, SIZE, 0);    //Try again later || From which day
        // printf("%s", buffer);
        if(strcmp(buffer,"Sorry, another user is currently booking that room. Please try again later\n") != 0){
            // bzero(buffer, SIZE);
            // recv(serverfd, buffer, SIZE, 0);
            printf("%s", buffer);
            if(strcmp(season, "s") == 0 || strcmp(season, "S") == 0){
                i = 0;
                do{
                    if(i != 0)
                        printf("Please type valid dates based on the season you selected.\n");
                    scanf("%s", day1);
                    getchar();
                    printf("For how many days?\n");
                    scanf("%s", number_of_days);
                    getchar();
                    i++;
                }while(atoi(day1) < 105 || atoi(day1) > 288 || atoi(day1) + atoi(number_of_days) < 105 || atoi(day1) + atoi(number_of_days) > 288 );
            } else if(strcmp(season, "w") == 0 || strcmp(season, "W") == 0){
                i = 0;
                do{
                    if(i != 0)
                        printf("Please type valid dates based on the season you selected.\n");
                    scanf("%s", day1);
                    getchar();
                    printf("For how many days?\n");
                    scanf("%s", number_of_days);
                    getchar();
                    i++;
                }while(atoi(day1) > 105 && atoi(day1) < 288 && atoi(day1) + atoi(number_of_days) > 105 && atoi(day1) + atoi(number_of_days) < 288 );
                
            }
            send(serverfd, day1, SIZE, 0);
            sleep(0.2);
            send(serverfd, number_of_days, SIZE, 0);
            
        }else{
            // recv(serverfd, buffer, SIZE, 0);
            printf("%s", buffer);
            bzero(buffer, SIZE);
        }printf("\nMIA pasta sto else\n");
        recv(serverfd, buffer, SIZE, 0);
        printf("%s", buffer);
        bzero(buffer, SIZE);
    }
    else if (strcmp(choice, "no") == 0 || strcmp(choice,"NO") == 0)
    {
        //close(clientfd);          
        //return 0;  
    } 

    close(serverfd);  
    return 0;
}

    //      Code by:
    // Panagiotis Karoutsos 02034
    // Georgia Konstantiou 02057