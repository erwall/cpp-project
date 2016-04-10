#ifndef MEMSERV_H
#define MEMSERV_H
#include "server.h"
#include "article.h"
#include "newsgroup.h"
#include <iostream>
#include <vector>
#include <string>
class MemServ {
	public:
		MemServ();
		//~MemServ();
		bool createNG(const std::string& news_group);
		bool removeNG(int news_group_id);
		bool createArt(int news_group_id, std::string title, std::string author, std::string text);
		std::vector<NewsGroup> getNG();
		std::vector<Article> get_Art(int news_group_id);
		Article get_Art(int news_group_id, int art_id);
		bool delete_Art(int news_group_id, int art_id);
	private:
		std::vector<NewsGroup> ngvec;
		int ngid=0;
		int artid=0;
};
#endif