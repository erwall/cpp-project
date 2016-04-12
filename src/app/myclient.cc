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

void protocol_error(unsigned char cmd, unsigned char ans)
{
	cout << "Protocol error. Expected " << cmd;
	cout << " but received " << ans << endl;
}

bool read_end(MessageHandler *mh)
{
	unsigned char ans;
	if ((ans = mh->readCode()) != Protocol::ANS_END) {
		protocol_error(Protocol::ANS_END, ans);
		return false;
	}
	return true;
}

int read_num(MessageHandler *mh)
{
	unsigned char ans;
	if ((ans = mh->readCode()) != Protocol::PAR_NUM) {
		protocol_error(Protocol::PAR_NUM, ans);
		return -1;
	}
	return mh->readNumber();
}

string read_string(MessageHandler *mh)
{
	unsigned char ans;
	if ((ans = mh->readCode()) != Protocol::PAR_STRING) {
		protocol_error(Protocol::PAR_STRING, ans);
		return string();
	}

	return mh->readString(mh->readNumber());
}

void write_num(MessageHandler *mh, int num)
{
	mh->writeCode(Protocol::PAR_NUM);
	mh->writeNumber(num);
}

void write_string(MessageHandler *mh, string str)
{
	mh->writeCode(Protocol::PAR_STRING);
	mh->writeNumber(str.length());
	mh->writeString(str);
}

