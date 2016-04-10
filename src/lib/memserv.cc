#include "memserv.h"
using namespace std;
MemServ::MemServ() {

}
bool MemServ::createNG(const string &news_group) {
	ngvec.push_back(NewsGroup(news_group,ngid)); // temp för test
	++ngid;
	return true;
}
bool MemServ::removeNG(int news_group_id) {
	ngvec.erase(ngvec.begin()+news_group_id);
	--ngid;
	return true;
}
std::vector<NewsGroup> MemServ::getNG() {
	return ngvec;
}
std::vector<Article> MemServ::get_Art(int news_group_id) {
	ngvec[news_group_id].get_Art();
}
Article MemServ::get_Art(int news_group_id, int art_id) {
	ngvec[news_group_id].get_Art(art_id);
}
bool MemServ::delete_Art(int news_group_id, int art_id) {
	ngvec[news_group_id].delete_Art(art_id)	;
	--artid;
	return true;
}
bool MemServ::createArt(int news_group_id, string title, string author, string text) {
	ngvec[news_group_id].emplace_back(title, author, text, artid);
	++artid;
}