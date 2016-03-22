#ifndef SERVER_H_
#define SERVER_H_

#include "newsgroup.h"
#include "article.h"
#include "connection.h"
#include <list>
#include <utility>
#include <memory>

class Server
{
private:
	std::list<std::pair<size_t, Newsgroup>> nglist;
	int port;
	bool ready;
public:
	Server(int);
	bool isReady() const;
	std::shared_ptr<Connection> waitForActivity() const;
	void registerConnection(const std::shared_ptr<Connection>& conn);
	void deregisterConnection(const std::shared_ptr<Connection>& conn);
	Server(const Server&) = delete;
	Server& operator=(const Server&) = delete;
};



#endif /* SERVER_H_ */
