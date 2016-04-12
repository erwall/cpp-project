#ifndef NEWSGROUP_H_
#define NEWSGROUP_H_
#include "article.h"
#include <algorithm>
#include <string>
#include <vector>
#include <iostream>
#include <utility>
class NewsGroup
{
private:
	std::string name;
	int id;
	int artid = 0;
	std::vector<std::pair<int, Article>> artVec;
public:
	NewsGroup(std::string, int);
	bool createArt(std::string, std::string, std::string);
	void remove(Article);
	Article get(std::string);
	Article get(int);
	int getID();
	std::string getName();
	std::vector<std::pair<int,Article>> get_Art();
	Article get_Art(int art_id);
	bool delete_Art(int id);
};



#endif /* NEWSGROUP_H_ */
