#ifndef NEWSGROUP_H_
#define NEWSGROUP_H_
#include "article.h"
#include <string>
#include <vector>

class NewsGroup
{
private:
	std::string name;
	int id;
	std::vector<Article> artVec;
public:
	NewsGroup(std::string, size_t);
	void emplace_back(std::string, std::string, std::string, int);
	void push_back(Article);
	void remove(Article);
	Article get(std::string);
	Article get(int);
	int getID();
	std::string getName();
	std::vector<Article> get_Art();
	Article get_Art(int art_id);
	bool delete_Art(int id);
};



#endif /* NEWSGROUP_H_ */
