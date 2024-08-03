#include<stdio.h>
#include<string.h>
#include <stdlib.h>
#include <sys/socket.h>
#include <in.h>
#include <inet.h>
#include<unistd.h>
#define PORT 7890 // The port users will be connecting to



// Dumps raw memory in hex byte and printable split format
void dump(const unsigned char *data_buffer, const unsigned int length) {
 unsigned char byte;
 unsigned int i, j;
 for(i=0; i < length; i++) {
 byte = data_buffer[i];
 printf("%02x ", data_buffer[i]); // Display byte in hex.
 if(((i%16)==15) || (i==length-1)) {
 for(j=0; j < 15-(i%16); j++)
 printf(" ");
 printf("| ");
 for(j=(i-(i%16)); j <= i; j++) { // Display printable bytes from line.
 byte = data_buffer[j];
 if((byte > 31) && (byte < 127)) // Outside printable char range
 printf("%c", byte);
 else
 printf(".");
 }
 printf("\n"); // End of the dump line (each line is 16 bytes)
 } // End if
 } // End for
}

int main(void) {
 int sockfd, new_sockfd; // Listen on sock_fd, new connection on new_sockfd
 struct sockaddr_in host_addr, client_addr; // My address information
 socklen_t sin_size;
 int recv_length=1, yes=1;
 char buffer[1024];
 
 //pid = getpid();
 //printf("%d",pid);
 if ((sockfd = socket(PF_INET, SOCK_STREAM, 0)) == -1){perror("in socket");}
 //if (setsockopt(sockfd, SOL_SOCKET, SO_REUSEADDR, &yes, sizeof(int)) == -1){ perror("setting socket option SO_REUSEADDR");}
 
 host_addr.sin_family = AF_INET; // Host byte order , protocol 0 for standard
 host_addr.sin_port = htons(PORT); // Short, network byte order converting to big endian from little endian 2 bytes 
 host_addr.sin_addr.s_addr = 0; // Automatically fill with my IP. mainly its 127.0.0.1
 memset(&(host_addr.sin_zero), '\0', 8); // Zero the rest of the struct. fuilling the last 8 bytes with null.
 
 printf("%d",ntohs(host_addr.sin_port));
 printf("%s",inet_ntoa(host_addr.sin_addr));


 if (bind(sockfd, (struct sockaddr *)&host_addr, sizeof(struct sockaddr)) == -1) {printf("binding to socket");}
 
 if (listen(sockfd, 5) == -1){ printf("listening on socket");}
 
 while(1) { // Accept loop.
 	sin_size = sizeof(struct sockaddr_in);
 	new_sockfd = accept(sockfd, (struct sockaddr *)&client_addr, &sin_size);
 	if(new_sockfd != -1)
 	printf("accepting connection");
	printf("server: got connection from %s port %d\n",inet_ntoa(client_addr.sin_addr), ntohs(client_addr.sin_port));
	send(new_sockfd, "Hello, world!\n", 13, 0);
	//send(new_sockfd,Msg,strlen(Msg),0);
 	recv_length = recv(new_sockfd, &buffer, 1024, 0);
 	while(recv_length > 0) {
 		printf("RECV: %d bytes\n", recv_length);
 		dump(buffer, recv_length);
		 recv_length = recv(new_sockfd, &buffer, 1024, 0);
 		}
	 close(new_sockfd);
 }
 return 0;
}








