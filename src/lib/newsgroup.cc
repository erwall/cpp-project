#include "newsgroup.h"

Newsgroup::Newsgroup(std::string name, size_t id)
{
	this->name = name;
	this->id = id;
}

void Newsgroup::emplace_back(std::string title, std::string author, std::string text,
		size_t id)
{
	artlist.emplace_back(title, author, text, id);
}

void Newsgroup::push_back(Article article)
{
	artlist.push_back(article);
}
