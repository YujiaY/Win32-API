#include "stdafx.h"
#include "Product.h"

//Use null constructor to get all methods
Product::Product()
{

}


Product::~Product()
{
}

//Use constructor for Product
Product::Product(string pCode, string pName, int pStock, float pCost, string pImage) 
{
	code = pCode;
	name = pName;
	stock = pStock;
	cost = pCost;
	image = pImage;
}

//Getters
int Product::getStock() 
{
	//Get stocks
	return stock;
}

string Product::getCode() 
{
	//Get code
	return code;
}

string Product::getName() 
{
	//Get name
	return name;
}

float Product::getCost() 
{
	//Get cost
	return cost;
}

string Product::getImage() 
{
	//Get image directory file
	return image;
}

//Write product details to file
string Product::printProduct() 
{
	return code + "," + name + "," + to_string(stock) + "," + setTwoDP(cost) + "," + image;
}

//Calculate retail price
float Product::markUp() 
{
	return cost * 1.6;
}

//Check if the user input is less than product's stock, return true, else false
bool Product::stockAvailable(int pStock) 
{
	if (stock < pStock) 
	{
		return false;
	}

	else 
	{
		return true;
	}
}

//Deduct product stock by the user required stock
int Product::makeSale(int pStock) 
{
	stock = stock - pStock;
	return stock;
}
