#ifndef ARTICLE_H_
#define ARTICLE_H_

#include <string>
#include <mutex>

class Article {
private:
	std::string title;
	std::string author;
	std::string text;
	int id;
public:
	Article(std::string, std::string, std::string, int);
	std::string getTitle();
	std::string getAuthor();
	std::string getText();
	int getID();
};



#endif /* INC_ARTICLE_H_ */
