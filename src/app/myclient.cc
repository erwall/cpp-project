/* myclient.cc: sample client program */
#include "connection.h"
#include "connectionclosedexception.h"
#include "protocol.h"
#include "messagehandler.h"
#include <iostream>
#include <string>
#include <stdexcept>
#include <cstdlib>

using namespace std;

/*
 * Send an integer to the server as four bytes.
 */
void writeNumber(const Connection& conn, int value) {
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}

/*
 * Read a string from the server.
 */
string readString(const Connection& conn) {
	string s;
	char ch;
	while ((ch = conn.read()) != '$') {
		s += ch;
	}
	return s;
}
bool validate(string command, string parameters) {
	return true; // finish
}

int main(int argc, char* argv[]) {
	if (argc != 3) {
		cerr << "Usage: myclient host-name port-number" << endl;
		exit(1);
	}
	
	int port = -1;
	try {
		port = stoi(argv[2]);
	} catch (exception& e) {
		cerr << "Wrong port number. " << e.what() << endl;
		exit(1);
	}
	
	Connection conn(argv[1], port);
	if (!conn.isConnected()) {
		cerr << "Connection attempt failed" << endl;
		exit(1);
	}

	/*
		Skickar title till myserver som sedan skriver ut dem. Varje char läses med
		conn.read() i myserver, behöver skicka med längden på strängen så att den vet
		hur mycket den ska läsa.

	*/
	MessageHandler mh(conn);
	string title = "yolosag";
	while(true) {
		string command;
		string parameters;
		
		getline(cin, command);
		getline(cin, parameters);
		
		if(!validate(command, parameters)) {
			// error
		}

		if(command == "list newsgroups") {
			mh.writeNumber(Protocol::COM_LIST_NG);
			mh.writeNumber(Protocol::COM_END);
			mh.readNumber(); // ANS_LIST_NG
			mh.readNumber(); // ANS_ACK
			mh.readNumber(); // ANS_END
		}

		else if(command == "create newsgroup") {

			mh.writeNumber(Protocol::COM_CREATE_NG);
			mh.writeNumber(Protocol::PAR_STRING); 
			mh.writeNumber(parameters.size());
			mh.writeString(parameters);
			mh.writeNumber(Protocol::COM_END);

			mh.readNumber(); // ANS_CREATE_NG
			mh.readNumber(); // ANS_ACK
			mh.readNumber(); // ANS_END
		}
		else if(command == "Delete newsgroup") {

		}
		// if(line == "GO") {
		// 	
			
		// }	
	
		
	}
	
	// cout << "Type a number: ";
	// int nbr;
	// while (cin >> nbr) {
	// 	if(nbr == 1) { // create newsgroup test
	// 		conn.write(Protocol::COM_CREATE_NG);
	// 		// conn.write(Protocol::PAR_STRING);
	// 		// string a = "a";
	// 		// const string &s = a;
	// 		// int nbr2 = s.size();
	// 		// conn.write((nbr2 >> 24) & 0xFF);
	// 		// conn.write((nbr2 >> 16) & 0xFF);
	// 		// conn.write((nbr2 >> 8)	 & 0xFF);
	// 		// conn.write(nbr2 & 0xFF);
	// 		// for(unsigned char c: s) {
	// 		// 	conn.write(c);
	// 		// }
	// 		conn.write(Protocol::COM_END);
	// 		auto start = conn.read();
	// 		auto ack = conn.read();
	// 		auto end = conn.read();
	// 	}
	// 	try {
	// 		cout << nbr << " is ...";
	// 		writeNumber(conn, nbr);
	// 		string reply = readString(conn);
	// 		cout << " " << reply << endl;
	// 		cout << "Type another number: ";
	// 	} catch (ConnectionClosedException&) {
	// 		cout << " no reply from server. Exiting." << endl;
	// 		exit(1);
	// 	}
	// }
}

