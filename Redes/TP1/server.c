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
void selectOperation(int clientSocket, char *operation)
{
    

    if ( strcmp(operation,"add") == 0 ) 
    {
        
        addLocation(clientSocket);
    }
    else if ( strcmp(operation ,"rm") == 0 )
    {
        
        removeLocation(clientSocket);
    }
    else if ( strcmp(operation ,"query") == 0)
    {
        //printf("e aq entrou em query ?");
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

        printf("%s\n", buffer);
        //send message function em common ...
        sendMessageToClient(clientSocket, buffer);

        return 0;
    }

    int isSaved = checkIfLocationIsSaved(location, locations);

    if (isSaved)
    {
        //mudar dps pra sprintf
        //envio da mensagem em buffer
        sprintf(buffer, "%d  %d  already exists\n", location.x, location.y);

        //send message function em common ...
        sendMessageToClient(clientSocket,buffer);
        return 0;
    }

    return 1;
}

//add location   NAO TESTADA
void addLocation(int clientSocket)
{

    char buffer[BUFFER_SIZE];
    memset(buffer,0,BUFFER_SIZE);

    //LOCAL PROBLEMÁTICO INICIE AQUI !

    if (!checkIsValidAndIsSaved(clientSocket))
    {
        sprintf(buffer,"invalid location");
        return;
        //sendMessageToClient(clientSocket,buffer);
    }

    int indexOfLocationToAdd = getFirstEmptyPlace(location ,locations);

    //printf("valor do index : %d",indexOfLocationToAdd);

    locations[indexOfLocationToAdd].x = location.x;
    locations[indexOfLocationToAdd].y = location.y;

    //send message function localizada em common ...
    sprintf(buffer, " %d %d added\n", location.x, location.y);
    

    sendMessageToClient(clientSocket,buffer);
}

// remove location 
void removeLocation(int clientSocket)
{

    char buffer[BUFFER_SIZE];
   
    if(!checkIfLocationIsValid(location)){
        printf("tragedia\n");
        return;
    }
   
    int indexOfLocationToRemove = getLocationIndex(location, locations);

    printf("valor de index para remoção : %d ",indexOfLocationToRemove);
    if (indexOfLocationToRemove < 0)
    {
        //send message function em common ...
        //nao existe
        sprintf(buffer, "%d %d does not exist", location.x, location.y);
        
        sendMessageToClient(clientSocket,buffer);
        return;
    }

    locations[indexOfLocationToRemove].x = -1;
    locations[indexOfLocationToRemove].y = -1;

    //send message function em common ...
    sprintf(buffer, "%d %d removed\n", location.x, location.y);
    //printf("%p\n", &buffer);

    sendMessageToClient(clientSocket,buffer);
}

