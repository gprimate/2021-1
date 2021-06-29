#include "common.h"
#include "constants.h"
#include "location.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include <sys/socket.h>
#include <sys/types.h>


Location location;
Location locations[MAX_NUMBER_OF_LOCATIONS];


void usage(int argc, char **argv)
{
    printf("usage: %s <v4|v6> <server port>\n", argv[0]);
    printf("example: %s v4 51511\n", argv[0]);
    exit(EXIT_FAILURE);
}

// RECEBER O TIPO DE OPERAÇAO E
//E EXECUTA A FUNCAO RESPECTIVA
int selectOperation(int clientSocket, char *operation)
{

    if (strcmp('add', operation) == 0)
    {

        addLocation(clientSocket);
    }
    else if (strcmp('rm', operation) == 0)
    {
        removeLocation(clientSocket);
    }

    else if (strcmp('list', operation) == 0)
    {
        listLocations(clientSocket);
    }
    else if (strcmp('query', operation) == 0)
    {
        query(clientSocket);
    }
    else
    {
        sendMessageToClient(clientSocket, " invalid arguments");
    }
}

// estou colocando aqui as funções q diz respeito ao server

int checkIsValidAndIsSaved(int clientSocket)
{

    char buffer[BUFFER_SIZE];

    int isValid = checkIfLocationIsValid(location);

    if (!isValid)
    {
        //muydar dps pra sprintf
        //envio da mensagem em buffer
        sprintf(buffer, "invalid location");

        printf("%p\n", &buffer);
        //send message function em common ...
        sendMessageToClient(clientSocket, buffer);

        return 0;
    }

    int isSaved = checkIfLocationIsSaved(location, locations);

    if (!isSaved)
    {
        //mudar dps pra sprintf
        //envio da mensagem em buffer
        sprintf(buffer, "%d  %d  already exists\n", location.x, location.y);

        //send message function em common ...
        printf("%p\n", &buffer);
        //sendMessageToClient(clientSocket,buffer);
        return 0;
    }

    return 1;
}

//add location   NAO TESTADA
void addLocation(int clientSocket)
{

    char buffer[BUFFER_SIZE];

    if (!checkIsValidAndIsSaved(clientSocket))
    {
        return;
    }

    int indexOfLocationToAdd = getLocationIndex(location ,locations);

    locations[indexOfLocationToAdd].x = location.x;
    locations[indexOfLocationToAdd].y = location.y;

    //send message function localizada em common ...
    sprintf(buffer, " %d %d added\n", location.x, location.y);
    printf("%p\n", &buffer);

    //sendMessageToClient(clientSocket,buffer); comentado so pra testar
}

// remove location NAO TESTADO
void removeLocation(int clientSocket)
{

    char buffer[BUFFER_SIZE];

    checkIsValidAndIsSaved(clientSocket);

    int indexOfLocationToRemove = getLocationIndex(location, locations);

    if (indexOfLocationToRemove < 0)
    {
        //send message function em common ...
        //nao existe
        sprintf(buffer, "%d %d already exists", location.x, location.y);
        printf("%p\n", &buffer);
        //sendMessageToCient(clientSocket,buffer); -> comentado so pra testar
    }

    locations[indexOfLocationToRemove].x = -1;
    locations[indexOfLocationToRemove].y = -1;

    //send message function em common ...
    sprintf(buffer, "%d %d removed\n", location.x, location.y);
    printf("%p\n", &buffer);

    //sendMessageToClient(clientSocket,buffer);
}

void listLocations(int clientSocket)
{

    char buffer[BUFFER_SIZE] = "";

    if (isEmpty(locations))
    {
        sprintf(buffer, "none\n");
    }
    else
    {

        for (size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++)
        {

            if (checkIfLocationIsValid(locations[i]))
            {
                char location[MAX_LOCATION_SIZE] = "";

                sprintf(location, "%d %d ", locations[i].x, locations[i].y);
                strcat(buffer, location);
            }
        }
    }
    sendMessageToClient(clientSocket, buffer);
}

void query(int clientSocket)
{

    char buffer[BUFFER_SIZE] = "";

    double smallestDistance = 10000 * sqrt(2);
    Location smallestLocation;

    if (isEmpty(locations))
    {
        sprintf(buffer, "none\n");
    }
    else
    {
        for (size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++)
        {

            if (checkIfLocationIsValid(locations[i]))
            {
                double distance = getDistanceBetweenLocations(location, locations[i]);

                if (distance < smallestDistance && distance != 0)
                {
                    smallestDistance = distance;
                    smallestLocation = locations[i];
                }
            }
        }
        sprintf(buffer, "%d %d ", smallestLocation.x, smallestLocation.y);
    }
    sendMessageToClient(clientSocket, buffer);
}


