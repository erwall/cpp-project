#include "server.h"

Server::Server(int port)
{
	this->port = port;
	ready = true;
}

/* Removes all registered connections */

Server::~Server() {
	connections.clear();
}

/* Waits for activity on the port. Returns a previously registered
connection object if an existing client wishes to communicate,
nullptr if a new client wishes to communicate */

std::shared_ptr<Connection> Server::waitForActivity() const {
;
}

bool Server::isReady() const
{
	/* when is server not ready? */
	return ready;
}
/* Registers a new connection */
void Server::registerConnection(const std::shared_ptr<Connection>& conn)
{
	/* Check connection valid & stuff? */
	if(!conn->isConnected()) {
		connections.push_back(conn);
	}
}

void Server::deregisterConnection(const std::shared_ptr<Connection>& conn)
{
	/* Removes conn from connections */
	/* Unsure if works (fak eduroam) */
	auto start = remove_if(connections.begin(), connections.end(), [&conn](Connection c) { return c == conn });
	connections.erase(start,connections.end());
}

int main() {


	return 0;

}
