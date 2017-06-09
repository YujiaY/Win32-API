#pragma once
#include "SpareSource.h"

class Product
{
	private:
		string code, name, image;
		int stock;
		float cost;

	public:
		Product();
		~Product();
		Product(string pCode, string pName, int pStock, float pCost, string pImage);
		string getCode();
		string getName();
		int getStock();
		float getCost();
		string getImage();
		float markUp();
		string printProduct();
		bool stockAvailable(int pStock);
		int makeSale(int pStock);
};

