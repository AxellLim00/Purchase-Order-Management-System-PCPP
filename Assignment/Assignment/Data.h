#include <iostream>
#pragma once

using namespace std;

class Data
{
protected:
	bool contin() {
		char cont;
		cout << "\nDo you wish to go further?(y/n) : ";
		cin >> cont;
		cont = tolower(cont);

		switch (cont)
		{
		case 'y':
			return true;
			break;
		case 'n':
			return false;
			break;
		}
		return false;
	}
	virtual bool save() {
		return true;
	}
public:
	virtual bool add() {
		return true;
	}
	virtual bool deletes() {
		return true;
	}
	virtual bool edit() {
		return true;
	}
	virtual bool search(string code) {
		return true;
	}
};

