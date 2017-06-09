#include "stdafx.h"
#include "SpotPrize.h"
#include "BigSpender.h"

//Use null constructor to get all methods
SpotPrize::SpotPrize()
{
}

SpotPrize::~SpotPrize()
{
}

//Get and set spotprize code to get 'Product of the Day'
string SpotPrize::getSCode() 
{
	return sCode;
}

void SpotPrize::setSCode(string code) 
{
	sCode = code;
}

//Override the calValue when the user has selected SpotPrize and sales value from before was over $20
float SpotPrize::calValue() 
{
	//Decrease sales value by deducting half of the spot prize's retail price
	return markUp(price) * quantity * 0.45; // 0.95 -markUp(price) * quantity * 0.5;
}

//Override the printSale when the user has selected SpotPrize
string SpotPrize::printSale()
{
	return to_string(number) + "," + name + "," + setTwoDP(price) + "," + to_string(quantity) + "," + setTwoDP(sale) + "," + cName + "," + cPhone + "," + sCode;
}
