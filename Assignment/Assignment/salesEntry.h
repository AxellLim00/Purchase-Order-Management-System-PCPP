#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <ctime>
#include "Date.h"
#include "itemEntry.h"
#include "Data.h"
#ifndef __SALES_ENTRY__
#define __SALES_ENTRY__

using namespace std;

struct itemSold {
	string itemid;
	int quantity = 0;
	itemSold* next = nullptr, * prev = nullptr;
};

struct sale {
	string code;
	Date date;
	itemSold* headIS = nullptr, * tailIS = nullptr;
	sale* next = nullptr, * prev = nullptr;
};

class salesEntry : public Data
{
private:
	itemSold* currentIS = nullptr;
	sale* headSale, * tailSale, * currentSale;
	sale* getData(string code) {
		currentSale = headSale;
		while (currentSale != NULL) {
			if (currentSale->code == code) {
				return currentSale;
			}
			else {
				currentSale = currentSale->next;
			}
		}
		return nullptr;
	}
	itemSold* getItemData(sale* sales, string itemCode) {
		currentIS = sales->headIS;
		while (currentIS != NULL) {
			if (currentIS->itemid == itemCode) {
				return currentIS;
			}
			else {
				currentIS = currentIS->next;
			}
		}
		return nullptr;
	}
	bool searchItem(sale* sales, string itemCode) {
		currentIS = sales->headIS;
		while (currentIS != NULL) {
			if (currentIS->itemid == itemCode) {
				return true;
			}
			else {
				currentIS = currentIS->next;
			}
		}
		return false;
	}
	bool isDateSame(string the_date) {
		currentSale = headSale;
		while (currentSale != NULL) {
			if (currentSale->date.print() == the_date) {
				return true;
			}
			else {
				currentSale = currentSale->next;
			}
		}
		return false;
	}
	bool save() {
		try
		{
			ofstream fileToWrtie("sales.txt");
			currentSale = headSale;
			while (currentSale != NULL)
			{
				fileToWrtie << currentSale->code << "," << currentSale->date.print() << ",";
				currentIS = currentSale->headIS;
				while (currentIS != NULL)
				{
					if (currentIS->next == nullptr) {
						fileToWrtie << currentIS->itemid << "|" << currentIS->quantity << endl;
					}
					else {
						fileToWrtie << currentIS->itemid << "|" << currentIS->quantity << "|";
					}
					currentIS = currentIS->next;
				}
				currentSale = currentSale->next;
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
	salesEntry() {
		headSale = tailSale = currentSale = nullptr;
		currentIS = nullptr;
		vector <string> record;
		vector <string> recordItem;
		ifstream fileToRead("sales.txt");
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
		int i = 0, j = 0, size;
		size = record.size();
		while (i < size) {
			if (i % 3 == 0) {
				currentSale = headSale;
				sale* newSales = new sale;

				newSales->code = record[i];
				newSales->date.setDate(record[i + 1]);

				string rawdataItem = record[i + 2];

				istringstream ss2(rawdataItem);

				while (ss2)
				{
					string s2;
					if (!getline(ss2, rawdataItem, '|')) break;
					recordItem.push_back(rawdataItem);
				}
				
				int sizeItem;
				sizeItem = recordItem.size();
				while (j < sizeItem) {
					if (j % 2 == 0) {
						currentIS = newSales->headIS;

						itemSold* newItemSold = new itemSold;

						newItemSold->itemid = recordItem[j];
						newItemSold->quantity = stoi(recordItem[j + 1]);

						newItemSold->next = NULL;
						newItemSold->prev = NULL;

						if (newSales->headIS == NULL)
						{
							newSales->headIS = newItemSold;
							newSales->tailIS = newItemSold;
						}
						else
						{
							newItemSold->prev = newSales->tailIS;
							newSales->tailIS->next = newItemSold;
						}
						newSales->tailIS = newItemSold;
						j = j + 2;
					}
				}

				newSales->next = NULL;
				newSales->prev = NULL;

				if (headSale == NULL)
				{
					headSale = newSales;
					tailSale = newSales;
				}
				else
				{
					newSales->prev = tailSale;
					tailSale->next = newSales;
				}
				tailSale = newSales;
			}
			i = i + 3;
		}
	}
	bool add() {
		try
		{
			sale* newSales = new sale;
			if (isDateSame(newSales->date.print())) {
				cout << "Today's Daily Sales Entry has already Been Created,\nPlease Edit or wait for tomorrow\n";
				return false;
			}
			newSales->headIS = newSales->tailIS = nullptr;
			cout << "Enter the following Information of the new Sales: " << endl << endl;
			Date* thedate = new Date;
			newSales->date = *thedate;

			itemEntry items;
			items.view();
			cout << endl;

			string id = "";
			int quant = 0, i = 1;
			while (id != "EXIT") {
				itemSold* newItem = new itemSold;
				cout << "Item Number " << i++ << " :\n"
					<< "Item Code (type 'EXIT' to exit with caps): ";
				cin >> id;
				if (id == "EXIT") {
					continue;
				}
				else if (!items.search(id)) {
					cout << "Item Code Does not Exist, Please see the list above\n\n";
					delete newItem;
					i--;
					continue;
				}
				else if (i != 1 && searchItem(newSales, id)) {
					cout << "Item Has already been entered by you, Please enter another Item\n\n";
					delete newItem;
					i--;
					continue;
				}
				newItem->itemid = id;
				cout << "Quantity\t\t\t: ";
				cin >> newItem->quantity;

				newItem->next = newSales->headIS;
				if (newSales->headIS != NULL)
					newSales->headIS->prev = newItem;
				newSales->headIS = newItem;
				newItem->prev = NULL;
				cout << endl;
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

			newSales->code = "SaE" + to_string(_year) + month + date + hour + min + sec;
			newSales->next = headSale;
			if (headSale != NULL)
				headSale->prev = newSales;
			headSale = newSales;
			newSales->prev = NULL;
			save();
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}
	bool deletes() {
		string salesCode;
		sale* salesDelete{};
		cout << "Enter Sales Code to delete: ";
		cin >> salesCode;

		if (search(salesCode)) {
			salesDelete = getData(salesCode);
			cout << "Date\t: " << salesDelete->date.print();
			currentIS = salesDelete->headIS;
			int count = 1;
			while (currentIS != NULL) {
				if (count < 2) {
					cout << "\nNo.\tItem Code\t\tQuantity" << endl;
				}
				cout << count << "\t" << currentIS->itemid << "\t" << currentIS->quantity << endl;
				count++;
				currentIS = currentIS->next;
			}
			cout << endl;
			if (contin()) {
				if (salesDelete->prev == NULL) {
					headSale = salesDelete->next;
					salesDelete->next->prev = NULL;
					delete salesDelete;
				}
				else if (salesDelete->next == NULL) {
					salesDelete->prev->next = NULL;
					delete salesDelete;
				}
				else {
					salesDelete->prev->next = salesDelete->next;
					salesDelete->next->prev = salesDelete->prev;
					delete salesDelete;
				}
				save();
				return true;
			}
			else {
				return false;
			}
		}
		else {
			cout << "\nSales Entry not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool edit() {
		string salesCode, editedVariable, itemCode, pick1, pick2, id, quant;
		sale* salesEdit{};
		itemEntry itemtest;
		int choice1 = 0, choice2;
		bool flag = true;
		cout << "Enter Sales Entry Code to Edit: ";
		cin >> salesCode;

		if (search(salesCode)) {
			salesEdit = getData(salesCode);
			cout << "Date\t: " << salesEdit->date.print();
			currentIS = salesEdit->headIS;
			int count = 1;
			while (currentIS != NULL) {
				if (count < 2) {
					cout << "\n\nNo.\tItem Code\t\tQuantity" << endl;
				}
				cout << count << "\t" << currentIS->itemid << "\t" << currentIS->quantity << endl;
				count++;
				currentIS = currentIS->next;
			}

			while (choice1 != 3) {
				cout << endl << "1. Edit/Delete Existing Item on list\n2. Add new Item to list\n3. Exit\nChoose the Number : ";
				cin >> pick1;
				try
				{
					choice1 = stoi(pick1);
				}
				catch (const std::exception&)
				{
					cout << "\nPlease Input according to the instructions given\n\n";
					continue;
				}
				switch (choice1)
				{
				case 1:
					while (flag) {
						cout << endl << "You can't edit the date\n Enter Item Code to edit/delete in the list : ";
						cin >> itemCode;
						flag = !searchItem(salesEdit, itemCode);
						if (flag) {
							cout << "\nItem Code is not in the list\n";
							return false;
						}
					}
					flag = true;
					while (flag) {
						cout << "\nWhich would you like to change :\n1. Item Code\n2. Quantity\nChoose the Number : ";
						cin >> pick2;
						try
						{
							choice2 = stoi(pick2);
						}
						catch (const std::exception&)
						{
							cout << "\nPlease Input according to the instructions given\n\n";
							continue;
						}
						switch (choice2) {
						case 1:
							cout << endl;
							itemtest.view();
							cout << "New item Code : ";
							cin >> editedVariable;
							if (!itemtest.search(editedVariable)) {
								cout << "Item Code Doesn not Exist\n\n";
								continue;
							}
							flag = false;
							break;
						case 2:
							cout << "New Quantity : ";
							cin >> editedVariable;
							try
							{
								stoi(editedVariable);
							}
							catch (const std::exception&)
							{
								cout << "Please Enter a number\n\n";
								continue;
							}
							flag = false;
							break;
						default:
							cout << "\nPlease Input according to the instructions given\n\n";
							continue;
							break;
						}
					}
					break;
				case 2:
					flag = true;
					cout << endl;
					itemtest.view();
					while (flag) {
						cout << "\nNew Item :\n"
							<< "Item Code: ";
						cin >> id;
						if (!itemtest.search(id)) {
							cout << "Item Code Does not Exist, Please see the list above\n\n";
							continue;
						}
						else if (searchItem(salesEdit, id)) {
							cout << "Item Has already been entered by you, Please enter another Item\n\n";
							continue;
						}
						cout << "Quantity\t\t\t: ";
						cin >> quant;
						try
						{
							stoi(quant);
						}
						catch (const std::exception&)
						{
							cout << "ERROR: Please Enter a number\n\n";
							continue;
						}
						flag = false;
					}
					
					break;
				case 3:
					return false;
					break;
				default:
					cout << "\nPlease Input according to the instructions given\n\n";
					continue;
					break;
				}
				break;
			}
			cout << endl;
			if (contin()) {
				itemSold* itemEdit{};
				itemEdit = getItemData(salesEdit, itemCode);
				if (choice1 == 1 && choice2 == 1) {
					itemEdit->itemid = editedVariable;
				}
				else if (choice1 == 1 && choice2 == 2) {
					itemEdit->quantity = stoi(editedVariable);
				}
				else if (choice1 == 2) {
					itemSold* newItem = new itemSold;
					newItem->itemid = id;
					newItem->quantity = stoi(quant);
					newItem->next = salesEdit->headIS;
					if (salesEdit->headIS != NULL)
						salesEdit->headIS->prev = newItem;
					salesEdit->headIS = newItem;
					newItem->prev = NULL;
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
			cout << "\nSales Entry not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool search(string code) {
		currentSale = headSale;
		while (currentSale != NULL) {
			if (currentSale->code == code) {
				return true;
			}
			else {
				currentSale = currentSale->next;
			}
		}
		return false;
	}
};
#endif