#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <ctime>
#include "ViewableData.h"
#ifndef __USER__
#define __USER__

using namespace std;

struct logins {
	string username, password, type;
	logins* next = nullptr, * prev = nullptr;
};

class User : public ViewableData
{
private:
	logins* headUser, * tailUser, * currentUser;
	logins* getData(string user) {
		currentUser = headUser;
		while (currentUser != NULL) {
			if (currentUser->username == user) {
				return currentUser;
			}
			else {
				currentUser = currentUser->next;
			}
		}
		return nullptr;
	}
	bool save() {
		try
		{
			ofstream fileToWrtie("users.txt");
			currentUser = headUser;
			while (currentUser != NULL)
			{
				fileToWrtie << currentUser->username << "," << currentUser->password << "," << currentUser->type << endl;
				currentUser = currentUser->next;
			}

			fileToWrtie.close();
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}
public:
	User() {
		headUser = tailUser = currentUser = nullptr;
		vector <string> record;
		ifstream fileToRead("users.txt");
		while (fileToRead)
		{
			string rawdata;
			if (!getline(fileToRead, rawdata)) break;

			istringstream ss(rawdata);

			while (ss)
			{
				string s;
				if (!getline(ss, rawdata, ',')) break;
				record.push_back(rawdata);
			}
		}
		fileToRead.close();
		int i = 0, size;
		size = record.size();
		while (i < size) {
			if (i % 3 == 0) {
				currentUser = headUser;
				logins* newUser = new logins;

				newUser->username = record[i];
				newUser->password = record[i + 1];
				newUser->type = record[i + 2];
				newUser->next = NULL;
				newUser->prev = NULL;

				if (headUser == NULL)
				{
					headUser = newUser;
					tailUser = newUser;
				}
				else
				{
					newUser->prev = tailUser;
					tailUser->next = newUser;
				}
				tailUser = newUser;
			}
			i = i + 3;
		}
	}
	bool login(string user, string pass) {
		if (search(user)) {
			if (pass == getData(user)->password)
				return true;
			else
				return false;
		}
		else
			return false;
	}
	string getType(string user) {
		return getData(user)->type;
	}
	bool add() {
		try
		{
			logins* newUser = new logins;
			cout << "Enter the following Information of the new User: " << endl << "Username\t\t\t\t\t\t: ";
			cin >> newUser->username;
			if (search(newUser->username)) {
				cout << "\nUsername is already taken, please another one.";
				system("PAUSE");
				system("CLS");
				add();
			}
			cout << "Password\t\t\t\t\t\t: ";
			cin >> newUser->password;
			cout << "Usertype (AD[admin]/PM[Purchase Manager]/SM[Sales Manager])\t: ";
			cin >> newUser->type;
			if (newUser->type != "AD" && newUser->type != "PM" && newUser->type != "SM") {
				cout << "Input the type According to Instructions.";
				system("PAUSE");
				system("CLS");
				add();
			}

			newUser->next = headUser;
			if (headUser != NULL)
				headUser->prev = newUser;
			headUser = newUser;
			newUser->prev = NULL;
			save();
			cout << endl;
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}
	bool deletes() {
		string user;
		logins* userDelete{};
		cout << "Enter Username to delete: ";
		cin >> user;

		if (search(user)) {
			userDelete = getData(user);
			cout << "Password\t: ********\nUsertype\t: " << userDelete->type << endl;
			if (contin()) {
				if (userDelete->prev == NULL) {
					headUser = userDelete->next;
					userDelete->next->prev = NULL;
					delete userDelete;
				}
				else if (userDelete->next == NULL) {
					userDelete->prev->next = NULL;
					delete userDelete;
				}
				else {
					userDelete->prev->next = userDelete->next;
					userDelete->next->prev = userDelete->prev;
					delete userDelete;
				}
				save();
				return true;
			}
			else {
				return false;
			}
		}
		else {
			cout << "\nUser not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool edit() {
		string user, editedVariable, pick;
		int choice;
		logins* userEdit{};
		cout << "Enter Username to Edit: ";
		cin >> user;

		if (search(user)) {
			userEdit = getData(user);
			cout << "Password\t: ********\nUsertype\t: " << userEdit->type;
			cout << "\nWhich info to edit?" << endl << "1. Password\n2. Usertype " << endl
				<< "Choose the number : ";
			cin >> pick;
			try
			{
				choice = stoi(pick);
			}
			catch (const std::exception&)
			{
				cout << "Please Input according to the instructions given\n";
				edit();
			}

			switch (choice) {
			case 1:
				cout << "\nNew password : ";
				cin >> editedVariable;
				break;
			case 2:
				cout << "\nNew Usertype (AD[admin]/PM[Purchase Manager]/SM[Sales Manager]) : ";
				cin >> editedVariable;
				if (editedVariable != "AD" && editedVariable != "PM" && editedVariable != "SM") {
					cout << "Input the type According to Instructions.";
					system("PAUSE");
					system("CLS");
					edit();
				}
				break;
			default:
				cout << "\nPlease Input according to the instructions given\n";
				return false;
				break;
			}
			if (contin()) {
				if (choice == 1) {
					userEdit->password = editedVariable;
				}
				else if (choice == 2) {
					userEdit->type = editedVariable;
				}
				else {
					cout << "\nError: Menu in edit is not in option, but still didn't go to default case\n";
					return false;
				}
				save();
				return true;
			}
			else {
				return false;
			}
		}
		else {
			cout << "\nItem not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool search(string user) {
		currentUser = headUser;
		while (currentUser != NULL) {
			if (currentUser->username == user) {
				return true;
			}
			else {
				currentUser = currentUser->next;
			}
		}
		return false;
	}
};

#endif