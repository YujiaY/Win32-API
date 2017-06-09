#include "stdafx.h"
#include "SpareSource.h"

//Display the cost on form as money format
string convertCost(float cost)
{
	stringstream stream;
	stream << fixed << setprecision(2) << "$";
	stream << cost;
	string rCost = stream.str();
	return rCost;
}

//Write the cost from file in 2 decimal places
string setTwoDP(float value) 
{
	stringstream stream;
	stream << fixed << setprecision(2);
	stream << value;
	string twoDp = stream.str();
	return twoDp;
}