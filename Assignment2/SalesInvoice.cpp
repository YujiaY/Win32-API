#include "stdafx.h"
#include "SalesInvoice.h"

//Use null constructor to get all methods
SalesInvoice::SalesInvoice()
{
}


SalesInvoice::~SalesInvoice()
{
}

//Use constructor for SalesInvoice
SalesInvoice::SalesInvoice(int sNumber, string sName, float sPrice, int sQuantity, float vSale) 
{
	number = sNumber;
	name = sName;
	price = sPrice;
	quantity = sQuantity;
	sale = vSale;
}
//Getters
int SalesInvoice::getNumber()
{
	//Get invoice number
	return number;
}

string SalesInvoice::getName()
{
	//Get selected product's name
	return name;
}

float SalesInvoice::getPrice()
{
	//Get selected product's retail price
	return price;
}

int SalesInvoice::getQuantity()
{
	//Get user's quantity
	return quantity;
}

float SalesInvoice::getValue()
{
	//Get sales value for this product
	return sale;
}

//Setters
void SalesInvoice::setNumber(int sNumber) 
{
	//Set invoice number
	number = sNumber;
}

void SalesInvoice::setName(string sName) 
{
	//Set name
	name = sName;
}

void SalesInvoice::setPrice(float sPrice) 
{
	//Set price
	price = sPrice;
}

void SalesInvoice::setQuantity(int sQuantity) 
{
	//Set quantity
	quantity = sQuantity;
}

void SalesInvoice::setValue(float vSale) 
{
	//Set sales value
	sale = vSale;
}

//Calculate retail price by increasing the product's cost by 60%
float SalesInvoice::markUp(float cost)
{
	return cost * 1.6;
}

//Calculate the sales value by the markup of the selected product by the user's required quantity
float SalesInvoice::calValue() 
{
	return markUp(price) * quantity;
}

//Print SalesInvoice details
string SalesInvoice::printSale() 
{
	return to_string(number) + "," + name + "," + setTwoDP(price) + ", Number of Item Sold: " + to_string(quantity) + ", Total Value of Sales:" + setTwoDP(sale);
}