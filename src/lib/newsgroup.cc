#include "newsgroup.h"
#include <iostream>
NewsGroup::NewsGroup(std::string name, size_t id)
{
	this->name = name;
	this->id = id;
}

bool NewsGroup::createArt(std::string title, std::string author, std::string text) {
	artVec.push_back(std::make_pair(artid, Article(title, author, text, artid)));
	artid++;
	return true;
}
int NewsGroup::getID() {
	return id;
}
bool NewsGroup::delete_Art(int art_id) { // 
	auto it = find_if(artVec.begin(), artVec.end(), [&art_id](const std::pair<int, Article>& p) {return p.first == art_id;});
	if(it != artVec.end()) {
		artVec.erase(it);
		artid--;
		return true;
	}
	return false;
}
std::vector<std::pair<int, Article>> NewsGroup::get_Art() {
	return artVec;
}
Article NewsGroup::get_Art(int art_id) {
	auto it = find_if(artVec.begin(), artVec.end(), [&art_id](const std::pair<int, Article>& p) {return p.first == art_id;});
	if(it != artVec.end()) {
		return it->second;
	}
	return Article("bad", "bad", "bad", -1); // ART DOES NOT EXIST
}
std::string NewsGroup::getName() {
	return name;
}
