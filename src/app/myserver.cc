/* myserver.cc: sample server program */
#include "memserv.h"
#include "protocol.h"
#include "diskserv.h"
#include "messagehandler.h"

#include "server.h"
#include "connection.h"
#include "connectionclosedexception.h"
#include "memserv.h"
#include <memory>
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;
void read_com_end(MessageHandler& mh, Server& server, const shared_ptr<Connection>& conn) {
	unsigned char c = mh.readCode();
	if(c != Protocol::COM_END) {
		server.deregisterConnection(conn);
		cout << "Client didn't follow protocol" << endl;
	}
}
void read_par_num(MessageHandler& mh, Server& server, const shared_ptr<Connection>& conn) {
	unsigned char c = mh.readCode();
	if(c != Protocol::PAR_NUM) {
		server.deregisterConnection(conn);
		cout << "Client didn't follow protocol" << endl;
	}
}
void read_par_string(MessageHandler& mh, Server& server, const shared_ptr<Connection>& conn) {
	unsigned char c = mh.readCode();
	if(c != Protocol::PAR_STRING) {
		server.deregisterConnection(conn);
		cout << "Client didn't follow protocol" << endl;
	}
}
int main(int argc, char* argv[]){
	if (argc != 2) {
		cerr << "Usage: myserver port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[1]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Server server(port);
	if (!server.isReady()) {
		cerr << "Server initialization error." << endl;
		exit(1);
	}
	//MemServ ms;
	DiskServ ms;																								;
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			MessageHandler mh(*conn.get());
			try {
				unsigned char cmd = mh.readCode();
				if(cmd == Protocol::COM_LIST_NG) {  
					vector<pair<int, NewsGroup>> ngvec = ms.getNG();
					read_com_end(mh,server,conn); // com_end
					mh.writeCode(Protocol::ANS_LIST_NG);
					mh.writeCode(Protocol::PAR_NUM);
					mh.writeNumber(ngvec.size());
					for(pair<int, NewsGroup> p: ngvec) {
						mh.writeCode(Protocol::PAR_NUM);
						mh.writeNumber(p.first);
						mh.writeCode(Protocol::PAR_STRING);
						mh.writeNumber(p.second.getName().size());
						mh.writeString(p.second.getName());
					}
					mh.writeCode(Protocol::ANS_END);
				}
				
				else if(cmd == Protocol::COM_CREATE_NG) {  
					
					read_par_string(mh, server, conn); // PAR_STRING
				    int sz = mh.readNumber(); // längd av sträng
					string title = mh.readString(sz);  // sträng
					read_com_end(mh, server, conn); // com_end

					mh.writeCode(Protocol::ANS_CREATE_NG);
					if(ms.createNG(title)) {
						mh.writeCode(Protocol::ANS_ACK);
					}
					else {
						mh.writeCode(Protocol::ANS_NAK);
						mh.writeCode(Protocol::ERR_NG_ALREADY_EXISTS);
					}
					mh.writeCode(Protocol::ANS_END);
					

				}
				else if(cmd == Protocol::COM_LIST_ART) {
					read_par_num(mh, server, conn);; // PAR_NUM
					int id = mh.readNumber(); // num_p = id av NG
					read_com_end(mh, server,conn); // COM_END

					
					NewsGroup ng = ms.getNG(id);
					mh.writeCode(Protocol::ANS_LIST_ART);
					if(ng.getID() != -1) {
						vector<pair<int, Article>> artvec = ms.get_Art(id); // moddad för diskserv compatibility
						mh.writeCode(Protocol::ANS_ACK);
						mh.writeCode(Protocol::PAR_NUM);
						mh.writeNumber(artvec.size());
						for(pair<int, Article> p: artvec) {
							mh.writeCode(Protocol::PAR_NUM);
							mh.writeNumber(p.first);
							mh.writeCode(Protocol::PAR_STRING);
							mh.writeNumber(p.second.getTitle().size());
							mh.writeString(p.second.getTitle());
						}
					}
					else {
						mh.writeCode(Protocol::ANS_NAK);
						mh.writeCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}
					mh.writeCode(Protocol::ANS_END);

				}
				else if(cmd == Protocol::COM_CREATE_ART) {
					
					read_par_num(mh, server, conn); // PAR_NUM
					int id = mh.readNumber(); // num_p = NG ID

					read_par_string(mh, server, conn); // PAR_STRING
					int sz1 = mh.readNumber(); 
					string title = mh.readString(sz1);

					read_par_string(mh, server, conn); // PAR_STRING	
					int sz2 = mh.readNumber();
					string author = mh.readString(sz2);

					read_par_string(mh, server, conn); // PAR_STRING	
					int sz3 = mh.readNumber();
					string text = mh.readString(sz3);

					read_com_end(mh, server, conn); //COM_END
					
					mh.writeCode(Protocol::ANS_CREATE_ART);
					
					if(ms.createArt(id, title, author, text)) {
						mh.writeCode(Protocol::ANS_ACK);
					}
					else {
						mh.writeCode(Protocol::ANS_NAK);
						mh.writeCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}
					mh.writeCode(Protocol::ANS_END);
					
				}
				else if(cmd == Protocol::COM_DELETE_NG) { // måste implementera list articles för att ḱunna klicka i servern
					read_par_num(mh, server, conn); // PAR_NUM;
					int id = mh.readNumber();
					read_com_end(mh, server, conn); // COM_END

					mh.writeCode(Protocol::ANS_DELETE_NG); 
					
					if(ms.removeNG(id)) {
						mh.writeCode(Protocol::ANS_ACK);
					} 
					else {
						mh.writeCode(Protocol::ANS_NAK);
						mh.writeCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}
					mh.writeCode(Protocol::ANS_END);

				}
				else if(cmd == Protocol::COM_DELETE_ART) { // kräver get article
					read_par_num(mh, server, conn); // PAR_NUM
					int id = mh.readNumber();

					read_par_num(mh, server, conn); // PAR_NUM
					int artid = mh.readNumber();

					read_com_end(mh, server, conn); // COM_END
					mh.writeCode(Protocol::ANS_DELETE_ART);
					int i = ms.delete_Art(id,artid);
					if(i == 1) {
						mh.writeCode(Protocol::ANS_ACK);
					}
					else if(i == -2) {
						mh.writeCode(Protocol::ANS_NAK);
						mh.writeCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}
					else if(i == -1) {
						mh.writeCode(Protocol::ANS_NAK);
						mh.writeCode(Protocol::ERR_ART_DOES_NOT_EXIST);
					}
					mh.writeCode(Protocol::ANS_END);
				}
				else if(cmd == Protocol::COM_GET_ART) {
					read_par_num(mh, server, conn);	// PAR_NUM
					int id = mh.readNumber(); // NG id

					read_par_num(mh, server, conn); // PAR_NUM
					int artid = mh.readNumber();  
					
					read_com_end(mh, server, conn); // com_end

					Article a = ms.get_Art(id, artid); 
					mh.writeCode(Protocol::ANS_GET_ART);
					if(a.getID() == -1) {
						mh.writeCode(Protocol::ANS_NAK);
						mh.writeCode(Protocol::ERR_ART_DOES_NOT_EXIST);
					}
					else if(a.getID() == -2) {
						mh.writeCode(Protocol::ANS_NAK);
						mh.writeCode(Protocol::ERR_NG_DOES_NOT_EXIST);
					}
					else {
						mh.writeCode(Protocol::ANS_ACK);

						mh.writeCode(Protocol::PAR_STRING);
						mh.writeNumber(a.getTitle().size()); // string size1
						mh.writeString(a.getTitle());

						mh.writeCode(Protocol::PAR_STRING);
						mh.writeNumber(a.getAuthor().size()); // string size 2
						mh.writeString(a.getAuthor());

						mh.writeCode(Protocol::PAR_STRING);
						mh.writeNumber(a.getText().size()); // string size 3
						mh.writeString(a.getText());
					}
					mh.writeCode(Protocol::ANS_END);
				}
				else {
					cout << "Server didn't recognize command" << endl;
					server.deregisterConnection(conn);
				}
			} 
			catch (ConnectionClosedException&) {
				server.deregisterConnection(conn);
				cout << "Client closed connection" << endl;
			}
		} 
		else {
			conn = make_shared<Connection>();
			server.registerConnection(conn);
			cout << "New client connects" << endl;
		}
	}
}
