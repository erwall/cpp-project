#ifndef CONNECTION_H
#define CONNECTION_H

typedef struct ConnectionClosedException {

} ConnectionClosedException;

class Connection
{
private:
	const char* host;
	int port;
	bool connected;
public:
	Connection(const char* host, int port);
	Connection();
	bool isConnected() const;
	void write(unsigned char ch) const;
	unsigned char read() const;
	Connection(const Connection&) = delete;
	Connection& operator=(const Connection&) = delete;
};

#endif
