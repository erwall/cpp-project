#include "server.h"

Server::Server(int port)
{
	this->port = port;
	ready = true;
}

bool Server::isReady() const
{
	return ready;
}

void Server::registerConnection(const std::shared_ptr<Connection>& conn)
{
	;
}

void Server::deregisterConnection(const std::shared_ptr<Connection>& conn)
{
	;
}

int main() {


	return 0;

}
