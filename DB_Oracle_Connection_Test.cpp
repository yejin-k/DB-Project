/////////////////////////////////////////////////////////////////
// 
// File	DBS211_DB_Connection_Test.cpp
// Version 1.0
// Author	Yejin Kim
// 
/////////////////////////////////////////////////////////////////

#include <iostream>
#include <cstring>
#include <iomanip> //custome
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

const int DEFAULT = 0;
const int INVALID_DATA_TYPE = 1;
const int NO_EXISTING_OUTPUT = 2;

const char errorMessages[2][81] = {
	"Invalid Data Type. Type again: ",
	"No Existing Output. Type again: "
};

void clearBuffer();
void displayErrMss(const int errType);
int selectEmpNum();

int errCode = DEFAULT;

int main(void) {
	Environment* env = nullptr;
	Connection* conn = nullptr;

	string str;
	string usr = "usrID";
	string pass = "usrPW";
	string srv = "oracleServer";	

	try {
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(usr, pass, srv);
		cout << "Connection is Successful!" << endl;
		cout << "------------------------------------------" << endl << endl;

		cout << "Employees table" << endl;

		cout << "Enter the employee number (####): ";
		int selected = selectEmpNum();

		Statement* stmt = conn->createStatement();

		string selectClause, fromClause, whereClause, orderClause;
		string query;

		ResultSet* rs;

		do {
			selectClause = "SELECT COUNT(*)";
			fromClause = "FROM employees";
			whereClause = "WHERE employeenumber = " + to_string(selected);
			orderClause = "";
			query = selectClause + " " + fromClause + " " + whereClause + " " + orderClause;

			rs = stmt->executeQuery(query);

			int count;
			while (rs->next()) {
				count = rs->getInt(1);
			}

			if (count == 0) {
				errCode = NO_EXISTING_OUTPUT;
				displayErrMss(errCode);
				selected = selectEmpNum();
			}
			else {
				errCode = DEFAULT;
			}
		} while (errCode != DEFAULT);

		selectClause = "SELECT employeenumber, lastname, firstname, jobtitle";
		fromClause = "FROM employees";
		whereClause = "WHERE employeenumber = " + to_string(selected);
		orderClause = "ORDER BY employeenumber";
		query = selectClause + " " + fromClause + " " + whereClause + " " + orderClause;

		rs = stmt->executeQuery(query);

		cout << "The employee with number " << selected << ":" << endl;
		cout << "# " << left << setw(15) << "employeenumber" << setw(10) << "lastname" << setw(10) << "firstname" << setw(30) << "jobtitle" << endl;

		while (rs->next()) {
			int empNum = rs->getInt(1);
			string lst = rs->getString(2);
			string fst = rs->getString(3);
			string jobtt = rs->getString(4);
			cout << "  " << setw(15) << empNum << setw(10) << lst << setw(10) << fst << setw(30) << jobtt << endl;
		}

		conn->terminateStatement(stmt);
		env->terminateConnection(conn);
		Environment::terminateEnvironment(env);
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	return 0;
}

void clearBuffer() {
	while (cin.get() != '\n');
}

void displayErrMss(const int errType) {
	if (errType != 0) {
		cout << errorMessages[errType - 1];
	}
}

int selectEmpNum() {
	bool done = false;
	int selected;
	do {
		errCode = DEFAULT;
		cin >> selected;
		if (!cin) {
			cin.clear();
			errCode = INVALID_DATA_TYPE;
			displayErrMss(errCode);
		}
		else {
			done = true;
		}
		clearBuffer();
	} while (!done);

	return selected;
}