void listLocations(int clientSocket)
{

    //printf("entrou em listLocations ? \n");
    char buffer[BUFFER_SIZE] = "";

    if (isEmpty(locations))
    {
        sprintf(buffer, "none\n");
    }
    else
    {

        for (size_t i = 0; i < MAX_NUMBER_OF_LOCATIONS; i++)
        {
            if(locations[i].x >= 0 && locations[i].y >= 0){
                if (checkIfLocationIsValid(locations[i])){
                char location[MAX_LOCATION_SIZE] = "";

                sprintf(location, "%d %d ", locations[i].x, locations[i].y);
                strcat(buffer, location);
                }


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
        printf("essa eh a smallest x : %d\n",smallestLocation.x);
        printf("essa eh a smallest y : %d\n",smallestLocation.y);

        sprintf(buffer, "%d %d", smallestLocation.x, smallestLocation.y);
    }

    printf("valor de buffer : %s\n",buffer);

    sendMessageToClient(clientSocket, buffer);
    return;
}

int checkIfOperationIsValid(char *buf) {
    char * token = strtok(buf, " ");

    char operation[MAX_OP_SIZE];
    Location location;

    int count = 0;
    printf("Esse eh o buf: %s|\n", buf);

    if ((strcmp(buf, ADD) == 0)){
        return 1;
    }
    if ((strcmp(buf, REMOVE) == 0)){
        return 1;
    }
    if ((strcmp(buf, QUERY) == 0)){ 
        return 1;
    }
    if ((strcmp(buf, LIST) == 0)){ 
        return 1;
    }

    return 0;
}

int checkIfLocationIsValidFromBuffer(char *buf) {

    //printf("Esse é o token que chegou como parametro: %s|\n", buf);
    char * token = strtok(buf, " ");
    //printf("Esse é o token: %s|\n", token);
    token = strtok(NULL, " "); 
    //printf("Esse é o segundo token: %s|\n", token);
    Location location;

    int coordx, coordy = -1;

    if (token != NULL) {
        for (size_t i = 0; i < strlen(token); i++) {
            if (!isdigit(token[i])) {
                printf("checkIfLocationIsValidFromBuffer retornou falso com i = %d\n", (int)i);
                return 0;
            }
        }
        coordx = atoi(token);
    } else {
        //printf("Primeiro token é nulo\n");
        return 0;
    }

    token = strtok(NULL, " "); 

    if (token != NULL) {
        for (size_t i = 0; i < strlen(token); i++) {
            if (!isdigit(token[i])) {
               // printf("checkIfLocationIsValidFromBuffer retornou falso com i = %d\n", (int)i);
                return 0;
            }
        }
        coordy = atoi(token);

        location.x = coordx;
        location.y = coordy;

        if(checkIfLocationIsValid(location)) {
            return 1;

        }
        

    } else {
       // printf("Segundo token é nulo\n");
        return 0;
    }
    return 0;
}

void serverExec(char *buf,int clientSocket){


    int contLines=0;
    char *token = strtok(buf,"\n");
    int i = 0;

    char lines[BUFFER_SIZE][BUFFER_SIZE];
    memset(lines,0,BUFFER_SIZE*BUFFER_SIZE);
    
     while (token != NULL)
    {

        //ver se deu certo isso valor de lines
        strcpy(lines[contLines], token);
        contLines ++;
        token = strtok(NULL, "\n");

    }

    /*
    //teste conteudo lines
    for(i=0;i<contLines;i++){
        
    }
    */
    //printf("Lines[0] antes de chamar funcao? %s\n", lines[0]);
    //printf("Is the operation valid? %d\n", checkIfOperationIsValid(lines[0]));
    //printf("Is the location valid? %d\n", checkIfLocationIsValidFromBuffer(lines[0]));


    // loop para iterar entre comandos
    for(i=0;i<contLines;i++){

        
        char line[BUFFER_SIZE][BUFFER_SIZE];
        memset(line,0,BUFFER_SIZE*BUFFER_SIZE);
        
        char *tok = strtok(lines[i]," ");
        int numOfTokens =0;
        //int cont=0;
       // printf("Lines[0] depois do strtok? %s\n", lines[0]);
        
        
        while(tok != NULL){
            strcpy(line[numOfTokens],tok);
            numOfTokens++;
            tok = strtok(NULL," ");

        }

        // AQUI VAI O TRATAMENTO DE COMANDOS INVALIDOS PELO NUMERO DE TOKENS


        //TRATAMENTO DO PRIMEIRO TOKEN SE EH UM DOS 4 COMANDOS PERMITIDOS
        
        if (numOfTokens > 1 && numOfTokens <= 3){


            location.x=atoi(line[1]);
            location.y=atoi(line[2]);
            
                    
            printf(" valor de operation : %s\n",line[0]);
            printf(" valor de location.x : %d\n",location.x);
            printf(" valor de location.y : %d\n",location.y);


            //Necessario debugar o select operation
           // sendMessageToClient(clientSocket, "\ncomeco\n"); //apagar
            //sendMessageToClient(clientSocket, line[0]); //apagar
          //  sendMessageToClient(clientSocket, "\nfinal da mensagem\n"); //apagar

            selectOperation(clientSocket,line[0]);


       }else if(numOfTokens == 1){
        //se o comando so tem 1 palavra eh pq eh list
        listLocations(clientSocket);
       }

       else{
           //comando invalido
           sendMessageToClient(clientSocket,"Invalid command");
       }

       
                

    }

    //printf("valor de contLines %d\n",contLines);

    
}

/*
void execucaoServidor(char *buf,int clientSocket){
    printf("chegou aaq ");
    //separar os comandos em linhas
    int contLines=0;
    char *token = strtok(buf,"\n");
    int i = 0;
            
    //Recuperar array de lines           
    char lines[BUFFER_SIZE][BUFFER_SIZE];
    memset(lines,0,BUFFER_SIZE*BUFFER_SIZE);

    while (token != NULL)
    {

        //ver se deu certo isso valor de lines
        strcpy(lines[contLines], token);
        contLines ++;
        token = strtok(NULL, "\n");

    }

    //teste conteudo lines
    for(i=0;i<contLines;i++){
        printf("line %d  content %s ",contLines,lines[i]);
    }

    //partir o comando da lista de comandos em espaços e mandar para o fluxo
    for(i=0;i<contLines;i++){

         
        //partindo uma frase em tokens
        char *line[BUFFER_SIZE];
        int numOfTokens =0;
        int cont=0;
        memset(line,0,BUFFER_SIZE);
        token = strtok(lines[i]," ");
        while(token != NULL){
            strcpy(line[numOfTokens],token);
            numOfTokens++;
            token = strtok(NULL," ");
        }

        //tratar possibilidades de comando
        //se tiver apenas 1 palavra vai pra list
       if (numOfTokens > 0 && numOfTokens <= 3){
        
        location.x=atoi(line[1]);
        location.y=atoi(line[2]);
        locations[i]=location;

        char *operation = line[0];
        selectOperation(clientSocket,operation);

           

       }else if(numOfTokens == 0){
        //se o comando so tem 1 palavra eh pq eh list
        listLocations(clientSocket);
       }

       else{
           //comando invalido
           sendMessageToClient(clientSocket,"Invalid command");
       }

    }

    //tratar operações invalidas

}
*/



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

    //INICIALIZAÇÃO DO AMBIENTE
    initializeLocations(locations);

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

            char buf[BUFFER_SIZE] = "";
            memset(buf, 0, BUFFER_SIZE);
            size_t count = recv(csock, buf, BUFFER_SIZE - 1, 0);
            
            printf("[msg] %s, %d bytes: %s\n", caddrstr, (int)count, buf);

            //recuperar mensagem total
            char input[BUFFER_SIZE];
            memset(input, 0, BUFFER_SIZE);
            strcpy(input, buf);
            //printf(" mensagem contida em input :  %s", input);
            int i = 0;

            //filtro msg
            //int cont=0;
            //printf(" input tem tamanho : %d\n",strlen(input));
            
            for (i = 0; i <= strlen(input); i++)
            {
                if (input[i] == '\n')
                {
                    input[i] = 0;
                }
            }
            //printf(" input tem tamanho : %d\n",strlen(input));
            //printf(" mensagem contida em input :  %s", input);

            //verificar kill    
            if (strcmp(input, "kill") == 0)
            {
                logexit("killed server");
                close(csock);
                return -1;
            }
            /*
            char *token = strtok(input, "\n");

            

            while (token != NULL)
            {
                printf(" conteudo de token : %s\n", token);
                token = strtok(NULL, "\n");
            }
            */

            //execucao servidor "init"
            if ((int)count > BUFFER_SIZE) {
                close(csock);
            }


            //printf("Verificar valor de input : %s",input);   
            serverExec(input,csock);
            //sendMessageToClient(csock, "checando\n");
            
            //execucaoServidor(input,csock);
            
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