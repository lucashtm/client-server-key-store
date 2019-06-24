#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <iostream>

#define PORT 5544
#define MAXLINE 1024

int main(){
  int sockfd;
  char buffer[MAXLINE];
  char *hello = "Hello ";
  struct sockaddr_in servaddr, cliaddr;

  // Creating socket file descriptor
  if ( (sockfd = socket(AF_INET, SOCK_DGRAM, 0)) < 0 ) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
  }

  memset(&servaddr, 0, sizeof(servaddr));
  memset(&cliaddr, 0, sizeof(cliaddr));

  // Filling server information
  servaddr.sin_family    = AF_INET; // IPv4
  servaddr.sin_addr.s_addr = INADDR_ANY;
  servaddr.sin_port = htons(PORT);

  // Bind the socket with the server address
  if ( bind(sockfd, (const struct sockaddr *)&servaddr, sizeof(servaddr)) < 0 ){
      perror("bind failed");
      exit(EXIT_FAILURE);
  }


  std::cout << "Server is listening on port " << PORT << std::endl;
  int n;
  socklen_t len;
  while(true){

    n = recvfrom(sockfd, (char *)buffer, MAXLINE,
                MSG_WAITALL, ( struct sockaddr *) &cliaddr,
                &len);
    buffer[n] = '\0';
    std::cout << "Received from client: " << buffer << std::endl;
    
    char* full_text;
    full_text = (char*)malloc(strlen(hello)+strlen(buffer)+1); 
    strcpy(full_text, hello ); 
    strcat(full_text, buffer);
    sendto(sockfd, (const char *)full_text, strlen(full_text),
        MSG_CONFIRM, (const struct sockaddr *) &cliaddr,
            len);
  }

  return 0;
}