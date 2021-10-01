#pragma once
#include <string>
#include "UsersList.h"
#include"QueueArr.cpp"
#define POPULATION_OF_EGYPT 1.e8

class Admin
{
private:
	const string ADMIN_PASSWORD = "adminpass12";
public:
	UsersList* userList;
	QueueArr <int>* waitingList;
	void logIn(UsersList*, QueueArr <int>*);
	void adminOperations();
	void displayAll();
	int certainRecord();
	void deliverVaccine();
	void Percentage();
	void Percentage(double);
	~Admin();
};
