/************************************************
*Rudd Taylor                                    *
*P1 Computer Programming						*
*Final project:Vending Machine					*
*vendingmachine.h								*
*File Purpose: Declaration of classes			*
*6/13/02										*
*Known Issues: Tax in the machine will display	*
*too many decimal places.						*
*Summary: A vending machine that contains 		*
*numerous different products. It has the 		*
*capability to view stats for individual		*
*products, or the entire machine. Sort			*
*capability has also been included, as has the 	*
*potential for tax.								*
*************************************************/

#ifndef VENDINGMACHINE_H
#define VENDINGMACHINE_H

class CProduct {
public:
	CProduct();														//default constructor
	void init(int quantity,int productNumber, char name[25], double Price);//overloaded constructor
	char szName[25];												//product name
	//all the get functions	
	const inline float getPrice()			{return fPrice;};		//get the price
	const inline float getPriceSold()		{return fPrice * iNumberSold;};
	const inline short getQuantity()		{return sQuantity;};	//get the quantity
	const inline int   getProductNumber()	{return iProductNumber;};//get the product number
	const inline int   getNumberSold()		{return iNumberSold;};	//get the number sold
	//all the change functions
	void		 isSold(int number);								//in case something is sold
	const void	 displayInfo();										//if display info is requested
private:
	float		 fPrice;											//the price of the product
	short		 sQuantity;											//the quantity left
	int			 iProductNumber;									//the product number
	int			 iNumberSold;										//the number sold
};


class CSortHandler {												//this sorts the products
public:
	CSortHandler();													//constructor
	~CSortHandler();												//destructor (deletes pointers)
	void sortInit(CProduct p1, CProduct p2, CProduct p3, CProduct p4, CProduct p5, CProduct p6, CProduct p7, CProduct p8);
	void bubbleSort();												//main bubble sort function
private:
	void bubble(CProduct *work[], const int size);					//the essence of bubble sorting
	inline const bool descending(int a, int b) {return b>a;};					//used for bubble sort
	void swapper(CProduct *pointer1, CProduct *pointer2);			//swaps two CProduct classes
	CProduct *point[8];												//the pointers to the products
};


class CMachine {													//the vending machine
public:
	CMachine(bool tax);
	void  machineMain();											//the main machine functions
	bool  endOfTheDay();											//for the end of the day
private:
	const void  displayDayStats();										//display the stats
	void  generalSort();											//general sort func, followed by tohers
	void  gatherGeneralStats();										//gather the general stats from the class
	
	bool  active;													//to see if the machine is on
	int   totalNumberSold;											//to see the total number sold
	float totalPriceSold;											//to see the total profit made
	int   totalNumberLeft;											//to see the total number left
//these are all the products
	CProduct cpButterfinger;			
	CProduct cpMountainDew;
	CProduct cpPopcorn;
	CProduct cpMicrowaveBurger;
	CProduct cpMicrowavePizza;
	CProduct cpTunaFish;
	CProduct cpBagel;
	CProduct cpTwinkies;
//this is the product sorter
	CSortHandler sorter;
};

#endif
	