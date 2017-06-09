#pragma once
#include "SpareSource.h"

class SalesInvoice
{
	protected:
		int number, quantity;
		string name;
		float price, sale;

	public:
		SalesInvoice();
		~SalesInvoice();
		SalesInvoice(int sNumber, string sName, float sPrice, int sQuantity, float vSale);
		int getNumber();
		string getName();
		float getPrice();
		int getQuantity();
		float getValue();
		void setNumber(int sNumber);
		void setName(string sName);
		void setPrice(float sPrice);
		void setQuantity(int sQuantity);
		void setValue(float vSale);
		float markUp(float cost);
		virtual float calValue();
		virtual string printSale();
};

