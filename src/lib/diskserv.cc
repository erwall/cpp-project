#include "diskserv.h"
#include <stdlib.h>
#include <string>
#include <iostream>
#include <fstream>
using namespace std;
DiskServ::DiskServ() {
	int ret = system("mkdir diskdatabase"); 
	if(ret == 0) {
		cout << "Created diskdatabase folder" << endl;
	}
	else if(ret == 256) {
		cout << "Diskdatabase folder already existed, should read everything" << endl; // mb
		ifstream is("diskdatabase/ng.txt");
		string ngs;
		int id;
		while(is >> ngs >> id) { // ser till att ngid är 1 över föregående gången databasen användes
		}
		ngid=id;
		ngid++;
		cout << "ngid since previous use: " << ngid << endl;
		is.close();
	}
	createNG("h"); // temp för test
	createNG("y");
	createNG("he"); // temp för test
	createNG("yo");
	createNG("hej"); // temp för test
	createNG("yol");
	vector<pair<int,NewsGroup>> ngvec = getNG();
	removeNG(0);
	removeNG(5);
	//listAll();
	createArt(1, "hej", "pa", "dej");
	createArt(1, "hej", "pa", "dej");
	createArt(1, "hej", "pa", "dej");
	createArt(2, "swag", "yo", "face");
}
bool DiskServ::createNG(const std::string& news_group) {
	string s = "mkdir diskdatabase/";
	s+=news_group;
	int ret = system(s.c_str());
	if(ret == 0) {
		ofstream os("diskdatabase/ng.txt", ios::app);
		os << news_group << " " << ngid;
		os << "\n";
		os.close();
		ngid++;
		return true;
	}
	else if(ret == 256) {
		return false;
	}
}
vector<pair<int,NewsGroup>> DiskServ::getNG() {
	vector<pair<int, NewsGroup>> ngvec;
	string ngs;
	int id;
	ifstream is("diskdatabase/ng.txt");
	while(is >> ngs >> id) {
		ngvec.push_back(make_pair(id,NewsGroup(ngs,id)));
	}
	is.close();
	return ngvec;
}
bool DiskServ::removeNG(int news_group_id) {
	//check exist (true remove | false)
	string ngs;
	int id;
	ifstream is("diskdatabase/ng.txt");
	while(is >> ngs >> id) {
		if(id == news_group_id) {
			is.close();
			vector<pair<int, NewsGroup>> ngvec = getNG();
			// remove folder & ur ng.txt
			string rem = "rm -r diskdatabase/";
			rem+=ngs; // remove identifier är id, folders är efter name
			int ret = system(rem.c_str());
			if(ret == 0) { // remove folder success
				if(remove("diskdatabase/ng.txt") == 0) { // remove file success
					cout << "file removed" << endl;
					ofstream os("diskdatabase/ng.txt", ofstream::out);
					for(pair<int, NewsGroup> p: ngvec) {
						if(!(p.first == news_group_id)) {
							os << p.second.getName();
							os << " ";
							os << p.first;
							os << "\n";
						}
					}
					os.close();
				}
				else {
					cout << "ng.txt was not removed correctly" << endl;
				}
			}
			else if(ret == 256) { // remove fail, mega error
				cout << "Something happened in removeNG, existing NG could not be removed" << endl;
			}
		}
	}
	return false; // news group does not exist
}
void DiskServ::listAll() { // for test
	vector<pair<int, NewsGroup>> ngvec = getNG();
	for(pair<int, NewsGroup> p: ngvec) {
		cout << p.first << endl;
		cout << p.second.getName() << endl;
	}
}
bool DiskServ::createArt(int news_group_id, string title, string author, string text) {
	string ngs, a, t, txt, path;
	int id, aid = -1;
	cout << "entered" << endl;
	ifstream is("diskdatabase/ng.txt");
	while(is >> ngs >> id) {
		cout << "entered2" << endl;
		if(news_group_id == id) { // NG exists => create article, borde läsa vad artid är i denna gruppen
			cout << "entered3" << endl;
			is.close();
			path = "diskdatabase/";
			path+=ngs;
			path+="/artid.txt";
			cout << "path: " << path << endl; 
			ifstream is(path);
			while(is >> aid >> t >> a >> txt) { // läs sista previous artid
			}
			aid = aid+1;
			ofstream os(path, ios::app);
			os << aid << " " << " " << title << " " << author << " " << text;
			os << "\n";
			os.close();
			return true;
		}
	}
	return false; // ng does not exist
}