#ifndef ARTICLE_H_
#define ARTICLE_H_

#include <string>
#include <mutex>

class Article {
private:
	std::string title;
	std::string author;
	std::string text;
	size_t id;
public:
	Article(std::string, std::string, std::string, size_t);
	std::string getTitle();
	std::string getAuthor();
	std::string getText();
};



#endif /* INC_ARTICLE_H_ */
