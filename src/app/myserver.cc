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
		
		// if(conn != nullptr) {
		// 	if(mh.readNumber() == Protocol::COM_CREATE_NG) {

		// 		cout << mh.readNumber() << endl; // = PAR_STRING

		// 		int sz = mh.readNumber();
		// 		cout << sz << endl;				// = size
				
		// 		string title = mh.readString(sz); 
		// 		cout << title << endl;

		// 		cout << mh.readNumber() << endl; // = COM_END? fuckar ur vet inte varför

		// 		mh.writeNumber(Protocol::ANS_CREATE_NG);
		// 		mh.writeNumber(Protocol::ANS_ACK);
		// 		mh.writeNumber(Protocol::ANS_END);
				
		// 	}	
		// }
		auto conn = server.waitForActivity();
		MessageHandler mh(*conn.get());
		if (conn != nullptr) {
			try {
				unsigned char cmd = mh.readNumber();
				if(cmd == Protocol::COM_LIST_NG) {
					mh.readNumber(); // com_end

					ms.list();

					mh.writeNumber(Protocol::ANS_LIST_NG);
					mh.writeNumber(Protocol::ANS_ACK);
					mh.writeNumber(Protocol::ANS_END);
				}
				else if(cmd == Protocol::COM_CREATE_NG) {  // funkar men validerar ingenting osv
					
					mh.readNumber(); // PAR_STRING
				    int sz = mh.readNumber(); //längd av sträng
					string title = mh.readString(sz);  // sträng
					mh.readNumber(); // com_end

					ms.createNG(title);  // skapar newsgroup i memserv, validerar inte atm
					
					mh.writeNumber(Protocol::ANS_CREATE_NG);
					mh.writeNumber(Protocol::ANS_ACK);
					mh.writeNumber(Protocol::ANS_END);
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
