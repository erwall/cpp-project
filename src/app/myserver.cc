/* myserver.cc: sample server program */
#include "memserv.h"
#include "protocol.h"
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

/*
 * Read an integer from a client.
 */
int readNumber(const shared_ptr<Connection>& conn) {
	unsigned char byte1 = conn->read();
	unsigned char byte2 = conn->read();
	unsigned char byte3 = conn->read();
	unsigned char byte4 = conn->read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}

/*
 * Send a string to a client.
 */
void writeString(const shared_ptr<Connection>& conn, const string& s) {
	for (char c : s) {
		conn->write(c);
	}
	conn->write('$');
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
	MemServ ms;
	cout << "created" << endl;
	
	while (true) {
		auto conn = server.waitForActivity();
		if (conn != nullptr) {
			MessageHandler mh(*conn.get());
			try {
				unsigned char cmd = mh.readCode(); // mod
				cout << "cmd server: " << (int) cmd << endl;
				
				if(cmd == Protocol::COM_LIST_NG) { // temporary för test purposes, funkar för testserver atm
					vector<pair<int, NewsGroup>> ngvec = ms.getNG();
					unsigned char c = mh.readCode(); // com_end	
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
					
					unsigned char c = mh.readCode(); // PAR_STRING
				    int sz = mh.readNumber(); //längd av sträng
					string title = mh.readString(sz);  // sträng
					unsigned char d = mh.readCode(); // com_end

					mh.writeCode(Protocol::ANS_CREATE_NG);
					if(ms.createNG(title)) {
						mh.writeCode(Protocol::ANS_ACK);
						mh.writeCode(Protocol::ANS_END);
					}
					else {
						cout << "here" << endl;
						mh.writeCode(Protocol::ERR_NG_ALREADY_EXISTS);
						mh.writeCode(Protocol::ANS_END);
					}
					

				}
				else if(cmd == Protocol::COM_LIST_ART) {
					mh.readCode(); // PAR_NUM
					int id = mh.readNumber(); // num_p = id av NG
					mh.readCode(); // COM_END

					
					NewsGroup ng = ms.getNG(id);
					vector<pair<int, Article>> artvec = ng.get_Art();

					mh.writeCode(Protocol::ANS_LIST_ART);
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
					mh.writeCode(Protocol::ANS_END);

				}
				else if(cmd == Protocol::COM_CREATE_ART) {
					
					mh.readCode(); // PAR_NUM
					int id = mh.readNumber(); // num_p = NG ID

					mh.readCode(); // PAR_STRING
					int sz1 = mh.readNumber(); 
					string title = mh.readString(sz1);

					mh.readCode(); // PAR_STRING	
					int sz2 = mh.readNumber();
					string author = mh.readString(sz2);

					mh.readCode(); // PAR_STRING	
					int sz3 = mh.readNumber();
					string text = mh.readString(sz3);

					mh.readCode(); //COM_END
					
					mh.writeCode(Protocol::ANS_CREATE_ART);
					
					ms.createArt(id, title, author, text);
					
					mh.writeCode(Protocol::ANS_ACK);
					mh.writeCode(Protocol::ANS_END);
					//ms.listArt(); // COM_CREATE_ART skapar ordentligt
				}
				else if(cmd == Protocol::COM_DELETE_NG) { // måste implementera list articles för att ḱunna klicka i servern
					mh.readCode(); // PAR_NUM;
					int id = mh.readNumber();
					mh.readCode(); // COM_END

					mh.writeCode(Protocol::ANS_DELETE_NG); 
					
					ms.removeNG(id); // inga kontroller

					mh.writeCode(Protocol::ANS_ACK);
					mh.writeCode(Protocol::ANS_END);
				}
				else if(cmd == Protocol::COM_DELETE_ART) { // kräver get article
					mh.readCode(); // PAR_NUM
					int id = mh.readNumber();

					mh.readCode(); // PAR_NUM
					int artid = mh.readNumber();

					mh.readCode(); // COM_END
					ms.delete_Art(id,artid);

					mh.writeCode(Protocol::ANS_DELETE_ART);

					mh.writeCode(Protocol::ANS_ACK);
					mh.writeCode(Protocol::ANS_END);
				}
				else if(cmd == Protocol::COM_GET_ART) { 
					mh.readCode();	// PAR_NUM
					int id = mh.readNumber(); // NG id
					cout << "NGID COM_GET_ART: " << id << endl; // fel? wtf

					mh.readCode(); // PAR_NUM
					int artid = mh.readNumber(); // art id
					cout << "ARTID COM_GET_ART: " << artid << endl;  // fel? wtf
					mh.readCode(); // COM_END
					
					Article a = ms.get_Art(id, artid); // returnerar fel?
					ms.listArt(id);
					
					cout << "returned author to COM_GET_ART: " << a.getAuthor() << endl;
					mh.writeCode(Protocol::ANS_GET_ART);
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
					
					mh.writeCode(Protocol::ANS_END);
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
