#pragma once
#include<string>
#include<iostream>
using namespace std;

class Logger {
	int type;
	static const int DEBUG = 0;
	static const int WARNNING = 1;
	static const int INFO = 2;
	static const int ERROR1 = 3;
	static const int SEVERE = 4;

public:
	Logger() {
		Logger(WARNNING);
	}

	Logger(int type) {
		this->type = type;
	}

	void log(int type, string text) {
		string output;

		switch (type)
		{
		case DEBUG:
			output = "[DEBUG] " + text;
			break;
		case INFO:
			output = "[INFO] " + text;
			break;
		case WARNNING:
			output = "[WARNNING] " + text;
			break;
		case ERROR1:
			output = "[ERROR] " + text;
			break;
		case SEVERE:
			output = "[SEVERE] " + text;
			break;
		default:
			break;
		}
		cout << output << endl;
	}

	void debug(string text) {
		log(DEBUG, text);
	}

	void info(string text) {
		log(INFO, text);
	}

	void warn(string text) {
		log(WARNNING, text);
	}

	void error(string text) {
		log(ERROR, text);
	}

	void severe(string text) {
		log(SEVERE, text);
	}
};