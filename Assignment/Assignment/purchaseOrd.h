#include <string>
#include <fstream>
#include <sstream>
#include <vector>
#include <iostream>
#include <ctime>
#include "Date.h"
#include "itemEntry.h"
#include "purchaseReq.h"
#include "ViewableData.h"
#ifndef __PURCHASE_ORDER__
#define __PURCHASE_ORDER__

using namespace std;

struct itemOrder {
	string itemid;
	int quantity = 0;
	itemOrder* next = nullptr, * prev = nullptr;
};

struct purchaseOrder {
	string code, approval;
	Date dateToOrder;
	itemOrder* headIO = nullptr, * tailIO= nullptr;
	purchaseOrder* next = nullptr, * prev = nullptr;
};

class purchaseOrd : public ViewableData
{
private:
	itemOrder* currentIO;
	purchaseOrder* headPO, * tailPO, * currentPO;
	purchaseOrder* getData(string code) {
		currentPO = headPO;
		while (currentPO != NULL) {
			if (currentPO->code == code) {
				return currentPO;
			}
			else {
				currentPO = currentPO->next;
			}
		}
		return nullptr;
	}
	itemOrder* getItemData(purchaseOrder* purOrd, string itemCode) {
		currentIO = purOrd->headIO;
		while (currentIO != NULL) {
			if (currentIO->itemid == itemCode) {
				return currentIO;
			}
			else {
				currentIO = currentIO->next;
			}
		}
		return nullptr;
	}
	bool searchItem(purchaseOrder* purOrd, string itemCode) {
		currentIO = purOrd->headIO;
		while (currentIO != NULL) {
			if (currentIO->itemid == itemCode) {
				return true;
			}
			else {
				currentIO = currentIO->next;
			}
		}
		return false;
	}
	bool save() {
		try
		{
			ofstream fileToWrtie("purchase order.txt");
			currentPO = headPO;
			while (currentPO != NULL)
			{
				fileToWrtie << currentPO->code << "," << currentPO->dateToOrder.print() << "," << currentPO->approval << ",";
				currentIO = currentPO->headIO;
				while (currentIO != NULL)
				{
					if (currentIO->next == nullptr) {
						fileToWrtie << currentIO->itemid << "|" << currentIO->quantity << endl;
					}
					else {
						fileToWrtie << currentIO->itemid << "|" << currentIO->quantity << "|";
					}
					currentIO = currentIO->next;
				}
				currentPO = currentPO->next;
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
	purchaseOrd() {
		headPO = tailPO = currentPO = nullptr;
		currentIO = nullptr;
		vector <string> record;
		vector <string> recordItem;
		ifstream fileToRead("purchase order.txt");
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
				currentPO = headPO;
				purchaseOrder* newPurOrd = new purchaseOrder;

				newPurOrd->code = record[i];
				newPurOrd->dateToOrder.setDate(record[i + 1]);
				newPurOrd->approval = record[i + 2];
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
						currentIO = newPurOrd->headIO;

						itemOrder* newItemOrder = new itemOrder;

						newItemOrder->itemid = recordItem[j];
						newItemOrder->quantity = stoi(recordItem[j + 1]);

						newItemOrder->next = NULL;
						newItemOrder->prev = NULL;

						if (newPurOrd->headIO == NULL)
						{
							newPurOrd->headIO = newItemOrder;
							newPurOrd->tailIO = newItemOrder;
						}
						else
						{
							newItemOrder->prev = newPurOrd->tailIO;
							newPurOrd->tailIO->next = newItemOrder;
						}
						newPurOrd->tailIO = newItemOrder;
						j = j + 2;
					}
				}

				newPurOrd->next = NULL;
				newPurOrd->prev = NULL;

				if (headPO == NULL)
				{
					headPO = newPurOrd;
					tailPO = newPurOrd;
				}
				else
				{
					newPurOrd->prev = tailPO;
					tailPO->next = newPurOrd;
				}
				tailPO = newPurOrd;
			}
			i = i + 4;
		}
	}
	bool add(string user) {
		try
		{
			string PRcode;
			purchaseOrder* newPurOrd = new purchaseOrder;
			purchaseReq PR;
			purchaseRequest prToApprove;
			itemRequest* currentIR;
			newPurOrd->headIO = newPurOrd->tailIO = nullptr;
			PR.view();
			cout << "\nEnter the following Information of the new Purchase Order: " << endl;
			cout << "Enter the Purchase Requisite Code to Approve : ";
			cin >> PRcode;
			if (!PR.search(PRcode)) {
				cout << "Purchase Requisite Code Does not exist, please the the list again";
				return false;
			}
			else if (PR.getData(PRcode)->approved == true) {
				cout << "\nPurchase Requisition has been Approved. Please Edit it to access it or approve another one.";
				return false;
			}
			else {
				prToApprove = *PR.getData(PRcode);
				cout << "Date\t: " << prToApprove.dateToOrder.print();
				currentIR = prToApprove.headIR;
				int count = 1;
				while (currentIR != NULL) {
					if (count < 2) {
						cout << "\nNo.\tItem Code\t\t\tQuantity" << endl;
					}
					cout << count << "\t" << currentIR->itemid << "\t" << currentIR->quantity << endl;
					count++;
					currentIR = currentIR->next;
				}
			}
			if (!contin()) {
				return false;
			}
			else {

				newPurOrd->dateToOrder = prToApprove.dateToOrder;
				newPurOrd->approval = user;
				currentIR = prToApprove.headIR;
				while (currentIR != NULL) {
					itemOrder* newItem = new itemOrder;
					newItem->itemid = currentIR->itemid;
					newItem->quantity = currentIR->quantity;
					
					newItem->next = newPurOrd->headIO;
					if (newPurOrd->headIO != NULL)
						newPurOrd->headIO->prev = newItem;
					newPurOrd->headIO = newItem;
					newItem->prev = NULL;

					currentIR = currentIR->next;
				}
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

			newPurOrd->code = "PO" + to_string(_year) + month + date + hour + min + sec;
			newPurOrd->next = headPO;
			if (headPO != NULL)
				headPO->prev = newPurOrd;
			headPO = newPurOrd;
			newPurOrd->prev = NULL;
			save();
			return true;
		}
		catch (const std::exception&)
		{
			return false;
		}
	}
	bool deletes() {
		string purOrdCode;
		purchaseOrder* purOrdDelete{};
		cout << "Enter Purchase Order Code to delete: ";
		cin >> purOrdCode;

		if (search(purOrdCode)) {
			purOrdDelete = getData(purOrdCode);
			cout << "Date\t: " << purOrdDelete->dateToOrder.print() << endl;
			cout << "Approval\t: " << purOrdDelete->approval << endl;
			currentIO = purOrdDelete->headIO;
			int count = 1;
			while (currentIO != NULL) {
				if (count < 2) {
					cout << "No.\tItem Code\t\t\tQuantity" << endl;
				}
				cout << count << "\t" << currentIO->itemid << "\t" << currentIO->quantity << endl;
				count++;
				currentIO = currentIO->next;
			}
			if (contin()) {
				if (purOrdDelete->prev == NULL) {
					headPO = purOrdDelete->next;
					purOrdDelete->next->prev = NULL;
					delete purOrdDelete;
				}
				else if (purOrdDelete->next == NULL) {
					purOrdDelete->prev->next = NULL;
					delete purOrdDelete;
				}
				else {
					purOrdDelete->prev->next = purOrdDelete->next;
					purOrdDelete->next->prev = purOrdDelete->prev;
					delete purOrdDelete;
				}
				save();
				return true;
			}
			else {
				return false;
			}
		}
		else {
			cout << "\nPurchase Order not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool edit() {
		string salesCode, editedVariable, itemCode, pick1, pick2, id, quant;
		purchaseOrder* purOrdEdit{};
		Date dateTest;
		itemEntry itemtest;
		int choice1 = 0, choice2;
		bool flag = true;
		cout << "Enter Purchase Order Code to Edit: ";
		cin >> salesCode;

		if (search(salesCode)) {
			purOrdEdit = getData(salesCode);
			cout << "Date\t: " << purOrdEdit->dateToOrder.print() << endl;
			cout << "Approval\t: " << purOrdEdit->approval << endl;
			currentIO = purOrdEdit->headIO;
			int count = 1;
			while (currentIO != NULL) {
				if (count < 2) {
					cout << "\nNo.\tItem Code\t\tQuantity" << endl;
				}
				cout << count << "\t" << currentIO->itemid << "\t" << currentIO->quantity << endl;
				count++;
				currentIO = currentIO->next;
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
						flag = !searchItem(purOrdEdit, itemCode);
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
						else if (searchItem(purOrdEdit, id)) {
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
							cout << "ERROR: Please Enter a number\n";
							continue;
						}
						flag = false;
					}

					break;
				case 3:
					flag = true;
					while (flag) {
						cout << "\nDate to needed Order(DD-MM-YYYY): ";
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
				itemOrder* itemEdit{};
				itemEdit = getItemData(purOrdEdit, itemCode);
				if (choice1 == 1 && choice2 == 1) {
					itemEdit->itemid = editedVariable;
				}
				else if (choice1 == 1 && choice2 == 2) {
					itemEdit->quantity = stoi(editedVariable);
				}
				else if (choice1 == 2) {
					itemOrder* newItem = new itemOrder;
					newItem->itemid = id;
					newItem->quantity = stoi(quant);
					newItem->next = purOrdEdit->headIO;
					if (purOrdEdit->headIO != NULL)
						purOrdEdit->headIO->prev = newItem;
					purOrdEdit->headIO = newItem;
					newItem->prev = NULL;
				}
				else if (choice1 == 3) {
					purOrdEdit->dateToOrder.setDate(editedVariable);
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
			cout << "\nPurchase Order not found, please make sure the Code is correct\n";
			return false;
		}
	}
	bool view() {
		currentPO = headPO;
		int count = 1;
		while (currentPO != NULL) {
			if (count < 2) {
				cout << "No.\tCode\t\t\tDate to Order\tApproval\tItem Code\t\tQuantity" << endl;
			}
			cout << endl << count++ << "\t" << currentPO->code << "\t" << currentPO->dateToOrder.print() << "\t" << currentPO->approval << "\t\t";
			currentIO = currentPO->headIO;
			int i = 0;
			while (currentIO != NULL) {
				if (i++ < 1)
					cout << currentIO->itemid << "\t" << currentIO->quantity << endl;
				else
					cout << "\t\t\t\t\t\t\t\t" << currentIO->itemid << "\t" << currentIO->quantity << endl;
				currentIO = currentIO->next;
			}
			currentPO = currentPO->next;
		}
		system("pause");
		return true;;
	}
	bool search(string code) {
		currentPO = headPO;
		while (currentPO != NULL) {
			if (currentPO->code == code) {
				return true;
			}
			else {
				currentPO = currentPO->next;
			}
		}
		return false;
	}
};

#endif