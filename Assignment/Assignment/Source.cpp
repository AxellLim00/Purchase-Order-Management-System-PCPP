#include <iostream>
#include "User.h"
#include "itemEntry.h"
#include "supplierEntry.h"
#include "salesEntry.h"
#include "purchaseReq.h"
#include "purchaseOrd.h"

using namespace std;

string type = "", userName = "";
int category = -1;
itemEntry items;
supplierEntry suppliers;
User users;
salesEntry sales;
purchaseOrd purOrds;
purchaseReq purReqs;

string getUserType(string);
void mmenu();//Main Menu 
void error();
bool confirm();
void innerMenu();

int main() {
	string user, pass;
	system("CLS");
	cout << "Welcome to PURCHASE ORDER MANAGEMENT SYSTEM of TRANSYSLOGICS SDN BHD\n\nPlease Enter Login Credentials:\nUsername : ";
	cin >> user;
	cout << "Password : ";
	cin >> pass;
	User currentUser;
	if (currentUser.login(user, pass)) {
		cout << "\nLogin Successful.\n";
		userName = user;
		type = currentUser.getType(user);
		system("PAUSE");
		mmenu();
	}
	else {
		cout << "\nUsername and/or Password is wrong, Please try again.\n";
		system("PAUSE");
		main();
	}
	return 0;
}
void mmenu() {
	string mmenuOption;
	category = NULL;
	system("CLS");
	cout << "Welcome to PURCHASE ORDER MANAGEMENT SYSTEM\n"
		<< "Below are Databases you can access (categorized)\n"
		<< "1. Item Entry \n"
		<< "2. Supplier Entry \n"
		<< "3. Daily Item-wise Sales Entry \n"
		<< "4. Purchase Requisition \n"
		<< "5. Purchase Order \n";
		if (type == "AD")
			cout << "6. User \n";
		cout << "0. Log Out \n"
		<< "x. Exit \n"
		<< "Please input a number corresponding to the option above : ";
	cin >> mmenuOption;
	if (mmenuOption == "X" || mmenuOption == "x")
		exit(0);
	try
	{
		category = stoi(mmenuOption);
	}
	catch (const std::exception&)
	{
		error();
		mmenu();
	}
	if (type != "AD" && category == 6) {
		error();
		mmenu();
	}
	else if (category < 0 || category > 6 ) {
			system("PAUSE");
			system("CLS");
			mmenu();
	}
	else if (category == 0) {
		if (confirm()) {
			system("PAUSE");
			system("CLS");
			main();
		}
		else {
			system("PAUSE");
			system("CLS");
			mmenu();
		}
	}
	else {
		innerMenu();
	}
}
void error() {
	cout << "Error : please input according to the OPTION or INSTRUCTION mention Above\n";
	system("PAUSE");
	system("CLS");
}
bool confirm() {
	char cont;
	cout << "\n\nDo you wish to go further?(y/n) : ";
	cin.ignore();
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
	default:
		error();
		confirm();
	}
	return false;
}
void innerMenu() {
	string mmenuOption = "";
	int pick = NULL;
	bool action = false;
	system("CLS");
	cout << "What would you like to do?\n";
	if (type == "AD" || (type == "SM" && category >= 1 && category <= 4) || (type == "PM" && category == 5))
		cout << "1. Add \n"
			<< "2. Edit \n"
			<< "3. Delete \n";

	if (((type == "AD"  || type == "PM") && (category == 1 || category == 2 || category == 4 || category == 5 )) || (type == "SM" && (category == 4 || category == 5)))
		cout << "4. View/Display \n";
	cout << "0. Back to Main Menu \n"
		<< "Please input a number corresponding to the option above : ";
	cin >> mmenuOption;
	try
	{
		pick = stoi(mmenuOption);
	}
	catch (const std::exception&)
	{
		pick = NULL;
		error();
		innerMenu();
	}
	switch (pick)
	{
	case 1:
		if (!(type == "AD" || (type == "SM" && category >= 1 && category <= 4) || (type == "PM" && category == 5))) 
			error();
		else {
			system("CLS");
			switch (category)
			{
			case 1:
				action = items.add();
				break;
			case 2:
				action = suppliers.add();
				break;
			case 3:
				action = sales.add();
				break;
			case 4:
				action = purReqs.add();
				break;
			case 5:
				action = purOrds.add(userName);
				break;
			case 6:
				action = users.add();
				break;
			default:
				cout << "Error Category is out of bounds (1 - 6)";
				break;
			}
			if (action)
				cout << "Add Success\n";
			else
				cout << "Add Fail\n";
			system("pause");
		}
		innerMenu();
		break;
	case 2:
		if (!(type == "AD" || (type == "SM" && category >= 1 && category <= 4) || (type == "PM" && category == 5))) 
			error();		
		else {
			system("CLS");
			switch (category)
			{
			case 1:
				action = items.edit();
				break;
			case 2:
				action = suppliers.edit();
				break;
			case 3:
				action = sales.edit();
				break;
			case 4:
				action = purReqs.edit();
				break;
			case 5:
				action = purOrds.edit();
				break;
			case 6:
				action = users.edit();
				break;
			default:
				cout << "Error Category is out of bounds (1 - 6)";
				break;
			}
			if (action)
				cout << "Edit Success\n";
			else
				cout << "Edit Fail\n";
			system("pause");
		}
		innerMenu();
		break;
	case 3:
		if (!(type == "AD" || (type == "SM" && category >= 1 && category <= 4) || (type == "PM" && category == 5))) 
			error();
		else {
			system("CLS");
			switch (category)
			{
			case 1:
				action = items.deletes();
				break;
			case 2:
				action = suppliers.deletes();
				break;
			case 3:
				action = sales.deletes();
				break;
			case 4:
				action = purReqs.deletes();
				break;
			case 5:
				action = purOrds.deletes();
				break;
			case 6:
				action = users.deletes();
				break;
			default:
				cout << "Error Category is out of bounds (1 - 6)";
				break;
			}
			if (action)
				cout << "Delete Success\n";
			else
				cout << "Delete Fail\n";
			system("pause");
		}
		innerMenu();
		break;
	case 4:
		if (!(((type == "AD" || type == "PM") && (category == 1 || category == 2 || category == 4 || category == 5)) || (type == "SM" && (category == 4 || category == 5))))
			error();
		else {
			system("CLS");
			switch (category)
			{
			case 1:
				items.view();
				break;
			case 2:
				suppliers.view();
				break;
			case 4:
				purReqs.view();
				break;
			case 5:
				purOrds.view();
				break;
			default:
				cout << "Error Category is out of bounds (1 - 6)";
				break;
			}
		}
		innerMenu();
		break;
	case 0:
		system("CLS");
		mmenu();
		break;
	default:
		error();
		innerMenu();
		break;
	}
}