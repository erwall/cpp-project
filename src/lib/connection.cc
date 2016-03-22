#include "connection.h"

Connection::Connection(const char* host, int port)
{
	this->host = host;
	this->port = port;
	connected = true;
}

Connection::Connection()
{
	host = nullptr;
	port = 0;
	connected = false;
}

bool Connection::isConnected() const
{
	return connected;

}

void Connection::write(unsigned char ch) const
{
	;
}

unsigned char Connection::read() const
{
	return '0';
}
