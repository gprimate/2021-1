#include "common.h"
#include "constants.h"

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

void usage(int argc, char **argv) {
	printf("usage: %s <server IP> <server port>\n", argv[0]);
	printf("example: %s 127.0.0.1 51511\n", argv[0]);
	exit(EXIT_FAILURE);
}

int main(int argc, char **argv) {

	//se faltar argumentos ...
	if (argc < 3) {
		usage(argc, argv);
	}


	// storage
	struct sockaddr_storage storage;
	if (0 != addrparse(argv[1], argv[2], &storage)) {
		usage(argc, argv);
	}

	//socket
	int s;
	s = socket(storage.ss_family, SOCK_STREAM, 0);
	if (s == -1) {
		logexit("socket");
	}

	// conexao com server
	struct sockaddr *addr = (struct sockaddr *)(&storage);
	if (0 != connect(s, addr, sizeof(storage))) {
		logexit("connect");
	}

	// log de conexao
	char addrstr[BUFFER_SIZE];
	addrtostr(addr, addrstr, BUFFER_SIZE);
	printf("connected to %s\n", addrstr);

	//envio de mensagens
	
	char buf[BUFFER_SIZE];
	memset(buf, 0, BUFFER_SIZE);
	
	/*
	printf("mensagem> ");
	fgets(buf, BUFFER_SIZE-1, stdin);
	size_t count = send(s, buf, strlen(buf)+1, 0);
	if (count != strlen(buf)+1) {
		logexit("send");
	}
	*/
	//fim de envio de mensagem
	

	//buf recebe 0
	//memset(buf, 0, BUFFER_SIZE);
	unsigned total = 0;
	while(1) {

		//envio de mensagens
		char buf[BUFFER_SIZE] = "";
		memset(buf, 0, BUFFER_SIZE);
		printf("mensagem> ");
		fgets(buf, BUFFER_SIZE-1, stdin);
		if(buf == "kill"){
			break;
		}

		size_t count = send(s, buf, strlen(buf)+1, 0);
		if (count != strlen(buf)+1) {
			logexit("send");
		}
		
		char bufz[BUFFER_SIZE] = "";
		memset(bufz,0,BUFFER_SIZE);
		count = recv(s, bufz, BUFFER_SIZE, 0);
		//printf("tam de count : %d\n",count);
		printf("server : %s\n",bufz);


		if (count == 0 || count > 500) {
			// Connection terminated.
			
			break;
		}

		/*
		if(buf[0] ==1){
			break;
		}
		*/

		//total += count;
	}
	close(s);

	printf("received %u bytes\n", total);
	puts(buf);

	exit(EXIT_SUCCESS);
}