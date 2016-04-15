#include "diskserv.h"

using namespace std;
DiskServ::DiskServ() {
	string p = "diskdatabase";
	int ret = mkdir(p.c_str(), 0700);
	if(ret == 0) {
		cout << "Created diskdatabase folder" << endl;
	}
	else {
		cout << "Diskdatabase folder already existed, should read everything" << endl; // mb
		string tmp, ngs, inp, tmp2;
		int id = 0;
		ifstream is("diskdatabase/ng.txt");
		while(getline(is,tmp)) {
			inp+=tmp;
		}
		is.close();
		while(inp.find("ngname:") != string::npos) {
			size_t begng = inp.find("ngname:");
			size_t endng = inp.find("endngname:");
			ngs = inp.substr(7,endng-7); // andra argument = length
			size_t begid = inp.find("id:");
			size_t endid = inp.find("endid:");
			tmp2 = inp.substr(begid+3, endid-(begid+3));
			id = stoi(tmp2);
			ngid = id;
			inp.erase(begng,endid+6);
		}
		ngid++;
		cout << "ngid since previous use: " << ngid << endl;
	}
}
bool DiskServ::createNG(const std::string& news_group) { // should befine
	string s = "diskdatabase/";
	s+=news_group;
	int ret = mkdir(s.c_str(), 0700);
	if(ret == 0) {
		ofstream os("diskdatabase/ng.txt", ios::app);
		os << "ngname:"; 
		os << news_group; 
		os << "endngname:";
		os << "id:";
		os << ngid;
		os << "endid:";
		os << "\n";
		os.close();
		ngid++;
		return true;
	}
	else {
		return false;
	}
}
vector<pair<int,NewsGroup>> DiskServ::getNG() {  // should be fine
	vector<pair<int, NewsGroup>> ngvec;
	string tmp, inp, ngs, tmp2;
	int id;
	ifstream is("diskdatabase/ng.txt");
	while(getline(is,tmp)) {
		inp+=tmp;
	}
	is.close();
	while(inp.find("ngname:") != string::npos) {
		size_t begng = inp.find("ngname:");
		size_t endng = inp.find("endngname:");
		ngs = inp.substr(7,endng-7); // andra argument = length
		size_t begid = inp.find("id:");
		size_t endid = inp.find("endid:");
		tmp2 = inp.substr(begid+3, endid-(begid+3));
		id = stoi(tmp2);
		inp.erase(begng,endid+6);
		ngvec.push_back(make_pair(id, NewsGroup(ngs,id)));
	}
	return ngvec;
}
bool DiskServ::removeNG(int news_group_id) { // should be fine
	//check exist (true remove | false)
	NewsGroup ng = getNG(news_group_id); // ng didnt exist
	if(ng.getID() == -1) {
		return false;
	}
	string ngpath = "diskdatabase/";
	ngpath+=ng.getName();
	string artpath = "diskdatabase/";
	artpath+=ng.getName();
	artpath+="/artid.txt";
	unlink(artpath.c_str());
	remove(ngpath.c_str());
	vector<pair<int, NewsGroup>> ngvec = getNG();
	ofstream os("diskdatabase/ng.txt", ofstream::out); // rewrite ng.txt
	for(pair<int, NewsGroup> p: ngvec) {
		if(p.second.getID() != news_group_id) {
			os << "ngname:"; 
			os << p.second.getName();
			os << "endngname:";
			os << "id:";
			os << p.second.getID();
			os << "endid:";
			os << "\n";
		}
	}
	os.close();
	return true; // done
}
void DiskServ::listAll() { // for test
	vector<pair<int, NewsGroup>> ngvec = getNG();
	for(pair<int, NewsGroup> p: ngvec) {
		cout << p.first << endl;
		cout << p.second.getName() << endl;
	}
}
bool DiskServ::createArt(int news_group_id, string title, string author, string text) {
	NewsGroup ng = getNG(news_group_id);
	if(ng.getID() == -1) {
		return false; // ng does not exist
	}
	string ngs, a, t, txt, path, tmp, tmp2;
	int id, aid = 0;
	ngs = ng.getName();
	path = "diskdatabase/";
	path+=ngs;
	path+="/artid.txt";
	vector<pair<int, Article>> artvec = get_Art(news_group_id); 
	if(artvec.size() == 0) {
		aid = 0;
	}
	else {
		pair<int, Article> p = artvec.back();
		Article article = p.second;
		aid = article.getID();
		aid++;
	}

	ofstream os(path, ios::app);
	os << "title:";
	os << title;
	os << "endtitle:";
	os << "author:";
	os << author;
	os << "endauthor:";
	os << "text:";
	os << text;
	os << "endtext:";
	os << "id:";
	os << aid;
	os << "endid:";
	os << "\n";
	os.close();
	return true;
}
vector<pair<int, Article>> DiskServ::get_Art(int news_group_id) { // check
	string tmp, inp, ngs, path, auth, title, text, tmp2;
	int id, aid;
	vector<pair<int, Article>> artvec;
	NewsGroup ng = getNG(news_group_id);
	if(ng.getID() == -1) {
		return artvec;
	}	
	path = "diskdatabase/";
	path += ng.getName();
	path += "/artid.txt";
	ifstream is(path);
	while(getline(is,tmp)) {
		inp+=tmp;
		inp+="\n";
	}
	is.close();
	while(inp.find("title:") != string::npos) {
			size_t begtitle = inp.find("title:");
			size_t endtitle = inp.find("endtitle:");
			title = inp.substr(begtitle+6, endtitle-(begtitle+6));
			size_t begauthor = inp.find("author:");
			size_t endauthor = inp.find("endauthor:");
			auth = inp.substr(begauthor+7, endauthor-(begauthor+7));
			size_t begtext = inp.find("text:");
			size_t endtext = inp.find("endtext:");
			text = inp.substr(begtext+5, endtext-(begtext+5));
			size_t begid = inp.find("id:");
			size_t endid = inp.find("endid:");
			tmp2 = inp.substr(begid+3, endid-(begid+3));
			aid = stoi(tmp2);
			inp.erase(begtitle,endid+6);
			artvec.push_back(make_pair(aid, Article(title,auth,text,aid)));
	}
	return artvec;
}
NewsGroup DiskServ::getNG(int news_group_id) { // should be fine
	vector<pair<int,NewsGroup>> ngvec = getNG();
	for(pair<int,NewsGroup> p: ngvec) {
		if(p.first == news_group_id) {
			return p.second;
		}
	}
	return NewsGroup("badboy",-1); // ng didn't exist
}
Article DiskServ::get_Art(int news_group_id, int art_id) {
	string ngs, path, auth, text, title;
	int id, artid;
	NewsGroup ng = getNG(news_group_id);
	if(ng.getID() == -1) {
		return Article("BAD","BAD","BAD",-2); // ng didn't exist
	}
	vector<pair<int, Article>> artvec = get_Art(news_group_id);
	for(pair<int, Article> p: artvec) {
		if(p.first == art_id) {
			return Article(p.second.getTitle(),p.second.getAuthor(),p.second.getText(),p.second.getID());
		}
	}
	return Article("BAD","BAD","BAD", -1); // article didn't exist
}
int DiskServ::delete_Art(int news_group_id, int art_id) {
	NewsGroup ng = getNG(news_group_id);
	Article a = get_Art(news_group_id, art_id);
	int ret = a.getID();
	if(ret == -1 || ret == -2) { // NG or ART doesn't exist
		return ret;
	}
	string path = "diskdatabase/";
	path+=ng.getName();
	path+="/artid.txt";
	vector<pair<int,Article>> artvec = get_Art(news_group_id);
	ofstream os(path, ofstream::out);
	for(pair<int,Article> p: artvec) {
		if(p.second.getID() != art_id) {
			os << "title:";
			os << p.second.getTitle();
			os << "endtitle:";
			os << "author:";
			os << p.second.getAuthor();;
			os << "endauthor:";
			os << "text:";
			os << p.second.getText();
			os << "endtext:";
			os << "id:";
			os << p.second.getID();
			os << "endid:";
			os << "\n";
		}
	}
	return 1;	
}