int main(int argc, char* argv[]) {
	enum Commands {
		HELP = 0,
		LIST_NG = 10,
		CREATE_NG = 11,
		DELETE_NG = 12,
		LIST_ART = 20,
		CREATE_ART = 21,
		DELETE_ART = 22,
		GET_ART = 23
	};

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

	cout << "Client initialized on port: " << port << endl;
	cout << "Type \"help\" for list of commands" << endl;

	MessageHandler mh(conn);
	while(true) {
		string command, name, title, author, text, msg;
		int cmd, N, ng_id, art_id;
		unsigned char ans;

		/* Read command and translate to enum */
		cin >> command;
		if (command.compare("help"))
			cmd = 0;
		else if (command.compare("lsng"))
			cmd = 10;
		else if (command.compare("mkng"))
			cmd = 11;
		else if (command.compare("rmng"))
			cmd = 12;
		else if (command.compare("lsart"))
			cmd = 20;
		else if (command.compare("mkart"))
			cmd = 21;
		else if (command.compare("rmart"))
			cmd = 22;
		else if (command.compare("cat"))
			cmd = 23;

		/* Handle all commands */
		switch (cmd) {
		case Commands::LIST_NG:
			/* Write commands to server */
			mh.writeCode(Protocol::COM_LIST_NG);
			mh.writeCode(Protocol::COM_END);

			/* Read server confirmation */
			if ((ans = mh.readCode()) != Protocol::ANS_LIST_NG) {
				protocol_error(Protocol::ANS_LIST_NG, ans);
				break;
			}

			/* Read number of newsgroups */
			N = read_num(&mh);
			if (N < 0)
				break;

			cout << "Id\tName" << endl;
			/* Read all newsgroups information */
			while (N--) {
				/* Read newsgroup id */
				ng_id = read_num(&mh);
				if (ng_id < 0)
					break;

				/* Read newsgroup name */
				name = read_string(&mh);
				if (name.empty())
					break;

				/* Print newsgroup information */
				cout << ng_id << "\t " << name << endl;
			}

			/* Read server finish confirmation */
			if (!read_end(&mh))
				break;

			cout << endl;

			break;
		case Commands::CREATE_NG:
			/* Read newsgroup name from cin */
			if (!(cin >> name)) {
				cout << "Invalid newsgroup name parameter";
				cout << endl;
			}

			/* Write command to server */
			mh.writeCode(Protocol::COM_CREATE_NG);
			/* Write newsgroup name to server */
			write_string(&mh, name);
			/* Write end command to server */
			mh.writeCode(Protocol::COM_END);

			/* Read server confirmation */
			if ((ans = mh.readCode()) != Protocol::ANS_CREATE_NG) {
				protocol_error(Protocol::ANS_CREATE_NG, ans);
				break;
			}

			/* Read server response to newsgroup creation */
			ans = mh.readCode();
			switch (ans) {
			case Protocol::ANS_ACK:
				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that newsgroup was created */
				cout << "Newsgroup \"" << name;
				cout << "\" was created" << endl;

				break;
			case Protocol::ANS_NAK:
				/* Read error code */
				ans = mh.readCode();
				if (ans != Protocol::ERR_NG_ALREADY_EXISTS) {
					protocol_error(
					Protocol::ERR_NG_ALREADY_EXISTS, ans);
					break;
				}

				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that newsgroup already exists */
				cout << "Newsgroup already exists" << endl;

				break;
			default:
				protocol_error(Protocol::ANS_ACK, ans);
				break;
			}

			break;
		case Commands::DELETE_NG:
			/* Read newsgroup id from cin */
			if (!(cin >> ng_id)) {
				cout << "Invalid parameter. Must be an integer";
				cout << endl;
				break;
			}

			/* Write command to server */
			mh.writeCode(Protocol::COM_DELETE_NG);
			/* Write newsgroup id to server */
			write_num(&mh, ng_id);
			/* Write end command to server */
			mh.writeCode(Protocol::COM_END);

			/* Read server confirmation */
			if ((ans = mh.readCode()) != Protocol::ANS_DELETE_NG) {
				protocol_error(Protocol::ANS_DELETE_NG, ans);
				break;
			}

			/* Read server response to newsgroup deletion */
			ans = mh.readCode();
			switch (ans) {
			case Protocol::ANS_ACK:
				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that newsgroup was deleted */
				cout << "Newsgroup with id " << ng_id;
				cout << " was deleted" << endl;

				break;
			case Protocol::ANS_NAK:
				/* Read error code */
				ans = mh.readCode();
				if (ans != Protocol::ERR_NG_DOES_NOT_EXIST) {
					protocol_error(
					Protocol::ERR_NG_DOES_NOT_EXIST, ans);
					break;
				}

				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that newsgroup does not exists */
				cout << "Newsgroup does not exists" << endl;

				break;
			default:
				protocol_error(Protocol::ANS_ACK, ans);
				break;
			}

			break;
		case Commands::LIST_ART:
			/* Read newsgroup id from cin */
			if (!(cin >> ng_id)) {
				cout << "Invalid newsgroup id parameter";
				cout << endl;
				break;
			}

			/* Write command to server */
			mh.writeCode(Protocol::COM_LIST_ART);
			/* Write newsgroup id to server */
			write_num(&mh, ng_id);
			/* Write end command to server */
			mh.writeCode(Protocol::COM_END);

			/* Read server confirmation */
			if ((ans = mh.readCode()) != Protocol::ANS_LIST_ART) {
				protocol_error(Protocol::ANS_LIST_ART, ans);
				break;
			}

			/* Read server response to article listing */
			ans = mh.readCode();
			switch (ans) {
			case Protocol::ANS_ACK:
				/* Read number of articles */
				N = read_num(&mh);
				if (N < 0)
					break;

				cout << "Id\tTitle" << endl;
				/* Read all articles information */
				while (N--) {
					/* Read article id */
					art_id = read_num(&mh);
					if (art_id < 0)
						break;

					/* Read article name */
					title = read_string(&mh);
					if (title.empty())
						break;

					/* Print article information */
					cout << art_id << "\t " << title;
					cout << endl;
				}

				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				cout << endl;

				break;
			case Protocol::ANS_NAK:
				/* Read error code */
				ans = mh.readCode();
				if (ans != Protocol::ERR_NG_DOES_NOT_EXIST) {
					protocol_error(
					Protocol::ERR_NG_DOES_NOT_EXIST, ans);
					break;
				}

				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that newsgroup does not exists */
				cout << "Newsgroup does not exists" << endl;

				break;
			default:
				protocol_error(Protocol::ANS_ACK, ans);
				break;
			}

			break;
		case Commands::CREATE_ART:
			/* Read newsgroup id from cin */
			if (!(cin >> ng_id)) {
				cout << "Invalid newsgroup id parameter";
				cout << endl;
				break;
			}

			/* Read article title from cin */
			if (!(cin >> title)) {
				cout << "Invalid article name parameter";
				cout << endl;
			}
			/* Read article author from cin */
			if (!(cin >> author)) {
				cout << "Invalid article author name parameter";
				cout << endl;
			}

			/* Read article text from cin */
			if (getline(cin, text) < 0) {
				cout << "Invalid article text parameters";
				cout << endl;
			}

			/* Write command to server */
			mh.writeCode(Protocol::COM_CREATE_ART);
			/* Write newsgroup id to server */
			write_num(&mh, ng_id);
			/* Write article title to server */
			write_string(&mh, title);
			/* Write article author to server */
			write_string(&mh, author);
			/* Write article text to server */
			write_string(&mh, text);
			/* Write end command to server */
			mh.writeCode(Protocol::COM_END);

			/* Read server confirmation */
			if ((ans = mh.readCode()) != Protocol::ANS_CREATE_ART) {
				protocol_error(Protocol::ANS_CREATE_ART, ans);
				break;
			}

			/* Read server response to article creation */
			ans = mh.readCode();
			switch (ans) {
			case Protocol::ANS_ACK:
				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that article was created */
				cout << "Article was created" << endl;

				break;
			case Protocol::ANS_NAK:
				/* Read error code */
				ans = mh.readCode();
				if (ans != Protocol::ERR_NG_DOES_NOT_EXIST) {
					protocol_error(
					Protocol::ERR_NG_DOES_NOT_EXIST, ans);
					break;
				}

				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that newsgroup does not exists */
				cout << "Newsgroup does not exists" << endl;

				break;
			default:
				protocol_error(Protocol::ANS_ACK, ans);
				break;
			}

			break;
		case Commands::DELETE_ART:
			/* Read newsgroup id from cin */
			if (!(cin >> ng_id)) {
				cout << "Invalid newsgroup id parameter";
				cout << endl;
				break;
			}

			/* Read article id from cin */
			if (!(cin >> art_id)) {
				cout << "Invalid article id parameter";
				cout << endl;
				break;
			}

			/* Write command to server */
			mh.writeCode(Protocol::COM_DELETE_ART);
			/* Write newsgroup id to server */
			write_num(&mh, ng_id);
			/* Write article id to server */
			write_num(&mh, art_id);
			/* Write end command to server */
			mh.writeCode(Protocol::COM_END);

			/* Read server confirmation */
			if ((ans = mh.readCode()) != Protocol::ANS_DELETE_ART) {
				protocol_error(Protocol::ANS_DELETE_ART, ans);
				break;
			}

			/* Read server response to article deletion */
			ans = mh.readCode();
			switch (ans) {
			case Protocol::ANS_ACK:
				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that article was deleted */
				cout << "Article with id " << art_id;
				cout << "in newsgroup with id " << ng_id;
				cout << " was deleted" << endl;

				break;
			case Protocol::ANS_NAK:
				/* Read error code */
				ans = mh.readCode();
				switch (ans) {
				case Protocol::ERR_NG_DOES_NOT_EXIST:
					msg = "Newsgroup does not exist";
					break;
				case Protocol::ERR_ART_DOES_NOT_EXIST:
					msg = "Article does not exist";
					break;
				default:
					protocol_error(
					Protocol::ERR_NG_DOES_NOT_EXIST, ans);
					break;
				}

				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that newsgroup or article does
				 * not exists */
				cout << msg << endl;

				break;
			default:
				protocol_error(Protocol::ANS_ACK, ans);
				break;
			}

			break;
		case Commands::GET_ART:
			/* Read newsgroup id from cin */
			if (!(cin >> ng_id)) {
				cout << "Invalid newsgroup id parameter";
				cout << endl;
				break;
			}

			/* Read article id from cin */
			if (!(cin >> art_id)) {
				cout << "Invalid article id parameter";
				cout << endl;
				break;
			}

			/* Write command to server */
			mh.writeCode(Protocol::COM_GET_ART);
			/* Write newsgroup id to server */
			write_num(&mh, ng_id);
			/* Write article id to server */
			write_num(&mh, art_id);
			/* Write end command to server */
			mh.writeCode(Protocol::COM_END);

			/* Read server confirmation */
			if ((ans = mh.readCode()) != Protocol::ANS_GET_ART) {
				protocol_error(Protocol::ANS_GET_ART, ans);
				break;
			}

			/* Read server response to article read */
			ans = mh.readCode();
			switch (ans) {
			case Protocol::ANS_ACK:
				title = read_string(&mh);
				if (title.empty())
					break;
				author = read_string(&mh);
				if (author.empty())
					break;
				text = read_string(&mh);
				if (text.empty())
					break;

				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that article was deleted */
				cout << "Article with id " << art_id;
				cout << "in newsgroup with id " << ng_id;
				cout << " was deleted" << endl;

				break;
			case Protocol::ANS_NAK:
				/* Read error code */
				ans = mh.readCode();
				switch (ans) {
				case Protocol::ERR_NG_DOES_NOT_EXIST:
					msg = "Newsgroup does not exist";
					break;
				case Protocol::ERR_ART_DOES_NOT_EXIST:
					msg = "Article does not exist";
					break;
				default:
					protocol_error(
					Protocol::ERR_NG_DOES_NOT_EXIST, ans);
					break;
				}

				/* Read server finish confirmation */
				if (!read_end(&mh))
					break;

				/* Inform user that newsgroup or article does
				 * not exists */
				cout << msg << endl;

				break;
			default:
				protocol_error(Protocol::ANS_ACK, ans);
				break;
			}

			break;
		case Commands::HELP:
			break;
		default:
			cout << "Invalid command: " << command << endl;
			break;
		}
	}
}
