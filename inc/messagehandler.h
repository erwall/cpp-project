#ifndef MESSAGEHANDLER_H
#define MESSAGEHANDLER_H
#include <connection.h>
#include <string>
#include <memory>

class MessageHandler {
	public:
		MessageHandler(Connection &c) : conn(c) {};
		void writeNumber(int value);
		int readNumber();
		void writeString(const std::string &s);
		std::string readString();
	
	private:
		Connection &conn;
};
#endif