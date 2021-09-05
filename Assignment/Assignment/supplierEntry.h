#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <ctime>
#include "ViewableData.h"
#pragma warning(disable : 4996)
#ifndef __SUPPLIER_ENTRY__
#define __SUPPLIER_ENTRY__

using namespace std;

struct supplier {
	string code;
	char name[50] = {};
	supplier* next = nullptr, * prev = nullptr;
};

class supplierEntry : public ViewableData
{
private:
	supplier* headSup, * tailSup, * currentSup;
	supplier* getData(string code) {
		currentSup = headSup;
		while (currentSup != NULL) {
			if (currentSup->code == code) {
				return currentSup;
			}
			else {
				currentSup = currentSup->next;
			}
		}
		return nullptr;
	}
	bool save() {
		try
		{
			ofstream fileToWrtie("supplier.txt");
			currentSup = headSup;
			while (currentSup != NULL)
			{
				fileToWrtie << currentSup->code << "," << currentSup->name << endl;
				currentSup = currentSup->next;
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
	supplierEntry() {
		headSup = tailSup = currentSup = nullptr;
		vector <string> record;
		ifstream fileToRead("supplier.txt");
		while (fileToRead)
		{
			string rawdata;
			if (!getline(fileToRead, rawdata)) break;

			istringstream ss(rawdata);

			while (ss)
			{
				string 
					s;
				if (!getline(ss, rawdata, ',')) break;
				record.push_back(rawdata);
			}
		}
		fileToRead.close();
		int i = 0, size;
		size = record.size();
		while (i < size) {
			if (i % 2 == 0) {
				currentSup = headSup;
				supplier* newSupplier = new supplier;

				newSupplier->code = record[i];
				fill_n(newSupplier->name, sizeof(newSupplier->name), NULL); //a problem with the name already filled with junk
				for (int j = 0; j < record[i + 1].size(); j++) {
					newSupplier->name[j] = record[i + 1][j];
				}
				newSupplier->next = NULL;
				newSupplier->prev = NULL;

				if (headSup == NULL)
				{
					headSup = newSupplier;
					tailSup = newSupplier;
				}
				else
				{
					newSupplier->prev = tailSup;
					tailSup->next = newSupplier;
				}
				tailSup = newSupplier;
			}
			i = i + 2;
		}
	}
	bool add() {
		try
		{
			supplier* newSupplier = new supplier;
			cout << "Enter the following Information of the new Supplier: " << endl << "Name\t\t\t: ";
			cin.ignore();
			cin.getline(newSupplier->name, sizeof(newSupplier->name));

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

			newSupplier->code = "SuE" + to_string(_year) + month + date + hour + min + sec;
			newSupplier->next = headSup;
			if (headSup != NULL)
				headSup->prev = newSupplier;
			headSup = newSupplier;
			newSupplier->prev = NULL;
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
		string supplierCode;
		supplier* supplierDelete{};
		cout << "Enter Supplier Entry Code to delete: ";
		cin >> supplierCode;

		if (search(supplierCode)) {
			supplierDelete = getData(supplierCode);
			cout << "Name\t: " << supplierDelete->name << endl;
			if (contin()) {
				if (supplierDelete->prev == NULL) {
					headSup = supplierDelete->next;
					supplierDelete->next->prev = NULL;
					delete supplierDelete;
				}
				else if (supplierDelete->next == NULL) {
					supplierDelete->prev->next = NULL;
					delete supplierDelete;
				}
				else {
					supplierDelete->prev->next = supplierDelete->next;
					supplierDelete->next->prev = supplierDelete->prev;
					delete supplierDelete;
				}
				save();
				return true;
			}
			else {
				return false;
			}
		}
		else {
			cout << "\nSupplier not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool edit() {
		string supplierCode, editedVariable;
		supplier* supplierEdit{};
		cout << "Enter Supplier Entry Code to Edit: ";
		cin >> supplierCode;

		if (search(supplierCode)) {
			supplierEdit = getData(supplierCode);
			cout << "Name\t: " << supplierEdit->name << endl;
			cout << "\nNew Name : ";
			cin.ignore();
			getline(cin, editedVariable);
			if (contin()) {
				strcpy(supplierEdit->name, editedVariable.c_str());
				save();
				return true;
			}
			else {
				return false;
			}
		}
		else {
			cout << "\nSupplier not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool view() {
		currentSup = headSup;
		int count = 1;
		while (currentSup != NULL) {
			if (count < 2) {
				cout << "No.\tCode\t\t\tName" << endl;
			}
			cout << count << "\t" << currentSup->code << "\t" << currentSup->name << endl;
			count++;

			currentSup = currentSup->next;
		}
		system("pause");
		return true;;
	}
	bool search(string code) {
		currentSup = headSup;
		while (currentSup != NULL) {
			if (currentSup->code == code) {
				return true;
			}
			else {
				currentSup = currentSup->next;
			}
		}
		return false;
	}
};
#endif