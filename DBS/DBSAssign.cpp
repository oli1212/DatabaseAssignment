#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <cstring>
#include <occi.h>

using oracle::occi::Environment;
using oracle::occi::Connection;
using namespace oracle::occi;
using namespace std;

struct Employee { // structure declaration
	int employeeNumber;
	char lastName[50];
	char firstName[50];
	char extension[10];
	char email[100];
	char officecode[10];
	int reportsTo;
	char jobTitle[50];
};
int menu(void);
int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp);
void displayEmployee(Connection* conn, struct Employee emp);
void displayAllEmployees(Connection* conn);
void getEmployee(struct Employee* emp);
void insertEmployees(Connection* conn, struct Employee* emp);
void insertEmployee(Connection* conn, struct Employee* emp);
void displayUpdateEmployee(Connection* conn);
void updateEmployee(Connection* conn, int employeeNumber);
void updateEmployee(Connection* conn, Employee* emp);
void displayDeleteEmployee(Environment* env, Connection* conn);
void deleteEmployee(Connection* conn, int employeeNumber);

int menu(void) { // print menu for user
	int input = 0;

	cout << "********************* HR Menu *********************" << endl;
	cout << "1) Find Employee" << endl;
	cout << "2) Employees Report" << endl;
	cout << "3) Add Employee" << endl;
	cout << "4) Update Employee" << endl;
	cout << "5) Remove Employee" << endl;
	cout << "0) Exit" << endl;
	cout << "Enter an option (0-5): ";
	cin >> input;
	while (input > 5 || input < 0) {
		cout << "Incorrect input, enter an option (0-5): ";
		cin >> input;
	}
	return input;
}

int findEmployee(Connection* conn, int employeeNumber, struct Employee* emp) { // search for employee data with the user inputted employee number
	Statement* stmt = conn->createStatement("SELECT * FROM employees WHERE employeeNumber = :1");
	stmt->setInt(1, employeeNumber);
	ResultSet* rs = stmt->executeQuery();

	int rsi = 0;

	if (rs->next())
	{
		emp->employeeNumber = rs->getInt(1);
		strcpy(emp->lastName, rs->getString(2).c_str());
		strcpy(emp->firstName, rs->getString(3).c_str());
		strcpy(emp->extension, rs->getString(4).c_str());
		strcpy(emp->email, rs->getString(5).c_str());
		strcpy(emp->officecode, rs->getString(6).c_str());
		emp->reportsTo = rs->getInt(7);
		strcpy(emp->jobTitle, rs->getString(8).c_str());
		return rsi = 1;
	}
	conn->terminateStatement(stmt);
	return rsi;
}             
void displayEmployee(Connection* conn, struct Employee emp) { // display employees data
	int input;
	int result;
	cout << "Enter Employee Number: ";
	cin >> input;
	result = findEmployee(conn, input, &emp);
	if (result == 0) {
		cout << "Employee " << input << " does not exist." << endl;
		cout << endl;
	}
	else if (result == 1) {
		Statement* stmt = conn->createStatement("SELECT * FROM employees WHERE employeeNumber = :1");
		stmt->setInt(1, input);
		ResultSet* rs = stmt->executeQuery();
		if (!rs->next()) {
			cout << "error";
		}
		else {
			emp.employeeNumber = rs->getInt(1);
			strcpy(emp.lastName, rs->getString(2).c_str());
			strcpy(emp.firstName, rs->getString(3).c_str());
			strcpy(emp.extension, rs->getString(4).c_str());
			strcpy(emp.email, rs->getString(5).c_str());
			strcpy(emp.officecode, rs->getString(6).c_str());
			emp.reportsTo = rs->getInt(7);
			strcpy(emp.jobTitle, rs->getString(8).c_str());
		}

		cout << endl;
		cout << "-------------- Employee Information -------------" << endl;
		cout << "Employee Number: " << emp.employeeNumber << endl;
		cout << "Last Name: " << emp.lastName << endl;
		cout << "First Name: " << emp.firstName << endl;
		cout << "Extension: " << emp.extension << endl;
		cout << "Email: " << emp.email << endl;
		cout << "Office Code: " << emp.officecode << endl;
		cout << "Manager ID: " << emp.reportsTo << endl;
		cout << "Job Title: " << emp.jobTitle << endl;
		cout << endl;
		conn->terminateStatement(stmt);
	}

}
void displayAllEmployees(Connection* conn) { // display all employees if there are
	Statement* stmt = conn->createStatement("SELECT e.employeenumber,e.firstname || ' ' || e.lastname AS fullname, e.email, offices.phone, e.extension, m.firstname || ' ' || m.lastname AS managername FROM employees e JOIN offices USING(officecode) LEFT OUTER JOIN employees m ON e.reportsto = m.employeenumber ORDER BY employeenumber ASC");
	ResultSet* rs = stmt->executeQuery();
	if (!rs->next()) {
		//if the result set is empty
		cout << "ResultSet is empty." << endl;
	}
	else {
		cout << "------   ---------------    ---------------------------------  ----------------  ---------   -----------------" << endl;
		cout << "ID       Employee Name      Email                              Phone             Extension   Manager Name     " << endl;
		cout << "-----    ---------------    ---------------------------------  ----------------  ---------   -----------------" << endl;
		do {
			cout << left;
			cout.fill(' ');
			cout.precision(9);
			cout.width(9);
			cout << rs->getInt(1);
			cout.width(19);
			cout << rs->getString(2);
			cout.width(35);
			cout << rs->getString(3);
			cout.width(18);
			cout << rs->getString(4);
			cout.width(12);
			cout << rs->getString(5);
			cout << rs->getString(6);
			cout << endl;
		} while (rs->next());
	}
	cout << endl;
	conn->terminateStatement(stmt);
}

