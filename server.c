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

void *myThread();
void selectionSort(char *arr[], int rows);
int arraySortedOrNot(char **arr, int n);

typedef struct hotels{                                                         

    int beds;
    int number;
    int winter;
    int summer;
    int* days;
    char* hotel;
    char* location;
    char* view;
    
}
hotels;

int sizes[SIZE], rows = -1, *locked_rooms;
hotels *rooms;
pthread_mutex_t lock;


int main (int argc, char **argv){

    int socketfd, port, clientfd;
    struct sockaddr_in address;
    char buffer[SIZE];
    int len = sizeof(address);

    int  i, lf, y, j, k;
    char *hotel, *location, line[SIZE], *temp, *view, **grammes, *day, *dayz;
    FILE *fpointer;

    pthread_t thread_id;

    socketfd = socket(AF_INET, SOCK_STREAM, 0);

    address.sin_family = AF_INET;
    printf ("Give Port: ");
    scanf("%d", &port);
    address.sin_port = htons(port);

    address.sin_addr.s_addr = htonl(INADDR_ANY);

    if(setsockopt(socketfd, SOL_SOCKET, SO_REUSEADDR, &(int){1}, sizeof(int)) < 0)
    {                                                                                      
        perror("Setsockopt(SO_REUSEADDR) failed");
        exit(EXIT_FAILURE);
    }

    bind(socketfd, (struct sockaddr*) &address, sizeof(address));

    listen(socketfd, 1);

    fpointer = fopen("hotel.txt","r");   
                                             
    if (fpointer == NULL)                                                       
    {
        fprintf(stderr,"No dictionary file detected\n");
        return 0;
    }

    while(lf != -1)
    {
        lf = fscanf(fpointer,"%s",line);
        rows++;        
    }

    printf("\n\nTHE AVAILABLE HOTEL ROOMS ARE: %d", rows);

    locked_rooms = (int *)malloc(rows * sizeof(int));
    
    grammes = (char **)malloc(rows * sizeof(char*));

    rooms = (hotels*)malloc(rows * sizeof(hotels));
    //sizes = (int*)malloc(rows * sizeof(int));

    y = 0;
    lf = 0;

    fseek(fpointer,0,SEEK_SET);

    while(lf != -1)
    {
        lf = fscanf(fpointer,"%s",line);

        grammes[y] = (char*)malloc(strlen(line)*sizeof(char));
        strcpy(grammes[y], line);
        y++;        
    }

    printf("\n\nPRIN\n\n");

    for(y = 0; y < rows; y++)
    {
        printf("%s\n", grammes[y]);
    }

    if(!arraySortedOrNot(grammes, rows)){

        selectionSort(grammes, rows);

        printf("\n\nMETA\n\n");

        fpointer = fopen("hotel.txt","w"); 

        for(y = 0; y < rows; y++)
        {
            printf("%s\n", grammes[y]);
            if(y != rows - 1){
                fprintf(fpointer,"%s\n", grammes[y]);
            }else{
                fprintf(fpointer,"%s", grammes[y]);
            }
        }
    }

    fclose(fpointer);

    fpointer = fopen("hotel.txt","r");

    fseek(fpointer,0,SEEK_SET);

    for(i = 0; i < rows; i++)                                                   
    {
        fscanf(fpointer,"%s",line);
        
        hotel = strtok(line, "/"); 
        rooms[i].hotel = (char *)malloc(strlen(hotel) * sizeof(char));
        strcpy(rooms[i].hotel, hotel);
        rooms[i].beds = atoi(strtok(NULL, "/")); 
        rooms[i].number = atoi(strtok(NULL, "/"));
        location = strtok(NULL, "/");
        rooms[i].location = (char *)malloc(strlen(location) * sizeof(char));
        strcpy(rooms[i].location, location);
        rooms[i].summer = atoi(strtok(NULL, "/"));
        rooms[i].winter = atoi(strtok(NULL, "/"));
        view = strtok(NULL, "/");
        rooms[i].view = (char *)malloc(strlen(view) * sizeof(char));
        strcpy(rooms[i].view, view);
        temp = strtok(NULL, "/");

        sizes[i] = 0;

        if(temp != NULL){      

            dayz = (char *)malloc(strlen(temp) * sizeof(char));
            strcpy(dayz,temp);

            day = strtok(temp,".");

            j = 0;

            while(day != NULL){

                day = strtok(NULL,".");
                j++;
            } 

            rooms[i].days = (int *)malloc(j * sizeof(int)); 

            day = strtok(dayz,".");

            sizes[i] = j;

            j = 0;

            while(day != NULL){
                rooms[i].days[j] = atoi(day);
                day = strtok(NULL,".");
                j++;
            }
        }
    }

    fclose(fpointer);

    while(1){

        clientfd = accept(socketfd, (struct sockaddr*) &address,  &len);

        pthread_create(&thread_id, NULL, myThread, &clientfd);
        
    }

    pthread_join(thread_id,NULL);
    //na leei client disconnected otan vgainei kapoios client
    return 0;
}


