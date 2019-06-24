#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <string.h>
#include <netdb.h>
#include <iostream>
#include <sstream>
#include <map>
#include <vector>

#define PORT "5544"
#define HOSTNAME "127.0.0.1"
#define MAXLINE 1024

using namespace std;


vector<string> getCommand(){
	string line;
	string token;
	vector<string> command;

	getline(std::cin, line);
	istringstream stream(line);
	while (stream >> token)
			command.push_back(token);
	return command;
}
// Driver code
int main() {
	struct addrinfo hints, *res;
	int sockfd;

	// first, load up address structs with getaddrinfo():

	memset(&hints, 0, sizeof hints);
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;

	getaddrinfo(HOSTNAME, PORT, &hints, &res);

	// make a socket:

	sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);

	// connect!
	connect(sockfd, res->ai_addr, res->ai_addrlen);

	vector<string> command;
	map<string, int> m;
	m.insert({"set", 0});
	m.insert({"clear", 1});
	int iterations = 0;
	while(true){
		cout << "[" << iterations << "]>>> ";
		command = getCommand();
		cout << command.size();
		switch(command.size()){
			case 1:{
				system("clear");
				iterations = 0;
				break;
			}
			case 2:{
				break;
			}
			case 3:{
				string msg = command[1]+":"+command[2];
				char data[command[1].length()+command[2].length()+2];
				strcpy(data, msg.c_str());
				send(sockfd, (char*)data, strlen(data), 0);
				iterations++;
				break;
			}
			default:
				continue;
		}
		
	}
	return 0;
}
