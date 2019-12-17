#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string>
#include <netdb.h>
#include <iostream>
#include <map>
#include <vector>

#define PORT "5544"
#define HOSTNAME "127.0.0.1"
#define MAXLINE 1024

using namespace std;

vector<string> split(string str, string token){
    vector<string>result;
    while(str.size()){
        int index = str.find(token);
        if(index!=string::npos){
            result.push_back(str.substr(0,index));
            str = str.substr(index+token.size());
            if(str.size()==0)result.push_back(str);
        }else{
            result.push_back(str);
            str = "";
        }
    }
    return result;
}

int main(){
  struct sockaddr_storage their_addr;
  socklen_t addr_size;
  struct addrinfo hints, *res;
  int sockfd;

  // first, load up address structs with getaddrinfo():

  memset(&hints, 0, sizeof hints);
  hints.ai_family = AF_UNSPEC;  // use IPv4 or IPv6, whichever
  hints.ai_socktype = SOCK_STREAM;
  hints.ai_flags = AI_PASSIVE;     // fill in my IP for me

  int status;
  if ((status = getaddrinfo(HOSTNAME, PORT, &hints, &res)) != 0) {
    fprintf(stderr, "getaddrinfo error: %s\n", gai_strerror(status));
    exit(1);
  }

  // make a socket:
  if ( (sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol)) < 0 ) {
      perror("socket creation failed");
      exit(EXIT_FAILURE);
  }


  // bind it to the port we passed in to getaddrinfo():
  addr_size = sizeof their_addr;
  bind(sockfd, res->ai_addr, res->ai_addrlen);
  listen(sockfd, 20);
  int recv_sock = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);

  map<string, string> m;
  char buffer[20];
  while(true){
    memset(&buffer, 0, sizeof buffer);
    recv(recv_sock, buffer, 20, 0);
    string s(buffer);
    if(s.compare("") == 0){
      recv_sock = accept(sockfd, (struct sockaddr *)&their_addr, &addr_size);
      continue;
    }
    vector<string> pair = split(s, ":");
    m[pair[0]] = pair[1];
    cout << pair[0] << ": " << pair[1] << endl;
  }

  return 0;
}