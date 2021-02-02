#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <pthread.h>

#define PORT 8080
#define MAX_MESSAGE_SIZE 256

void *client_start(void *HOST) {
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	// Set up struct for socket info
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = inet_addr(HOST);
	
	int connection_status = connect(network_socket, (struct socketaddr *) &server_address, 
							sizeof(server_address));
	if (connection_status == -1) {
		printf("Socket connection error!\n");
		exit(1);
	}	
	
	char server_response[MAX_MESSAGE_SIZE];
	
	// Wait for message or close connection
	while (strcmp(server_response, "CLOSE_CONN") != 0) {
		recv(network_socket, &server_response, sizeof(server_response), 0);
		if (strcmp(server_response, "") != 0) {
			printf("REMOTE -> %s\n", server_response);		
		}
	}
	
	printf("\nREMOTE closed the connection\n");
	close(network_socket);
	exit(1);
}

void *server_start(void *HOST) {
	int server_socket;
	char *msg = malloc(MAX_MESSAGE_SIZE);
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	// Set up struct for socket info
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = inet_addr(HOST);
	
	// Bind and listen
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	listen(server_socket, 5);
	printf("Listening for connection from %s...\n", HOST);
	
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);
	printf("Connection Established to %s...\n\n", HOST);
	
	while (strcmp(msg, "CLOSE_CONN") != 0) {
		fgets(msg, MAX_MESSAGE_SIZE, stdin);
		printf("\nYOU -> %s\n", msg);
		send(client_socket, msg, MAX_MESSAGE_SIZE, 0);
	}
	
	free(msg);
	exit(0);
}

int main(int argc, char const *argv[]) {
	// Command line arg check
	if (argc < 2) {
		printf("No agrument supplied: Local IPV4\n");
		exit(1);
	}
	else if (argc < 3) {
		printf("No argument supplied: Remote IPV4\n");
		exit(1);
	}
	
	// Set up threads
	pthread_t server_id;
	pthread_t client_id;
	
	pthread_create(&server_id, NULL, server_start, (void *) argv[1]);
	pthread_create(&client_id, NULL, client_start, (void *) argv[2]);
	
	pthread_join(server_id, NULL);
	pthread_join(client_id, NULL);

	return 0;
}