//recuperar operação
char getOperation (char *buff){

    // tratar o buffer
    char *token;
    token = strtok(buff," ");
    return token;
}



void execucaoServidor(char *buf,int clientSocket){

    //separar os comandos em linhas
    int contLines=0;
    char *token = strtok(buf,"\n");
    int i = 0;
            
    //Recuperar array de lines           
    char *lines[BUFFER_SIZE];
    

    while (token != NULL)
    {

        //ver se deu certo isso valor de lines
        strcpy(lines[i], token);
        contLines ++;
        token = strtok(NULL, "\n");

    }

    //partir a string em espaços e mandar para o fluxo
    for(i=0;i<contLines;i++){

         
       
        //partindo uma linha em tokens
        char *line;
        memset(line,0,BUFFER_SIZE);
        token = strtok(lines[i]," ");
        while(token != NULL){
            //strcpy(line[i]);
            i++;

                
        }
        /*
        //apos partir line[0] = op  / line[1] = x / line[2] = y;
        locations[i].x = line[1];
        locations[i].y = line[2];
        */
        //operacao corrente
        char *operation = getOperation(line[0]);

        //continuação para definição da operação ..
        selectOperation(clientSocket,operation);
    

    }
    /*
    char line
    token = strtok(lines[i]," ");
    */
  
   

    //tratar operações invalidas

   

}



int main(int argc, char **argv)
{

    // argumentos invalidos
    if (argc < 3)
    {
        usage(argc, argv);
    }

    // criacao storage
    struct sockaddr_storage storage;
    if (0 != server_sockaddr_init(argv[1], argv[2], &storage))
    {
        usage(argc, argv);
    }

    //socket server
    int s;
    s = socket(storage.ss_family, SOCK_STREAM, 0);
    if (s == -1)
    {
        logexit("socket");
    }

    // socket options
    int enable = 1;
    if (0 != setsockopt(s, SOL_SOCKET, SO_REUSEADDR, &enable, sizeof(int)))
    {
        logexit("setsockopt");
    }

    //bind de porta para o server
    struct sockaddr *addr = (struct sockaddr *)(&storage);
    if (0 != bind(s, addr, sizeof(storage)))
    {
        logexit("bind");
    }

    // w8 connections
    if (0 != listen(s, 10))
    {
        logexit("listen");
    }

    // log espera de conexões
    char addrstr[BUFFER_SIZE];
    addrtostr(addr, addrstr, BUFFER_SIZE);
    printf("bound to %s, waiting connections\n", addrstr);

    //loop server
    while (1)
    {

        // storage e socket client
        struct sockaddr_storage cstorage;
        struct sockaddr *caddr = (struct sockaddr *)(&cstorage);
        socklen_t caddrlen = sizeof(cstorage);

        //acc connections
        int csock = accept(s, caddr, &caddrlen);
        if (csock == -1)
        {
            logexit("accept");
        }

        //log conexao cliente
        char caddrstr[BUFFER_SIZE];
        addrtostr(caddr, caddrstr, BUFFER_SIZE);
        printf("[log] connection from %s\n", caddrstr);

        while (1)
        {

            char buf[BUFFER_SIZE];
            memset(buf, 0, BUFFER_SIZE);
            size_t count = recv(csock, buf, BUFFER_SIZE - 1, 0);
            printf("[msg] %s, %d bytes: %s\n", caddrstr, (int)count, buf);

            //recuperar mensagem total
            char input[BUFFER_SIZE];
            memset(input, 0, BUFFER_SIZE);
            strcpy(input, buf);
            printf(" mensagem contida em input :  %s", input);
            int i = 0;

            //filtro msg
            //int cont=0;
            for (i = 0; i <= strlen(input); i++)
            {
                if (input[i] == '\n')
                {
                    input[i] = 0;
                }
            }

            //verificar kill
            if (strcmp(input, "kill") == 0)
            {
                logexit("killed server");
                close(csock);
                return -1;
            }

            char *token = strtok(input, "\n");
            sendMessageToClient(csock, "checando\n");

            while (token != NULL)
            {
                printf(" conteudo de token : %s\n", token);
                token = strtok(NULL, "\n");
            }

            // tratamento das consultas
            char *operation;

            //execucao servidor "init"
            execucaoServidor(input,csock);
            
            //sprintf(buf, "remote endpoint: %.1000s\n", caddrstr);
            //count = send(csock, buf, strlen(buf) + 1, 0);
            /*
         if (count != strlen(buf) + 1) {
            logexit("send");
        }
        close(csock);

        }

        */
        }

        exit(EXIT_SUCCESS);
    }
}