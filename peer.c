#include <unistd.h> 
#include <stdio.h> 
#include <sys/socket.h> 
#include <stdlib.h> 
#include <netinet/in.h> 
#include <string.h>
#include <pthread.h>

#define PORT 8080

void *client_start() {
	int network_socket;
	network_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	int connection_status = connect(network_socket, (struct socketaddr *) &server_address, 
							sizeof(server_address));	
	if (connection_status == -1) {
		printf("Socket connection error...\n");
	}	
	
	printf("CLIENT STARTED\n");
	
	char server_response[256];
	
	while (strcmp(server_response, "CLOSE_CONN") != 0) {
		recv(network_socket, &server_response, sizeof(server_response), 0);
		if (strcmp(server_response, "") != 0) {
			printf("REMOTE -> %s\n", server_response);		
		}
	}
	
	printf("REMOTE closed the connection");
	close(network_socket);
}

void *server_start() {
	char test_message[256] = "Test message!!!";
	
	int server_socket;
	server_socket = socket(AF_INET, SOCK_STREAM, 0);
	
	struct sockaddr_in server_address;
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(PORT);
	server_address.sin_addr.s_addr = inet_addr("127.0.0.1");
	
	bind(server_socket, (struct sockaddr*) &server_address, sizeof(server_address));
	listen(server_socket, 5);
	
	printf("SERVER STARTED\n");
	
	int client_socket;
	client_socket = accept(server_socket, NULL, NULL);
	
	for (int i = 0; i < 10; i++) {
		sleep(1);
		send(client_socket, test_message, sizeof(test_message), 0);
	}
}

int main(int argc, char const *argv[]) {
	pthread_t client_id;
	pthread_t server_id;
	pthread_create(&server_id, NULL, server_start, NULL);
	pthread_create(&client_id, NULL, client_start, NULL);
	pthread_join(client_id, NULL);
	pthread_join(server_id, NULL);
	return 0;
}

