#include "article.h"

Article::Article(std::string title, std::string author, std::string text,
		size_t id)
{
	this->title = title;
	this->author = author;
	this->text = text;
	this->id = id;
}
