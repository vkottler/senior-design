#include <stdio.h>
#include <stdlib.h> 
#include <sys/socket.h> 
#include <sys/types.h> 
#include <netinet/in.h> 
#include <arpa/inet.h>
#include <unistd.h>

#include <string.h> 

#define UNUSED(x) ((void)(x))

int main(int argc, char **argv)
{
	puts("Running Client: \n");
	UNUSED(argc);
	UNUSED(argv);
	
	//CREATE A SOCKET 
	int network_socket; 
	network_socket = socket(AF_INET, SOCK_STREAM, 0);

	//SPECIFY AN ADDRESS FOR THE SOCKET 
	struct sockaddr_in server_address; 
	server_address.sin_family = AF_INET;
	server_address.sin_port = htons(5000);
	server_address.sin_addr.s_addr = inet_addr("10.42.0.79");

	int connection_status = connect(network_socket,(struct sockaddr *) &server_address,sizeof(server_address));
	if(connection_status == -1){
		perror("Error making connection to socket\n");
	}
	else
	{
		printf("Connected to server\n");
	}
	int accel[3] = {10000,20000,30000};
	send(network_socket,accel,sizeof(int)*3,0);

	//receive data from server
	//char server_response[256];
	//recv(network_socket,&server_response,sizeof(server_response),0);

	//print out server's response 
	//printf("Server send: %s\n", server_response);

	//close(network_socket);
	
	//CHANNELS AND MANIFEST CREATED, NEED TO SEND TO SERVER 

	//NEED TO SEND PACKETS TO SERVER 

	return 0;
}
