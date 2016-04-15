#ifndef DISKSERV_H
#define DISKSERV_H
#include "server.h"
#include "article.h"
#include "newsgroup.h"
#include <iostream>
#include <vector>
#include <algorithm>
#include <cstdio>
#include <unistd.h>
#include <string>
#include <stdlib.h>
#include <string>
#include <iostream>
#include <cstdlib>
#include <fstream>
#include <sys/stat.h>
class DiskServ {
	public:
		DiskServ();
		bool createNG(const std::string& news_group);
		bool removeNG(int news_group_id);
		bool createArt(int news_group_id, std::string title, std::string author, std::string text);
		std::vector<std::pair<int,NewsGroup>> getNG();
		std::vector<std::pair<int, Article>> get_Art(int news_group_id);
		Article get_Art(int news_group_id, int art_id);
		int delete_Art(int news_group_id, int art_id);
		void listArt(int news_group_id);
		NewsGroup getNG(int id);
		void listAll(); // for test
		std::string replace_white_space(std::string& news_group);
		std::string replace_underscore(std::string& news_group);
	private:
		//std::vector<std::pair<int, NewsGroup>> ngvec;
		int ngid=0;
		int artid=0; // kan ta bort?
};
#endif