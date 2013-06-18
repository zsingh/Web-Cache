#include <stdio.h>
#include <sys/socket.h>
#include <netdb.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <strings.h>

int main(int argc, char * argv[])
{
	int sock,sendStatus,recvStatus;
	int IPLength = 15;
	char *IP = (char *)malloc(IPLength+1);

	struct sockaddr_in *address;
	struct hostent *host;

	char *domainName, *stuff;
	char buffer[1024];
	int IPcheck;

	if(argc == 2){
		domainName = argv[1];
	}
	else{
		printf("Error, invalid/no argument for URL\n");
		return;
	}

	char def[25] = "/index.html";
	if((host = gethostbyname(domainName)) == NULL){
		printf("Error, Invalid domain. \n");
		return;
	}

	if(inet_ntop(AF_INET, (void *)host->h_addr_list[0], IP, IPLength) == NULL){
	printf("Error, IP cannot be created. \n");
	return;
		}

	printf("The domain we will retreive an object from: %s \n", domainName);
	printf("The given IP for domain %s is %s \n", domainName, IP);

	if(( sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 ){
		printf("Socket couldn't be created. \n");
		return;
	}

	printf("socket created! \n");

	address = (struct sockaddr_in *)malloc(sizeof(struct sockaddr_in*));
	address->sin_family = AF_INET;
	IPcheck = inet_pton(AF_INET, IP, (void*)(&(address->sin_addr.s_addr)));
	if (IPcheck < 0 || IPcheck == 0)
	{
		printf("inet_pton mess up! \n");
		return;
	}
	address->sin_port = htons(80);
	printf("connecting...\n");

	if( connect(sock, (struct sockaddr *)address, sizeof(struct sockaddr)) < 0){
		printf("Could not connect. \n");
		return;
	}

	printf("connected! \n");
	printf("sending HTTP request \n");

	sprintf(buffer, "GET %s HTTP/1.0\n\n", def);

	sendStatus = send(sock, buffer, sizeof(buffer), 0);
	if (sendStatus == -1){
		printf("Query sending failed. \n");
		return;
	}

	printf("query sent!");

	memset(buffer, 0, sizeof(buffer));
	while((recvStatus = recv(sock, buffer, sizeof(buffer), 0)) > 0){
		stuff = buffer;
		if(stuff != NULL){
			fprintf(stdout, stuff);
		}
		memset(buffer, 0, recvStatus);
	}
	return;
}

