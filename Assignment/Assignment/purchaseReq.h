#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <ctime>
#include "Date.h"
#include "itemEntry.h"
#include "ViewableData.h"
#ifndef __PURCHASE_REQUISITION__
#define __PURCHASE_REQUISITION__

using namespace std;

struct itemRequest {
	string itemid;
	int quantity = 0;
	itemRequest* next = nullptr, * prev = nullptr;
};

struct purchaseRequest {
	string code;
	Date dateToOrder;
	bool approved = false;
	itemRequest* headIR = nullptr, * tailIR = nullptr;
	purchaseRequest* next = nullptr, * prev = nullptr;
};

class purchaseReq : public ViewableData
{
private:
	itemRequest* currentIR;
	purchaseRequest* headPR, * tailPR, * currentPR;
	itemRequest* getItemData(purchaseRequest* purReq, string itemCode) {
		currentIR = purReq->headIR;
		while (currentIR != NULL) {
			if (currentIR->itemid == itemCode) {
				return currentIR;
			}
			else {
				currentIR = currentIR->next;
			}
		}
		return nullptr;
	}
	bool searchItem(purchaseRequest* purReq, string itemCode) {
		currentIR = purReq->headIR;
		while (currentIR != NULL) {
			if (currentIR->itemid == itemCode) {
				return true;
			}
			else {
				currentIR = currentIR->next;
			}
		}
		return false;
	}
	bool save() {
		try
		{
			ofstream fileToWrtie("purchase requisite.txt");
			currentPR = headPR;
			while (currentPR != NULL)
			{
				fileToWrtie << currentPR->code << "," << currentPR->dateToOrder.print() << "," << boolalpha << currentPR->approved << ",";
				currentIR = currentPR->headIR;
				while (currentIR != NULL)
				{
					if (currentIR->next == nullptr) {
						fileToWrtie << currentIR->itemid << "|" << currentIR->quantity << endl;
					}
					else {
						fileToWrtie << currentIR->itemid << "|" << currentIR->quantity << "|";
					}
					currentIR = currentIR->next;
				}
				currentPR = currentPR->next;
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
	purchaseReq() {
		headPR = tailPR = currentPR = nullptr;
		currentIR = nullptr;
		vector <string> record;
		vector <string> recordItem;
		ifstream fileToRead("purchase requisite.txt");
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
			if (i % 4 == 0) {
				currentPR = headPR;
				purchaseRequest* newPurReq = new purchaseRequest;

				newPurReq->code = record[i];
				newPurReq->dateToOrder.setDate(record[i + 1]);
				if (record[i + 2] == "true")
					newPurReq->approved = true;
				else
					newPurReq->approved = false;
				string rawdataItem = record[i + 3];

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
						currentIR = newPurReq->headIR;

						itemRequest* newItemRequested = new itemRequest;

						newItemRequested->itemid = recordItem[j];
						newItemRequested->quantity = stoi(recordItem[j + 1]);

						newItemRequested->next = NULL;
						newItemRequested->prev = NULL;

						if (newPurReq->headIR == NULL)
						{
							newPurReq->headIR = newItemRequested;
							newPurReq->tailIR = newItemRequested;
						}
						else
						{
							newItemRequested->prev = newPurReq->tailIR;
							newPurReq->tailIR->next = newItemRequested;
						}
						newPurReq->tailIR = newItemRequested;
						j = j + 2;
					}
				}

				newPurReq->next = NULL;
				newPurReq->prev = NULL;

				if (headPR == NULL)
				{
					headPR = newPurReq;
					tailPR = newPurReq;
				}
				else
				{
					newPurReq->prev = tailPR;
					tailPR->next = newPurReq;
				}
				tailPR = newPurReq;
			}
			i = i + 4;
		}
	}
	bool add() {
		try
		{
			string dateInput;
			purchaseRequest* newPurReq = new purchaseRequest;
			newPurReq->approved = false;
			newPurReq->headIR = newPurReq->tailIR = nullptr;
			cout << "Enter the following Information of the new Purchase Requisite: " << endl;
			cout << "Date to needed Order(DD-MM-YYYY): ";
			cin >> dateInput;

			if (!newPurReq->dateToOrder.setDate(dateInput)) {
				cout << endl << "Date entered has wrong format.";
				system("pause");
				system("cls");
				delete newPurReq;
				add();
			}
			else if (!newPurReq->dateToOrder.isreal()) {
				cout << endl << "Date entered isn't a real date.";
				system("pause");
				system("cls");
				delete newPurReq;
				add();
			}
			else
				newPurReq->dateToOrder.setDate(dateInput);
			cout << endl;
			itemEntry items;
			items.view();
			cout << endl;

			string id = "";
			int quant = 0, i = 1;
			while (id != "EXIT") {
				itemRequest* newItem = new itemRequest;
				cout << "Item Number " << i++ << " :\n"
					<< "Item Code (type 'EXIT' to exit with caps): ";
				cin >> id;
				if (id == "EXIT") {
					return true;
					continue;
				}
				else if (!items.search(id)) {
					cout << "Item Code Does not Exist, Please see the list above\n\n";
					delete newItem;
					i--;
					continue;
				}
				else if (i != 1 && searchItem(newPurReq, id)) {
					cout << "Item Has already been entered by you, Please enter another Item\n\n";
					delete newItem;
					i--;
					continue;
				}
				newItem->itemid = id;
				cout << "Quantity\t\t\t: ";
				cin >> newItem->quantity;

				newItem->next = newPurReq->headIR;
				if (newPurReq->headIR != NULL)
					newPurReq->headIR->prev = newItem;
				newPurReq->headIR = newItem;
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

			newPurReq->code = "PR" + to_string(_year) + month + date + hour + min + sec;
			newPurReq->next = headPR;
			if (headPR != NULL)
				headPR->prev = newPurReq;
			headPR = newPurReq;
			newPurReq->prev = NULL;
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
		string purReqCode;
		purchaseRequest* purReqDelete
		{};
		cout << "Enter Purchase Requisite Code to delete: ";
		cin >> purReqCode;

		if (search(purReqCode)) {
			purReqDelete = getData(purReqCode);
			cout << "Date\t: " << purReqDelete->dateToOrder.print();
			currentIR = purReqDelete->headIR;
			int count = 1;
			while (currentIR != NULL) {
				if (count < 2) {
					cout << "\nNo.\tItem Code\t\t\tQuantity" << endl;
				}
				cout << count << "\t" << currentIR->itemid << "\t" << currentIR->quantity << endl;
				count++;
				currentIR = currentIR->next;
			}
			if (contin()) {
				if (purReqDelete->prev == NULL) {
					headPR = purReqDelete->next;
					purReqDelete->next->prev = NULL;
					delete purReqDelete;
				}
				else if (purReqDelete->next == NULL) {
					purReqDelete->prev->next = NULL;
					delete purReqDelete;
				}
				else {
					purReqDelete->prev->next = purReqDelete->next;
					purReqDelete->next->prev = purReqDelete->prev;
					delete purReqDelete;
				}
				save();
				return true;
			}
			else {
				return false;
			}
		}
		else {
			cout << "\nPurchase Requisite not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool edit() {
		string salesCode, editedVariable, itemCode, pick1, pick2, id, quant;
		purchaseRequest* purReqEdit{};
		Date dateTest;
		itemEntry itemtest;
		int choice1 = 0, choice2;
		bool flag = true;
		cout << "Enter Purchase Requisite Code to Edit: ";
		cin >> salesCode;

		if (search(salesCode)) {
			purReqEdit = getData(salesCode);
			cout << "Date\t: " << purReqEdit->dateToOrder.print();
			currentIR = purReqEdit->headIR;
			int count = 1;
			while (currentIR != NULL) {
				if (count < 2) {
					cout << "\n\nNo.\tItem Code\t\tQuantity" << endl;
				}
				cout << count << "\t" << currentIR->itemid << "\t" << currentIR->quantity << endl;
				count++;
				currentIR = currentIR->next;
			}

			while (choice1 != 4) {
				cout << endl << "1. Edit/Delete Existing Item on list\n2. Add new Item to list\n3. Edit Date\n4. Exit\nChoose the Number : ";
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
						flag = !searchItem(purReqEdit, itemCode);
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
						else if (searchItem(purReqEdit, id)) {
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
					flag = true;
					while (flag) {
						cout << "Date to needed Order(DD-MM-YYYY): ";
						cin >> editedVariable;
						if (!dateTest.setDate(editedVariable)) {
							cout << endl << "Date entered has wrong format.";
							system("pause");
							cout << endl;
							continue;
						}
						else if (!dateTest.isreal()) {
							cout << endl << "Date entered isn't a real date.";
							system("pause");
							cout << endl;
							continue;
						}
						else
							flag = false;
					}
					
					break;
				case 4:
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
				itemRequest* itemEdit{};
				itemEdit = getItemData(purReqEdit, itemCode);
				if (choice1 == 1 && choice2 == 1) {
					itemEdit->itemid = editedVariable;
				}
				else if (choice1 == 1 && choice2 == 2) {
					itemEdit->quantity = stoi(editedVariable);
				}
				else if (choice1 == 2) {
					itemRequest* newItem = new itemRequest;
					newItem->itemid = id;
					newItem->quantity = stoi(quant);
					newItem->next = purReqEdit->headIR;
					if (purReqEdit->headIR != NULL)
						purReqEdit->headIR->prev = newItem;
					purReqEdit->headIR = newItem;
					newItem->prev = NULL;
				}
				else if (choice1 == 3) {
					purReqEdit->dateToOrder.setDate(editedVariable);
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
			cout << "\nPurchase Requisite not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool view() {
		currentPR = headPR;
		int count = 1;
		while (currentPR != NULL) {
			if (count < 2) {
				cout << "No.\tCode\t\t\tDate to Order\tApproved?\tItem Code\t\tQuantity" << endl;
			}
			cout << endl << count++ << "\t\t" << currentPR->code << "\t" << currentPR->dateToOrder.print() << "\t" << currentPR->approved << "\t";
			currentIR = currentPR->headIR;
			int i = 0;
			while (currentIR != NULL) {
				if (i++ < 1)
					cout << currentIR->itemid << "\t" << currentIR->quantity << endl;
				else
					cout << "\t\t\t\t\t\t\t\t" << currentIR->itemid << "\t" << currentIR->quantity << endl;
				currentIR = currentIR->next;
			}
			currentPR = currentPR->next;
		}
		system("pause");
		return true;;
	}
	bool search(string code) {
		currentPR = headPR;
		while (currentPR != NULL) {
			if (currentPR->code == code) {
				return true;
			}
			else {
				currentPR = currentPR->next;
			}
		}
		return false;
	}
	purchaseRequest* getData(string code) {
		currentPR = headPR;
		while (currentPR != NULL) {
			if (currentPR->code == code) {
				return currentPR;
			}
			else {
				currentPR = currentPR->next;
			}
		}
		return nullptr;
	}
};
#endif