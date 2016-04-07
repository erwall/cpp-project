#include "messagehandler.h"
using namespace std;
int MessageHandler::readNumber() {
	unsigned char byte1 = conn.read();	
	unsigned char byte2 = conn.read();
	unsigned char byte3 = conn.read();
	unsigned char byte4 = conn.read();
	return (byte1 << 24) | (byte2 << 16) | (byte3 << 8) | byte4;
}
void MessageHandler::writeNumber(int value) {
	conn.write((value >> 24) & 0xFF);
	conn.write((value >> 16) & 0xFF);
	conn.write((value >> 8)	 & 0xFF);
	conn.write(value & 0xFF);
}
void MessageHandler::writeString(const string& s) {
	for (char c : s) {
		conn.write(c);
	}
	conn.write('$');
}
string MessageHandler::readString(int sz) {
	string s;
	unsigned char ch;
	for(int i = 0; i != sz; ++i) {
		ch=conn.read();
		s += ch;
	}
	ch = conn.read(); // läser av dollar
	if(ch != '$') {
		cout << "String wasn't read correctly" << endl;
	}
	return s;
}
string MessageHandler::readString() {
	string s;
	char ch;
	while ((ch = conn.read()) != '$') {
		s += ch;
	}
	return s;
}