void *myThread(void *argument){

    int  i, k, n, x = 1, *ndays, j, lf, found, sel, p = 1, selection, bed, prlim, loc, number;
    int clientfd = *((int *)argument), room, booked_size, all_good = 1, *tempo2;
    char *hotel, *location, *days, *bookings, *day, line[SIZE], *view, **kati, temp[MAX];
    char buffer[SIZE], buffer2[136],season[SIZE], input[SIZE], *choices[rows], choice[SIZE];
    char *tempo1, day1[SIZE], number_of_days[SIZE];
    FILE *fpointer;
    
    printf("\nClient with Socket Descriptor %d connected!\n", clientfd);


    kati = (char**)malloc(rows * sizeof(char*));

    
    for(i = 0; i < rows; i++)                                                   
    {

        if( i == 0 ){

            kati[0] = (char *)malloc(strlen(rooms[i].location) * sizeof(char));
            strcpy(kati[0], rooms[i].location);
    
        }else{
    
            int is_unique = 1;
 
             for (k = 0; k < x; k++)
             {
                if (strcmp(kati[k], rooms[i].location) == 0){
                   is_unique = 0;
                   break;
                }
             }

             if (is_unique){
    	       kati[k] = (char *)malloc(strlen(rooms[i].location) * sizeof(char));
               strcpy(kati[k], rooms[i].location);
               x++;    
             }
           }     

    }
    send(clientfd, &x, sizeof(x), 0); 
    sel = 0;

    strcpy(buffer,"\nWelcome customer! You can book your room here. Give us a Location. \nYou can choose among the following: \n");
    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);
    sprintf(buffer,"\n1. %s\n",kati[0]);

    for (i=1; i<x; i++){
        
        sprintf(temp,"%d. %s\n",i+1,kati[i]);
        strcat(buffer,temp);
    }

    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);
    
    strcpy(buffer,"\nPlease enter the number of the location you want to visit: ");
    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);
    recv(clientfd, &loc , sizeof(loc), 0);

    while (loc > x || loc < 0)
    {
        strcpy(buffer,"\nSorry, this location is not available. Try again: ");
        send(clientfd, buffer, SIZE, 0);
        bzero(buffer, SIZE);
        recv(clientfd, &loc , sizeof(loc), 0);
    }

    sprintf(buffer, "\nWhat a nice choice! \n%s is an amazing place!\n\n", kati[loc-1]);
    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);

    strcpy(buffer,"\nHow many beds do you need? Give us a number: ");
    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);
    recv(clientfd, &bed , sizeof(bed), 0);
    
    strcpy(buffer,"\nWhich is the upper price limit for the room you want to book? Type here: ");
    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);
    recv(clientfd, &prlim , sizeof(prlim), 0);
    
    sprintf(buffer,"\nThe room you want to order has %d beds and the price limit is %d€ per day.\n", bed, prlim);
    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);

    strcpy(buffer,"If you want to book the room for winter type W.\nIf you want to book the room for summer type S: ");
    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);
    recv(clientfd, &season, SIZE, 0);

    //if(recv(clientfd, &season, SIZE, 0) <= 0){
        // printf("Client disconnected\n");
        // close(clientfd);                     // anixneuei an kleinei enas client
        // return 0;                            // 8a eprepe na to kanw se oles tis recv
    //}

    printf("\nEftasa mexri edw1\n");

    while (strcmp(season, "w") != 0 && strcmp(season,"W") != 0 && strcmp(season, "s") != 0 && strcmp(season,"S") != 0)
    {
        strcpy(buffer,"\nWrong input, please try again: ");
        send(clientfd, buffer, SIZE, 0);
        bzero(buffer, SIZE);
        recv(clientfd, &season, sizeof(season), 0);
    }
    
    printf("\nEftasa mexri edw2\n");

    if (strcmp(season, "w") == 0 || strcmp(season,"W") == 0)
    {
        sprintf(buffer,"\nThe available hotel rooms with %d beds are:\n\n", bed);
        send(clientfd, buffer, SIZE, 0);
        bzero(buffer, SIZE);

        j = 0;

        for (i = 0; i < rows; i++)
        {
            if(strcmp(rooms[i].location, kati[loc-1]) == 0 && rooms[i].winter <= prlim && rooms[i].beds == bed)
            {
                j++;
                sprintf(temp, "%d. Hotel: %s, Room: %d, Price: %d, View: %s\n", j , rooms[i].hotel, rooms[i].number, rooms[i].winter, rooms[i].view);
                choices[j-1] = (char *)malloc(strlen(temp) * sizeof(char));
                strcpy(choices[j-1], temp);
                printf("%s", temp);
                strcat(buffer,temp);
            }
        }
        send(clientfd, buffer, SIZE, 0);
        bzero(buffer, SIZE);
    }
    else if (strcmp(season, "s") == 0 || strcmp(season,"S") == 0)
    {
        sprintf(buffer,"\nThe available hotel rooms with %d beds are:\n\n", bed);
        send(clientfd, buffer, SIZE, 0);
        bzero(buffer, SIZE);

        j = 0;

        for (i = 0; i < rows; i++)
        {
            if(strcmp(rooms[i].location, kati[loc-1]) == 0 && rooms[i].winter <= prlim && rooms[i].beds == bed)
            {
                j++;
                sprintf(temp, "%d. Hotel: %s, Room: %d, Price: %d, View: %s\n", j , rooms[i].hotel, rooms[i].number, rooms[i].summer, rooms[i].view);
                choices[j-1] = (char *)malloc(strlen(temp) * sizeof(char));
                strcpy(choices[j-1], temp);
                printf("%s", temp);
                strcat(buffer,temp);
            }
        }
        send(clientfd, buffer, SIZE, 0);
        bzero(buffer, SIZE);
    }  
    strcpy(buffer,"Choose the hotel room you would like by entering the corresponding number: ");
    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);
    recv(clientfd, &number, sizeof(number), 0);
    while (number > j || number < 0)
    {
        strcpy(buffer,"\nSorry, this selection is unavailable. Try again: ");
        send(clientfd, buffer, SIZE, 0);
        bzero(buffer, SIZE);
        recv(clientfd, &number , sizeof(number), 0);
    }
    printf("\n");
    sprintf(buffer,"You have selected:\n%sWould you like to proceed? Type \"yes\" to proceed to booking, or \"no\" to return to room selection:  ", choices[number-1]);
    send(clientfd, buffer, SIZE, 0);
    bzero(buffer, SIZE);
    recv(clientfd, choice , SIZE, 0);
    printf("\nMIA pasta\n");

    while (strcmp(choice, "yes") != 0 && strcmp(choice,"YES") != 0 && strcmp(choice, "no") != 0 && strcmp(choice,"NO") != 0)
    {
        strcpy(buffer,"\nWrong input, please try again: ");
        send(clientfd, buffer, SIZE, 0);
        bzero(buffer, SIZE);
        bzero(season, SIZE);
        recv(clientfd, choice , SIZE, 0);
    }
    if (strcmp(choice, "yes") == 0 || strcmp(choice,"YES") == 0)
    {

        printf("\nMIA pasta, %s\n", choices[number - 1]);
        tempo1 = strtok(choices[number-1], ":");
        tempo1 = strtok(NULL, ", ");
        hotel = (char *)malloc(strlen(tempo1) * sizeof(char));
        strcpy(hotel,tempo1);
        tempo1 = strtok(NULL, ":");
        tempo1 = strtok(NULL, ", ");
        room = atoi(tempo1);
        printf("\nMIA pasta sto hotel %s, %d\n", hotel, room);

        printf("\nMIA pasta sto yes\n");
        for(i = 0; i < rows; i++){
            if(strcmp(rooms[i].hotel, hotel) == 0 && rooms[i].number == room)
                break;
        }

        if(locked_rooms[i] == 1){

            strcpy(buffer,"Sorry, another user is currently booking that room. Please try again later\n");
            send(clientfd, buffer, SIZE, 0);
            bzero(buffer, SIZE);

        }else{

            locked_rooms[i] = 1;

            pthread_mutex_lock(&lock);

            strcpy(buffer,"From which day do you want to book the room?\n");
            send(clientfd, buffer, SIZE, 0);
            bzero(buffer, SIZE);
            recv(clientfd, day1 , SIZE, 0);
            recv(clientfd, number_of_days , SIZE, 0);
            printf("\nMIA pasta sto booked size\n");
            all_good = 1;
            printf("\nMIA pasta %d, %s\n", room, hotel);
            if(sizes[i] > 0){
                
                for(j = 0; j < sizes[i]; j++){
                    for(n = 0; n < atoi(number_of_days); n++){
                        if(atoi(day1) + n == rooms[i].days[j]){
                            all_good = 0;
                            break;
                        }else
                            all_good = 1;
                    }
                    if(!all_good){
                        break;
                    }
                        
                }
            }else{
                printf("\nMIA pasta meta to booked size %d %d\n", atoi(day1), atoi(number_of_days));
                rooms[i].days = (int *)malloc(sizeof(int) * atoi(number_of_days));
                printf("\nMIA pasta meta to booked size\n");
                
            }

            if(all_good){

                if(sizes[i] > 0){
                    tempo2 = (int *)realloc(rooms[i].days, (sizes[i] + atoi(number_of_days)) * sizeof(int));
                    rooms[i].days = (int *)malloc((sizes[i] + atoi(number_of_days)) * sizeof(int));
                    for(j = 0; j < sizes[i]; j++){
                        rooms[i].days[j] = tempo2[j];
                    }
                }

                for(j = 0; j < atoi(number_of_days); j++){
                    rooms[i].days[sizes[i] + j] = atoi(day1) + j;
                }
                sizes[i] = sizes[i] + atoi(number_of_days);
                fpointer = fopen("hotel.txt","w");   

                for(j = 0; j < rows; j++){

                    fprintf(fpointer,"%s/%d/%d/%s/%d/%d/%s/", rooms[j].hotel, rooms[j].beds, rooms[j].number, rooms[j].location, rooms[j].summer, rooms[j].winter, rooms[j].view);

                    if(sizes[j] > 0){
                        fprintf(fpointer, "%d", rooms[j].days[0]);
                        for(n = 1; n < sizes[j]; n++){
                            fprintf(fpointer, ".%d", rooms[j].days[n]);
                        }
                    }
                    fprintf(fpointer,"/\n");
                }
                fclose(fpointer);
                strcpy(buffer, "Room booked successfully for those days\n");
                send(clientfd, buffer, SIZE, 0);
                bzero(buffer, SIZE);

            }else{
                strcpy(buffer, "We're sorry to inform you that the room is booked for those days\n");
                send(clientfd, buffer, SIZE, 0);
                bzero(buffer, SIZE);
            }

            pthread_mutex_unlock(&lock);

            locked_rooms[i] = 0;
        }

        
    }
    else if (strcmp(choice, "no") == 0 || strcmp(choice,"NO") == 0)
    {         
        //return 0;  
    } 

    close(clientfd);          
    //return 0;  

}

void selectionSort(char *arr[], int rows)
{
    int i, j, min_idx;
 
    for (i = 0; i < rows - 1; i++) {
 
        min_idx = i;
        for (j = i + 1; j < rows; j++)
            if(strcmp(arr[j],arr[min_idx]) < 0)
                min_idx = j;

        char *temp = arr[min_idx];
        arr[min_idx] = arr[i];
        arr[i] = temp;
    }
}

int arraySortedOrNot(char **arr, int n)
{
    // Array has one or no element or the
    // rest are already checked and approved.
    if (n == 1 || n == 0)
        return 1;
 
    // Unsorted pair found (Equal values allowed)
    if (strcmp(arr[n - 1], arr[n - 2]) < 0)
        return 0;
 
    // Last pair was sorted
    // Keep on checking
    return arraySortedOrNot(arr, n - 1);
}

    //      Code by:
    // Panagiotis Karoutsos 02034
    // Georgia Konstantiou 02057
