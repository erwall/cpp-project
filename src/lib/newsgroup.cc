#include "newsgroup.h"
#include <iostream>
NewsGroup::NewsGroup(std::string name, size_t id)
{
	this->name = name;
	this->id = id;
}

void NewsGroup::emplace_back(std::string title, std::string author, std::string text)
{
	artVec.emplace_back(Article(title, author, text, artid));
	artid++;
}

void NewsGroup::push_back(Article article)
{
	artVec.push_back(article);
	artid++;
}
int NewsGroup::getID() {
	return id;
}
bool NewsGroup::delete_Art(int id) { // 
	artVec.erase(artVec.begin()+id);
	artid--;
	std::cout << "Erased article: " << id << std::endl;
	for(Article a: artVec) {
		std::cout << "After erase: " << a.getAuthor() << std::endl;
	}
	return true;
}
std::vector<Article> NewsGroup::get_Art() {
	return artVec;
}
Article NewsGroup::get_Art(int art_id) {
	std::cout << "returned author: " << artVec[art_id].getAuthor() << std::endl;
	return artVec[art_id];
}
std::string NewsGroup::getName() {
	return name;
}
