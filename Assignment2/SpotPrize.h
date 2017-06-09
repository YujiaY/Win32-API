#pragma once
#include "BigSpender.h"

class SpotPrize : public BigSpender
{
	private:
		string sCode;
	public:
		SpotPrize();
		~SpotPrize();
		//Use the inheritance constructor in the header to avoid dependency error in source code
		//Inherited with BigSpender to relate with SpotPrize
		SpotPrize(string code, int sNumber, string sName, float sPrice, int sQuantity, float vSale,
			string name, string phone) : BigSpender(sNumber, sName, sPrice, sQuantity, vSale, name, phone) 
		{
			sCode = code;
		};
		string getSCode();
		void setSCode(string code);
		float calValue() override;
		string printSale() override;
};

