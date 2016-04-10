#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <connection.h>
#include <string>
#include <memory>
#include <iostream>

class MessageHandler {
	public:
		MessageHandler(Connection &c) : conn(c) {};
		void writeNumber(int value);
		int readNumber();
		void writeString(const std::string &s);
		std::string readString();
		std::string readString(int sz);
		void writeCode(unsigned char nbr);
		unsigned char readCode();
	
	private:
		Connection &conn;
};
#endif