void getEmployee(struct Employee* emp)
{
	cout << "-------------- New Employee Information -------------" << endl;
	cout << "Employee Number: ";
	cin >> emp->employeeNumber;
	cout << "Last Name: ";
	cin >> emp->lastName;
	cout << "First Name: ";
	cin >> emp->firstName;
	cout << "Extension: ";
	cin >> emp->extension;
	cout << "Email: ";
	cin >> emp->email;
	cout << "Office Code: ";
	cin >> emp->officecode;
	cout << "Manager ID: ";
	cin >> emp->reportsTo;
	cout << "Job Title: ";
	cin >> emp->jobTitle;
}

void insertEmployee(Connection* conn, struct Employee* emp)
{
	Statement* stmt = conn->createStatement("INSERT INTO employees values (:c1, :c2, :c3, :c4,:c5, :c6, : c7, : c8)");
	stmt->setInt(1, emp->employeeNumber);
	stmt->setString(2, emp->lastName);
	stmt->setString(3, emp->firstName);
	stmt->setString(4, emp->extension);
	stmt->setString(5, emp->email);
	stmt->setString(6, emp->officecode);
	stmt->setInt(7, emp->reportsTo);
	stmt->setString(8, emp->jobTitle);
	stmt->executeUpdate();
	conn->commit();
	conn->terminateStatement(stmt);
}

void insertEmployees(Connection* conn, struct Employee* emp)
{
	int checkEmployee = findEmployee(conn, emp->employeeNumber, emp);
	if (checkEmployee == 0)
	{
		insertEmployee(conn, emp);
		cout << "\nThe new employee is added successfully\n" << endl;
	}
	else
	{
		cout << "\nAn employee with the same employee number exists.\n" << endl;
	}
}

void displayUpdateEmployee(Connection* conn)
{
	int employeeNumber = 0;
	cout << "Employee Number: ";
	cin >> employeeNumber;
	updateEmployee(conn, employeeNumber);
}

void updateEmployee(Connection* conn, int employeeNumber)
{
	Employee* emp = new Employee();
	int checkEmployee = findEmployee(conn, employeeNumber, emp);
	if (checkEmployee > 0)
	{
		cout << "Last Name:";
		cout << emp->lastName << endl;
		cout << "First Name:";
		cout << emp->firstName << endl;
		cout << "Extension: ";
		cin >> emp->extension;
		updateEmployee(conn, emp);
		cout << "The employee's extension is updated successfully." << endl << endl;
	}
	else
	{
		cout << "The employee with ID " << employeeNumber << " does not exist." << endl << endl;
	}
}

void updateEmployee(Connection* conn, Employee* emp)
{
	string query = "UPDATE employees SET EXTENSION =:c1 WHERE EMPLOYEENUMBER=:c2";
	Statement* stmt = conn->createStatement(query);
	stmt->setString(1, emp->extension);
	stmt->setInt(2, emp->employeeNumber);
	stmt->executeUpdate();
	conn->commit();
	conn->terminateStatement(stmt);
}

void displayDeleteEmployee(Environment* env, Connection* conn)
{
	int employeeNumber = 0;
	cout << "Employee Number: ";
	cin >> employeeNumber;
	deleteEmployee(conn, employeeNumber);
	cout << "" << endl;
}


void deleteEmployee(Connection* conn, int employeeNumber)
{
	Employee* emp = new Employee();
	int checkEmployee = findEmployee(conn, employeeNumber, emp);

	if (checkEmployee > 0)
	{
		string query = "DELETE FROM employees WHERE EMPLOYEENUMBER=:c1";
		Statement* stmt = conn->createStatement(query);
		stmt->setInt(1, employeeNumber);
		stmt->execute();
		conn->commit();
		conn->terminateStatement(stmt);
		cout << "The employee with ID " << employeeNumber << " is deleted successfully." << endl;
	}

	else
	{
		cout << "The employee with ID " << employeeNumber << " does not exist." << endl;
	}
}

int main(void)
{
	int option;
	int quit = 0;
	/* OCCI Variables */
	Environment* env = nullptr;
	Connection* conn = nullptr;
	Statement* stmt = nullptr;
	ResultSet* rs = nullptr;
	Employee emps{};
	
	/* Used Variables */
	string user = "dbs211_211f17";
	string pass = "89221253";
	string constr = "myoracle12c.senecacollege.ca:1521/oracle12c";
	try {
		env = Environment::createEnvironment(Environment::DEFAULT);
		conn = env->createConnection(user, pass, constr);
		do {
			option = menu();
			if (option == 1) { // finding and printing employee
				displayEmployee(conn, emps);
			}
			else if (option == 2) {
				displayAllEmployees(conn);
			}
			else if (option == 3) {
				getEmployee(&emps);
				insertEmployees(conn, &emps);
			}
			else if (option == 4) {
				displayUpdateEmployee(conn);
			}
			else if (option == 5) {
				displayDeleteEmployee(env, conn);
			}
			else if (option == 0) {
				quit = 1;
			}
		} while (quit != 1);

		env->terminateConnection(conn); // close connection
		Environment::terminateEnvironment(env);
	}
	catch (SQLException& sqlExcp) {
		cout << sqlExcp.getErrorCode() << ": " << sqlExcp.getMessage();
	}
	return 0;
}	

