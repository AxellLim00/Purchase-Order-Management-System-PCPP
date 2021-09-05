#include <ctime>
#include <string>
#include <vector>
#pragma warning(disable : 4996)
#pragma once

using namespace std;

class Date
{
private:
	int date, month, year;
public:
    Date(int d, int m, int y) {
        date = d;
        month = m;
        year = y;
    }
    Date() {
        time_t t = std::time(0); 
        tm* now = std::localtime(&t);
        date = now->tm_mday;
        month = now->tm_mon + 1;
        year = now->tm_year + 1900;
    }
    Date(string the_date) {
        vector <string> record;
        istringstream ss(the_date);

        while (ss)
        {
            string s2;
            if (!getline(ss, the_date, '-')) break;
            record.push_back(the_date);
        }

        int i = 0, size;
        size = record.size();
        while (i < size) {
            date = stoi(record[i]);
            month = stoi(record[i + 1]);
            year = stoi(record[i + 2]);
            i = i + 3;
        }
    }
    bool setDate(string the_date) {
        try
        {
            vector <string> record;
            istringstream ss(the_date);

            while (ss)
            {
                string s2;
                if (!getline(ss, the_date, '-')) break;
                record.push_back(the_date);
            }

            int i = 0, size;
            size = record.size();
            while (i < size) {
                date = stoi(record[i]);
                month = stoi(record[i + 1]);
                year = stoi(record[i + 2]);
                i = i + 3;
            }
            return true;
        }
        catch (const std::exception&)
        {
            false;
        }
        return false;
    }
	bool isreal() {
        if (year >= 1900 && year <= 9999 && month >= 1 && month <= 12)
        {
            if ((date >= 1 && date <= 31) && (month == 1 || month == 3 || month == 5 || month == 7 || month == 8 || month == 10 || month == 12))
                return true;
            else if ((date >= 1 && date <= 30) && (month == 4 || month == 6 || month == 9 || month == 11))
                return true;
            else if ((date >= 1 && date <= 28) && (month == 2))
                return true;
            else if (date == 29 && month == 2 && (year % 400 == 0 || (year % 4 == 0 && year % 100 != 0)))
                return true;
            else
                return false;
        }
        else
        {
            return false;
        }
	}
    string print() {
        return to_string(date) + "-" + to_string(month) + "-" + to_string(year);
    }
};

