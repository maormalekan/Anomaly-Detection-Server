// Maor Malekan 321202962

#include "Server.h"

Server::Server(int port)throw (const char*) {
	stopMem = false;
	fd = socket(AF_INET, SOCK_STREAM, 0);
	if (fd < 0) {
		throw "socket failed";
	}
	server.sin_family = AF_INET;
	server.sin_addr.s_addr = INADDR_ANY;
	server.sin_port = htons(port);

	if (bind(fd,(struct sockaddr*)&server, sizeof(server)) < 0)	{
		throw "bind failed";
	}

	if (listen(fd, 2) < 0) {
		throw "listen failed";
	}
}

void Server::start(ClientHandler& ch)throw(const char*){
	t = new thread([&ch, this](){
		socklen_t firstClientSize = sizeof(client);
		while (!stopMem) {
			alarm(4);
			int firstClient = accept(fd, (struct sockaddr*)&client, &firstClientSize);
			if (firstClient < 0) {
				throw "accept failed";
			}
			ch.handle(firstClient);
			close(firstClient);
		}
	});
}

void Server::stop(){
	stopMem = true;
	t->join(); // do not delete this!
}

Server::~Server() {
	close(fd);
}

