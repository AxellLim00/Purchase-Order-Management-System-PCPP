#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <ctime>
#include "supplierEntry.h"
#include "ViewableData.h"
#ifndef __ITEM_ENTRY__
#define __ITEM_ENTRY__

using namespace std;

struct item {
	string code, supID;
	char name[50] = {};
	item* next = nullptr, * prev = nullptr;
};

class itemEntry : public ViewableData
{
private:
	item* headItem, * tailItem, * currentItem;
	item* getData(string code) {
		currentItem = headItem;
		while (currentItem != NULL) {
			if (currentItem->code == code) {
				return currentItem;
			}
			else {
				currentItem = currentItem->next;
			}
		}
		return nullptr;
	}
	bool save() {
		try
		{
			ofstream fileToWrtie("item.txt");
			currentItem = headItem;
			while (currentItem != NULL)
			{
				fileToWrtie << currentItem->code << "," << currentItem->name << "," << currentItem->supID << endl;
				currentItem = currentItem->next;
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
	itemEntry() {
		headItem = tailItem = currentItem = NULL;
		vector <string> record;
		ifstream fileToRead("item.txt");
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
				currentItem = headItem;

				item* newItem = new item;

				newItem->code = record[i];

				fill_n(newItem->name, sizeof(newItem->name), NULL); //a problem with the name already filled with junk
				for (int j = 0; j < record[i + 1].size(); j++) {
					newItem->name[j] = record[i + 1][j];
				}

				newItem->supID = record[i + 2];

				newItem->next = NULL;
				newItem->prev = NULL;

				if (headItem == NULL)
				{
					headItem = newItem;
					tailItem = newItem;
				}
				else
				{
					newItem->prev = tailItem;
					tailItem->next = newItem;
				}
				tailItem = newItem;
			}
			i = i + 3;
		}
	}
	bool add() {
		try
		{
			item* newItem = new item;
			cout << "Enter the following Information of the new Item: " << endl << "Name\t\t\t: ";
			cin.ignore();
			cin.getline(newItem->name, sizeof(newItem->name));
			cout << endl;
			supplierEntry se;
			se.view();
			cout << "\nSupplier code\t\t: ";
			cin >> newItem->supID;

			if (!se.search(newItem->supID)) {
				cout << "Supplier Code Does not exist, please try another one";
				return false;
			}

			time_t present = time(NULL);
			tm* local = localtime(&present);
			int _year = local->tm_year + 1900, _month = local->tm_mon + 1, _date = local->tm_mday;
			string month, date, hour, min, sec;
			if (_month < 10)
				month = "0" + to_string(_month);
			else
				month = to_string(_month);
			if (_date < 10)
				date = "0" + to_string(_date);
			else
				date = to_string(_date);
			if (local->tm_hour < 10)
				hour = "0" + to_string(local->tm_hour);
			else
				hour = to_string(local->tm_hour);
			if (local->tm_min < 10)
				min = "0" + to_string(local->tm_min);
			else
				min = to_string(local->tm_min);
			if (local->tm_sec < 10)
				sec = "0" + to_string(local->tm_sec);
			else
				sec = to_string(local->tm_sec);

			newItem->code = "IE" + to_string(_year) + month + date + hour + min + sec;
			newItem->next = headItem;
			if (headItem != NULL)
				headItem->prev = newItem;
			headItem = newItem;
			newItem->prev = NULL;
			save();
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
		
	}
	bool deletes() {
		string itemCode = "";
		item* itemDelete{};
		cout << "Enter Item Entry Code to delete: ";
		cin >> itemCode;

		if (search(itemCode)) {
			itemDelete = getData(itemCode);
			cout << "Name\t\t: " << itemDelete->name << "\nSupplier Code\t: " << itemDelete->supID << endl;
			if (contin()) {
				if (itemDelete->prev == NULL) {
					headItem = itemDelete->next;
					itemDelete->next->prev = NULL;
					delete itemDelete;
				}
				else if (itemDelete->next == NULL) {
					itemDelete->prev->next = NULL;
					delete itemDelete;
				}
				else {
					itemDelete->prev->next = itemDelete->next;
					itemDelete->next->prev = itemDelete->prev;
					delete itemDelete;
				}
				return true;
				save();
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
	bool edit() {
		string itemCode, editedVariable, pick;
		int choice;
		item* itemEdit{};
		supplierEntry se;
		cout << "Enter Item Entry Code to Edit: ";
		cin >> itemCode;

		if (search(itemCode)) {
			itemEdit = getData(itemCode);
			cout << "Name\t\t: " << itemEdit->name << "\nSupplier Code\t: " << itemEdit->supID;
			cout << "\n\nWhich info to edit?" << endl << "1. Name\n2. Supplier Code" << endl
				<< "Choose the letter : ";
			cin >> pick;
			try
			{
				choice = stoi(pick);
			}
			catch (const std::exception&)
			{
				cout << "\nPlease Input according to the instructions given\n";
				edit();
			}
			switch (choice) {
			case 1:
				cout << "\nNew Name : ";
				cin.ignore();
				getline(cin,editedVariable);

				break;
			case 2:
				cout << endl;
				se.view();
				cout << "\nNew Supplier Code : ";
				cin >> editedVariable;
				if (!se.search(editedVariable)) {
					cout << "\nSupplier Code does not exist in the database\n";
					return false;
				}
				break;
			default:
				cout << "\nPlease Input according to the instructions given\n";
				edit();
				break;
			}
			if (contin()) {
				if (choice == 1) {
					strcpy(itemEdit->name, editedVariable.c_str());
				}
				else if (choice == 2) {
					itemEdit->supID = editedVariable;
				}
				else {
					cout << "\nError: Menu in edit is not in option, but still didn't go to default case";
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
	bool view() {
		currentItem = headItem;
		int count = 1;
		while (currentItem != NULL) {
			if (count < 2) {
				cout << "No.\tCode\t\t\tName\t\t\tSupplier Code" << endl;
			}
			cout << count << "\t" << currentItem->code << "\t" << currentItem->name << "\t\t\t" << currentItem->supID << endl;
			count++;

			currentItem = currentItem->next;
		}
		system("pause");
		return true;
	}
	bool search(string code) {
		currentItem = headItem;
		while (currentItem != NULL) {
			if (currentItem->code == code) {
				return true;
			}
			else {
				currentItem = currentItem->next;
			}
		}

		return false;
	}
};
#endif