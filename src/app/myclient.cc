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

	MessageHandler mh(conn);
	while(true) {
		string command;
		string parameters;
		
		getline(cin, command);
		if(command != "list newsgroups") {
			getline(cin, parameters);
		}
		
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
		
	}
}

