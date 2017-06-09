#include "stdafx.h"
#include "BigSpender.h"
#include "SalesInvoice.h"

//Use null constructor to get all methods
BigSpender::BigSpender()
{
}


BigSpender::~BigSpender()
{
}

//Get and set customer name and phone
string BigSpender::getCName() 
{
	return cName;
}

string BigSpender::getCPhone() 
{
	return cPhone;
}

void BigSpender::setCName(string name) 
{
	cName = name;
}

void BigSpender::setCPhone(string phone) 
{
	cPhone = phone;
}

//Override the calValue when the sales value was over 20
float BigSpender::calValue() 
{
	//Decrease sales value by 5%
	return markUp(price) * quantity * 0.95;
}

//Override printSale when the object in SalesInvoice was BigSpender object
string BigSpender::printSale()
{
	return to_string(number) + "," + name + "," + setTwoDP(price) + "," + to_string(quantity) + "," + setTwoDP(sale) + "," + cName + "," +cPhone;
}
