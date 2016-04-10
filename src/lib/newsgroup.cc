#include "newsgroup.h"

NewsGroup::NewsGroup(std::string name, size_t id)
{
	this->name = name;
	this->id = id;
}

void NewsGroup::emplace_back(std::string title, std::string author, std::string text,
		int id)
{
	artVec.emplace_back(Article(title, author, text, id));
}

void NewsGroup::push_back(Article article)
{
	artVec.push_back(article);
}
int NewsGroup::getID() {
	return id;
}
bool NewsGroup::delete_Art(int id) {
	artVec.erase(artVec.begin()+id);
}
std::vector<Article> NewsGroup::get_Art() {
	return artVec;
}
Article NewsGroup::get_Art(int art_id) {
	return artVec[art_id];
}
std::string NewsGroup::getName() {
	return name;
}
