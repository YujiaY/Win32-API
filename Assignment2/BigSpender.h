#pragma once
#include "SalesInvoice.h"

class BigSpender : public SalesInvoice
{
	protected:
		string cName, cPhone;

	public:
		BigSpender();
		~BigSpender();
		//Use the inheritance constructor in the header to avoid dependency error in source code
		//Inherited with SalesInvoice to relate with BigSpender
		BigSpender(int sNumber, string sName, float sPrice, int sQuantity, float vSale,
			string name, string phone) : SalesInvoice(sNumber, sName, sPrice, sQuantity, vSale) 
		{
			cName = name;
			cPhone = phone;
		};
		string getCName();
		string getCPhone();
		void setCName(string name);
		void setCPhone(string phone);
		float calValue() override;
		string printSale() override;

};

