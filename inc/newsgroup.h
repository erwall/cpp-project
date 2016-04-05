#ifndef NEWSGROUP_H_
#define NEWSGROUP_H_
#include "article.h"
#include <string>
#include <list>

class Newsgroup
{
private:
	std::string name;
	size_t id;
	std::list<Article> artlist;
public:
	Newsgroup(std::string, size_t);
	void emplace_back(std::string, std::string, std::string, size_t);
	void push_back(Article);
	void remove(Article);
	Article get(std::string);
	Article get(size_t);

};



#endif /* NEWSGROUP_H_ */
