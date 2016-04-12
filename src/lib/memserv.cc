#include "memserv.h"
using namespace std;
MemServ::MemServ() {

}
bool MemServ::createNG(const string &news_group) {
	auto it = find_if(ngvec.begin(), ngvec.end(), [&news_group](pair<int, NewsGroup>& p) {return p.second.getName() == news_group;}); // borde kanske var const pair, men måste ändra getName() och annat skräp till const, funkade inte atm
	if(it == ngvec.end()) {
		ngvec.push_back(make_pair(ngid, NewsGroup(news_group,ngid)));
		++ngid;
		return true;
	}
	return false;
}
bool MemServ::removeNG(int news_group_id) {
	auto it = find_if(ngvec.begin(), ngvec.end(), [&news_group_id](const pair<int, NewsGroup>& p) {return p.first == news_group_id;});
	if(it != ngvec.end()) {
		ngvec.erase(it);
		return true;
	}
	return false;
}
vector<pair<int, NewsGroup>> MemServ::getNG() {
	return ngvec;
}
NewsGroup MemServ::getNG(int news_group_id) {
	auto it = find_if(ngvec.begin(), ngvec.end(), [&news_group_id](const pair<int, NewsGroup>& p) {return p.first == news_group_id;});
	if(it != ngvec.end()) {
		return it->second;
	}
	return NewsGroup("BADBOY", -1);
}
void MemServ::listArt(int news_group_id) { // test purposes only
	auto it = find_if(ngvec.begin(), ngvec.end(), [&news_group_id](const pair<int, NewsGroup>& p) {return p.first == news_group_id;});
	if(it != ngvec.end()) {
		NewsGroup ng = it->second;
		vector<pair<int, Article>> artvec = ng.get_Art();
		for(pair<int, Article> p: artvec) {
			cout << "Author: " << p.second.getAuthor() << endl;
		}
	}
}
vector<pair<int, Article>> MemServ::get_Art(int news_group_id) {
	auto it = find_if(ngvec.begin(), ngvec.end(), [&news_group_id](const pair<int, NewsGroup>& p) {return p.first == news_group_id;});
	if(it != ngvec.end()) {
		return it->second.get_Art();
	}
}
Article MemServ::get_Art(int news_group_id, int art_id) {
	auto it = find_if(ngvec.begin(), ngvec.end(), [&news_group_id](const pair<int, NewsGroup>& p) {return p.first == news_group_id;});
	if(it != ngvec.end()) {
		return it->second.get_Art(art_id);
	}
	return Article("bad","bad","bad",-2); // NG_DOES_NOT_EXIST
}
int MemServ::delete_Art(int news_group_id, int art_id) {
	auto it = find_if(ngvec.begin(), ngvec.end(), [&news_group_id](const pair<int, NewsGroup>& p) {return p.first == news_group_id;});
	if(it != ngvec.end()) {
		if(it->second.delete_Art(art_id)) {
			return 1; // passed
		}
		return -1; // non-existing article
	}
	return -2; // non-existing group
}
bool MemServ::createArt(int news_group_id, string title, string author, string text) {
	auto it = find_if(ngvec.begin(), ngvec.end(), [&news_group_id](const pair<int, NewsGroup>& p) {return p.first == news_group_id;});
	if(it != ngvec.end()) {	
		it->second.createArt(title, author, text); // ingen returnc
		return true;
	}
	return false; // ng does not exst
}