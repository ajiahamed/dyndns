#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <strings.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include "config.h"

int main(int argc, char *argv[]) {
	config_t config;
	int sockfd, portno, n;
	struct sockaddr_in srv_addr;
	struct hostent *server;
	char buffer[256];

	if(argc < 3) {
		fprintf(stderr, "%s -c <configfile>\n\t -h print this help\n", argv[0]);
		exit(1);
	}
	if(strcmp(argv[1], "-c") != 0) {
		fprintf(stderr, "%s -c <configfile>\n\t -h print this help\n", argv[0]);
		exit(1);
	}
	if(ReadCFG(&config, argv[2]) == false) {
		fprintf(stderr, "Error reading file: %s\n", argv[2]);
		exit(1);
	}
	portno = config.client.port;
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0)
		perror("Error opening socket");
	server = gethostbyname(config.client.host);
	if(server == NULL) {
		fprintf(stderr, "Unknown host\n");
		exit(1);
	}
	bzero((char *) &srv_addr, sizeof(srv_addr));
	srv_addr.sin_family = AF_INET;
	bcopy((char *) server->h_addr, (char *) &srv_addr.sin_addr.s_addr, server->h_length);
	srv_addr.sin_port = htons(portno);
	if(connect(sockfd, (struct sockaddr *) &srv_addr, sizeof(srv_addr)) < 0) {
		fprintf(stderr, "Error connecting to server\n");
		exit(1);
	}
	bzero(buffer, 256);
	strcpy(buffer, config.client.domain);
	n = write(sockfd, buffer, strlen(buffer) +1);
	if(n < 0) {
		fprintf(stderr, "Error sending data.");
		exit(1);
	}
	return 0;
}