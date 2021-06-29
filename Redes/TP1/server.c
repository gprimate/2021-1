#include "common.h"
#include "constants.h"
#include "location.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>


void usage(int argc, char **argv) {
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}



// RECEBER O TIPO DE OPERAÇAO E 
//TRATAR SE ESTA VALIDA OU INVALIDA E RETORNAR O TIPO DA OPERAÇAO
int getOperationType(char *buf){
    /*
    int i=0;
    char aux[10];

    for(i=0;i<10;i++ ){
        aux[i] = 0;
    }

    for (i=0;i< buf[i] != ' ' ;i++){
        if(buf )
    }
*/
    /*
    switch (expression)
    {
    case /* constant-expression */ //:
        /* code */
       // break;
    /*
    default:
        break;
    }
    */

}

// estou colocando aqui as funções q diz respeito ao server 



int checkIsValidAndIsSaved(int clientSocket , Location location, Location locations[]){

    char buffer[BUFFER_SIZE];

    int isValid = checkIfLocationIsValid(location);
    
    if(!isValid){
        //muydar dps pra sprintf
        //envio da mensagem em buffer
        sprintf(buffer,"invalid location");
        
        printf("%p\n",&buffer);
        //send message function em common ...
        sendMessageToClient(clientSocket,buffer);

        return 0;
    }
    
    int isSaved = checkIfLocationIsSaved(location,locations);

    if(!isSaved) {
        //mudar dps pra sprintf
        //envio da mensagem em buffer
        sprintf(buffer,"%d  %d  already exists\n", location.x ,location.y);

        //send message function em common ...
        printf("%p\n",&buffer);
        //sendMessageToClient(clientSocket,buffer);
        return 0;
    }

    return 1;
}





//add location   NAO TESTADA
void addLocation(int clientSocket , Location locationToAdd, Location locations[]){

    char buffer[BUFFER_SIZE];
    
    if (!checkIsValidAndIsSaved(clientSocket,locationToAdd,locations)) {
        return;
    }

    int indexOfLocationToAdd = getLocationIndex(locationToAdd,locations);  
    
    locations[indexOfLocationToAdd].x = locationToAdd.x;
    locations[indexOfLocationToAdd].y = locationToAdd.y;


    //send message function localizada em common ...
    sprintf(buffer," %d %d added\n",locationToAdd.x,locationToAdd.y);
    printf("%p\n",&buffer);

    //sendMessageToClient(clientSocket,buffer); comentado so pra testar
}

// remove location NAO TESTADO
void removeLocation(int clientSocket , Location locationToRemove, Location locations[]){

    char buffer[BUFFER_SIZE];

    checkIsValidAndIsSaved(clientSocket,locationToRemove,locations);

    int indexOfLocationToRemove = getLocationIndex(locationToRemove,locations);


    if(indexOfLocationToRemove < 0){
        //send message function em common ...
        //nao existe
        sprintf(buffer,"%d %d already exists",locationToRemove.x,locationToRemove.y);
        printf("%p\n",&buffer);
        //sendMessageToCient(clientSocket,buffer); -> comentado so pra testar
    }

    locations[indexOfLocationToRemove].x = -1;
    locations[indexOfLocationToRemove].y = -1;
    
    //send message function em common ...
    sprintf(buffer,"%d %d removed\n",locationToRemove.x,locationToRemove.y);
    printf("%p\n",&buffer);

    //sendMessageToClient(clientSocket,buffer);
}

void listLocations(int clientSocket, Location locations[]) {

<<<<<<< HEAD
    
=======
    char buffer[BUFFER_SIZE] = "";

    if (isEmpty(locations)) {
        sprintf(buffer, "none\n");
>>>>>>> 43d29e5815abed1719c7d38d3f890c5cc1f044fb

    } else {

        for (size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++) {

            if (checkIfLocationIsValid(locations[i])) {
                char location[MAX_LOCATION_SIZE] = "";

                sprintf(location, "%d %d ", locations[i].x, locations[i].y);
                strcat(buffer, location);
            }
        }
    }
    sendMessageToClient(clientSocket, buffer);
}


void query(int clientSocket, Location location, Location locations[]) {

    char buffer[BUFFER_SIZE] = "";
    
    double smallestDistance = 10000 * sqrt(2);
    Location smallestLocation;

    if (isEmpty(locations)) {
        sprintf(buffer, "none\n");

    } else {
        for (size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++) {

            if (checkIfLocationIsValid(locations[i])) {
                double distance = getDistanceBetweenLocations(location, locations[i]);

                if (distance < smallestDistance && distance != 0) {
                    smallestDistance = distance;
                    smallestLocation = locations[i];
                }
            }
        }
        sprintf(buffer, "%d %d ", smallestLocation.x, smallestLocation.y);
    }
    sendMessageToClient(clientSocket, buffer);
}


int main(int argc, char **argv) {

    // argumentos invalidos
    if (argc < 3) {
        usage(argc, argv);
    }


    // criacao storage
    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage)) {
        usage(argc, argv);
    }



    //socket server
    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1) {
        logexit("socket");
    }

    // socket options
    int enable = 1;
    if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int))) {
        logexit("setsockopt");
    }


    //bind de porta para o server
    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage))) {
        logexit("bind");
    }


    // w8 connections
    if (0 != listen(s, 10)) {
        logexit("listen");
    }

    // log espera de conexões
    char addrstr[BUFFER_SIZE];
    addrtostr(addr, addrstr, BUFFER_SIZE);
    printf("bound to %s, waiting connections\n", addrstr);


    //loop server
    while (1) {

        // storage e socket client
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        //acc connections
        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1) {
            logexit("accept");
        }
        
        //log conexao cliente
        char caddrstr[BUFFER_SIZE];
        addrtostr(caddr, caddrstr, BUFFER_SIZE);
        printf("[log] connection from %s\n", caddrstr);

        char buf[BUFFER_SIZE];
        memset(buf, 0, BUFFER_SIZE);
        size_t count = recv(csock, buf, BUFFER_SIZE - 1, 0);
        printf("[msg] %s, %d bytes: %s\n", caddrstr, (int)count, buf);
        
        //recuperar mensagem total
        char input[BUFFER_SIZE];
        memset(input,0,BUFFER_SIZE);
        strcpy(input,buf);
        printf(" mensagem contida em input :  %s",input);
        int i=0;

        //verificar kill
        //filtro msg
        int cont=0;
        for(i=0;i<= strlen(input);i++){
            
            if(input[i] == '\n'){
                input[i] = 0;
            }
            
        }
        

        if(strcmp(input,"kill") == 0){
            logexit("killed server");
            close(csock);
            return -1;
        }

        char *token = strtok(input,"\n");
        sendMessageToClient(csock,"checando");

        while(token != NULL){
            printf(" conteudo de token : %s\n",token);
            token = strtok(NULL,"\n");
        }
        
        i=0;
        char lines[BUFFER_SIZE][BUFFER_SIZE];
        memset(lines,0,BUFFER_SIZE*BUFFER_SIZE);

        //Recuperar array de lines
        
        while(token != NULL){
            strcpy(lines[i],token);
            i+= 1;
            token = strtok(NULL,"\n");


        }

        //char *mess = token;

        printf("mess %s",lines[0]);

        //continuação para definição da operação ..





        //sprintf(buf, "remote endpoint: %.1000s\n", caddrstr);
        //count = send(csock, buf, strlen(buf) + 1, 0);
        if (count != strlen(buf) + 1) {
            logexit("send");
        }
        close(csock);
    }

    exit(EXIT_SUCCESS);
}