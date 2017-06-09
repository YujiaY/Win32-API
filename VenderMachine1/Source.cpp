/************************************************
*Rudd Taylor                                    *
*P1 Computer Programming						*
*Final project:Vending Machine					*
*final.cpp										*
*File purpose: main source code for all			*
*non-inline functions							*
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

#include <iostream>													//for main stuff
#include <conio.h>													//for getch()
#include <iomanip>													//for system()

#include "vendingmachine.h"											//for classes

#include <tchar.h>

using namespace std;												//for namespace

CProduct::CProduct() {												//default constructor, used in case of error
	fPrice = 0;														//price to zero
	sQuantity = 0;													//quant to zero
	iNumberSold = 0;												//number sold to zero
	iProductNumber = 0;												//prod number to zero
	strcpy(szName, "");												//name to null
}

void CProduct::init(int quantity, int productNumber, char name[25], double Price) {//what should be the constructor, but couldnt because of composition
	fPrice = Price;													//set price to desired price
	strcpy(szName, name);											//set name to desired name
	iProductNumber = productNumber;									//set prod num to desired num
	sQuantity = quantity;											//set quantity to desired quantity
	iNumberSold = 0;												//set number sold to zero
}

const void CProduct::displayInfo() {								//displays info about a certain product
	system("cls");													//clearscreen
	cout << endl << endl << endl;									//give a little room
	cout << "Basic Product Info for " << szName << endl;			//display info
	cout << "\tPrice : " << fPrice << endl;
	cout << "\tProduct Number : " << iProductNumber << endl;
	cout << "\tQuantity Left : " << sQuantity << endl;
	cout << "\tNumber Sold : " << iNumberSold << endl;
	getch();
}

void CProduct::isSold(int number) {									//in case something is sold...
	if (sQuantity <= 0) {											//check if availible
		system("cls");
		cout << szName << " is out of stock" << endl				//if not...
			<< "Please try again tomorrow" << endl;
		getch();
		return;
	}
	else if (sQuantity - number < 0) {
		system("cls");
		cout << "There are not that many of this item, please try again" << endl;
		getch();
	}
	else {															//if so...
		for (int x = 1; x <= number; x++) {
			++iNumberSold;
			--sQuantity;
		}
	}
}

CMachine::CMachine(bool tax) {										//machine constructor, used to initialize all the products and machine info. input of bool tax (yes or no to tax)
	double addTax = 1;												//for tax
	if (tax)															//if yes on tax
		addTax = 1.08;												//make addTax 1.08 (tax amount)
																	//set all the products to their initial values....
	cpButterfinger.init(100, 1, "Butterfinger", .5 * addTax);
	cpMountainDew.init(200, 2, "Mountain Dew", .6 * addTax);
	cpPopcorn.init(50, 3, "Popcorn", 1 * addTax);
	cpMicrowaveBurger.init(30, 4, "Microwave Burger", 2 * addTax);
	cpMicrowavePizza.init(30, 5, "Microwave Pizza", 3 * addTax);
	cpTunaFish.init(300, 6, "Tuna Fish", .68 * addTax);
	cpBagel.init(120, 7, "Bagel", .49 * addTax);
	cpTwinkies.init(500, 8, "Twinkies", 1 * addTax);

	active = true;													//set machine as active
	totalNumberSold = 0;											//set default sales
	totalNumberLeft = 0;
	totalPriceSold = 0;
}

bool CMachine::endOfTheDay() {										//at the end of the day
	short choice;													//the menu choice
	system("cls");
	cout << "It is the end of the day, what do you want to do?" << endl//give the options
		<< "(1) View General Stats" << endl
		<< "(2) Sort data" << endl
		<< "(3) Quit" << endl;
	cin >> choice;													//input option
	switch (choice) {
	case 1:															//case display general stats
		displayDayStats();
		break;
	case 2:
		generalSort();												//case sort
		break;
	case 3:
		return false;												//case quit
	default:
		return true;												//default case
	}
	return true;
}

void CMachine::generalSort() {										//the general sort function
	sorter.sortInit(cpButterfinger, cpMountainDew, cpPopcorn, cpMicrowaveBurger, cpMicrowavePizza, cpTunaFish, cpBagel, cpTwinkies);
	sorter.bubbleSort();
	getch();
}
void CMachine::gatherGeneralStats() {								//get all the stats
	int temp;														//used for total price, total number left, total number sold

	temp = cpButterfinger.getPriceSold() + cpMountainDew.getPriceSold()
		+ cpPopcorn.getPriceSold() + cpMicrowaveBurger.getPriceSold()
		+ cpMicrowavePizza.getPriceSold() + cpTunaFish.getPriceSold()
		+ cpBagel.getPriceSold() + cpTwinkies.getPriceSold();
	totalPriceSold = temp;

	temp = cpButterfinger.getQuantity() + cpMountainDew.getQuantity()
		+ cpPopcorn.getQuantity() + cpMicrowaveBurger.getQuantity()
		+ cpMicrowavePizza.getQuantity() + cpTunaFish.getQuantity()
		+ cpBagel.getQuantity() + cpTwinkies.getQuantity();
	totalNumberLeft = temp;

	temp = cpButterfinger.getNumberSold() + cpMountainDew.getNumberSold()
		+ cpPopcorn.getNumberSold() + cpMicrowaveBurger.getNumberSold()
		+ cpMicrowavePizza.getNumberSold() + cpTunaFish.getNumberSold()
		+ cpBagel.getNumberSold() + cpTwinkies.getNumberSold();
	totalNumberSold = temp;
}

const void CMachine::displayDayStats() {							//displays the days stats
	gatherGeneralStats();											//gather stats before displaying

	short choice;													//for menu choice
	CProduct *cpointer = new CProduct;								//initialize pointer
	system("cls");													//clearscreen
	cout << "Vending Machine Stats for Today: \n";					//display menu
	cout << "Total number of items sold : " << totalNumberSold << endl
		<< "Total number of items left : " << totalNumberLeft << endl
		<< "Total profit made today : " << totalPriceSold << endl << endl;

	cout << "Please press a number to view specific stats\npress 0 to go to main screen\n";
	cout << "(1) Butterfinger" << endl
		<< "(2) Mountain Dew" << endl
		<< "(3) Popcorn" << endl
		<< "(4) Microwave Burger" << endl
		<< "(5) Microwave Pizza" << endl
		<< "(6) Tuna Fish" << endl
		<< "(7) Bagel " << endl
		<< "(8) Twinkies " << endl;
	cin >> choice;													//input choice
	switch (choice) {												//menu switch, set pointer equal to whatever you want to view stats for
	case 1:
		*cpointer = cpButterfinger;
		break;
	case 2:
		*cpointer = cpMountainDew;
		break;
	case 3:
		*cpointer = cpPopcorn;
		break;
	case 4:
		*cpointer = cpMicrowaveBurger;
		break;
	case 5:
		*cpointer = cpMicrowavePizza;
		break;
	case 6:
		*cpointer = cpTunaFish;
		break;
	case 7:
		*cpointer = cpBagel;
		break;
	case 8:
		*cpointer = cpTwinkies;
		break;
	case 0:
		return;
		break;
	}
	cpointer->displayInfo();										//display stats for chosen class 
	delete cpointer;												//delete the pointer
}

void CMachine::machineMain() {										//the main machine
	short choice, amount;											//choice for menu, amount for amount
	bool end = false;												//end the menu, set to false

	while (active) {													//while the machine is on...
		system("cls");
		cout << "\t************************************\n"
			<< "\t*           Vending Machine        *\n"
			<< "\t************************************\n"
			<< "\t*Product Name       *#Left * Price *\n"
			<< "\t************************************\n"
			<< "\t*(1)Butterfinger    * " << cpButterfinger.getQuantity() << "     " << cpButterfinger.getPrice() << "     \n"
			<< "\t*(2)Mountain Dew    * " << cpMountainDew.getQuantity() << "     " << cpMountainDew.getPrice() << "     \n"
			<< "\t*(3)Popcorn         * " << cpPopcorn.getQuantity() << "        " << cpPopcorn.getPrice() << "     \n"
			<< "\t*(4)Microwave Burger* " << cpMicrowaveBurger.getQuantity() << "        " << cpMicrowaveBurger.getPrice() << "     \n"
			<< "\t*(5)Microwave Pizza * " << cpMicrowavePizza.getQuantity() << "        " << cpMicrowavePizza.getPrice() << "     \n"
			<< "\t*(6)Tuna Fish       * " << cpTunaFish.getQuantity() << "     " << cpTunaFish.getPrice() << "     \n"
			<< "\t*(7)Bagel           * " << cpBagel.getQuantity() << "     " << cpBagel.getPrice() << "     \n"
			<< "\t*(8)Twinkies        * " << cpTwinkies.getQuantity() << "       " << cpTwinkies.getPrice() << "     \n"
			<< "\t*(0)View Sales Info *               \n"
			<< "\t************************************\n"
			<< "\t************************************\n"
			<< "\t*  Machine Made by Rudd  *         *\n"
			<< "\t************************************\n"
			<< "\t************************************\n"
			<< "\t* Administration: Press 77 to end  *\n"
			<< "\t************************************\n";
		cout << endl << "Your Selection: ";
		cin >> choice;												//cin menu choice
		if (choice == 77) {											//check for admin entry
			active = false;
		}
		else if (choice == 0) {										//check for sales info
			displayDayStats();
		}
		else if ((choice > 9) && (choice != 0) && (choice != 77)) {	//check for error in entry
			cout << "Invalid product code" << endl;
			getch();
		}
		else {
			cout << "How many?" << endl;							//ask for how many
			cin >> amount;											//enter how many
			switch (choice) {										//the selling menu for buying, uses whatever product.isSold(how many you want) to signal buying
			case 1:
				cpButterfinger.isSold(amount);
				break;
			case 2:
				cpMountainDew.isSold(amount);
				break;
			case 3:
				cpPopcorn.isSold(amount);
				break;
			case 4:
				cpMicrowaveBurger.isSold(amount);
				break;
			case 5:
				cpMicrowavePizza.isSold(amount);
				break;
			case 6:
				cpTunaFish.isSold(amount);
				break;
			case 7:
				cpBagel.isSold(amount);
				break;
			case 8:
				cpTwinkies.isSold(amount);
				break;
			default:
				getch();
				break;
			}
		}
	}
	end = true;														//end the menu
	while (end)														//while the menu is ended, display end of the day
		end = endOfTheDay();
	return;															//then quit
}

CSortHandler::CSortHandler() {										//for sorter, initialize all pointers
	point[0] = new CProduct;
	point[1] = new CProduct;
	point[2] = new CProduct;
	point[3] = new CProduct;
	point[4] = new CProduct;
	point[5] = new CProduct;
	point[6] = new CProduct;
	point[7] = new CProduct;
	point[8] = new CProduct;

	point[0] = NULL;												//to make sure [0] is set to nothing
}

CSortHandler::~CSortHandler() {										//deletes all pointers
	for (int x = 1; x <= 8; x++) {
		delete point[x];
	}
}

void CSortHandler::sortInit(CProduct p1, CProduct p2, CProduct p3, CProduct p4, CProduct p5, CProduct p6, CProduct p7, CProduct p8) {	//initialize all pointers to wanted classes
	*point[1] = p1;
	*point[2] = p2;
	*point[3] = p3;
	*point[4] = p4;
	*point[5] = p5;
	*point[6] = p6;
	*point[7] = p7;
	*point[8] = p8;
}

void CSortHandler::bubbleSort() {									//main bubble sort function
	const int arraySize = 8;										//array size is 8
	int counter;													//for the counter
	cout << "Sorting by Number of items sold\n";					//display how items are being sold
	cout << "\nData items in descending order\n";
	getch();

	bubble(point, arraySize);										//sort all items through point[]
	for (counter = 1; counter <= arraySize; counter++) {			//display sorted items
		cout << setw(4) << point[counter]->szName << " (Sold " << point[counter]->getNumberSold() << ")\n";
	}
	return;
}

void CSortHandler::bubble(CProduct *point[], const int size) {		//sort all items through point[]
	for (int pass = 1; pass <= size; pass++) {						//sorting for
		for (int count = 1; count < size; count++) {					//sorting for
			if (descending(point[count]->getNumberSold(), point[count + 1]->getNumberSold())) {	//if one is bigger than the other...
				swapper(point[count], point[count + 1]);				//swap them
			}
		}
	}
}

void CSortHandler::swapper(CProduct *pointer1, CProduct *pointer2) {//swaps two CProduct classes
	CProduct temp;
	temp = *pointer1;
	*pointer1 = *pointer2;
	*pointer2 = temp;
}

void main() {														//main
	char choice;													//tax choice
	bool tax = false;												//if tax is wanted
	cout << "Will there be tax in the machine? (y)es or (n)o" << endl;//ask if tax
	cin >> choice;													//cin choice 
	if (choice == 'y')												//if tax...
		tax = true;
	CMachine theVendingMachine(tax);								//initialize and declare machine (with input of tax)
	theVendingMachine.machineMain();								//start the machine
	return;															//end the program
}