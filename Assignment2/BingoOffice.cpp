// BingoOffice.cpp : Defines the entry point for the application.
//
#include "stdafx.h"
#include "BingoOffice.h"
#include <fstream> //Buffering file string
#include <string> //Enable strings
#include <iostream> //Read file and write file
#include <sstream> //Create a stringstream to combine string and array of characters into one string
#include "Product.h" //Use the Product class header to use those methods and constructors
#include "resource.h"
#include <strsafe.h> //Use for handling buffer from overruns, such as converting date into char
#include <atlstr.h> //Use for handling image file using CString for text in HWND handles
#include "SalesInvoice.h" //Use the SalesInvoice class header to use those methods and constructors
#include "BigSpender.h" //Use the BigSpender class header to use those methods and constructors
#include "SpotPrize.h" //Use the SpotPrize class header to use those methods and constructors
#include <stdlib.h> //Using random number for SpotPrize, to select any products
#include <time.h> //Using date to write in a file for today date

#include <shellapi.h>

#include <gdiplus.h> //Using to display an image on this application
#pragma comment (lib, "gdiplus.lib") //Getting a library for Gdiplus
using namespace Gdiplus; //Avoid repeating Gdiplus typing in source code

using namespace std; //Avoid repeating std typing in source code

#define MAX_LOADSTRING 100

// Global Variables:
Product product = Product(); //Using the method from Product
Product* list[10]; //Set the list of 10 products
SalesInvoice sales = SalesInvoice(); //Using the method from SalesInvoice
BigSpender spend = BigSpender(); //Using the method from BigSpender
SpotPrize spot = SpotPrize(); //Using the method from SpotPrize
SalesInvoice* salesList[20]; //Set the list of 20 invoices from SalesInvoice, BigSpender or SpotPrize

//Window Handle from buttons, labels, edit boxes and list box
HWND sell, cPhone, customer, cName, name, phone, close, 
quantity, value, listbox, invoice, bSpend, discount, imageStatic, cancel;

//Set the iNumber(invoice) to 1 and listCount(number of list in salesList) and 
//stock(user's input for stock) to 0
int iNumber = 1, listCount = 0, stock = 0;

//Collect file's name and set as string for reading product and rewrite product
string pFile = "Products.txt";

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);


int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

	GdiplusStartupInput gdiPlusInput;
	ULONG_PTR		token;
	GdiplusStartup(&token, &gdiPlusInput, NULL);

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_BINGOOFFICE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_BINGOOFFICE));

    MSG msg;

    // Main message loop:
    while (GetMessage(&msg, nullptr, 0, 0))
    {
        if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }
    }

	GdiplusShutdown(token);
    return (int) msg.wParam;
}

//Display the retail price and image of this product
void showProduct(HWND hwnd)
{
	//Get the selected index from listbox
	int lbItem = (int)SendMessage(listbox, LB_GETCURSEL, 0, 0);

	//If the listbox was not selected then display error message
	if (lbItem > -1) 
	{
		//Else, then display the retail price and image of the selected product

		//Display retail price to static text for retail and convert as money format
		string price = convertCost(list[lbItem]->markUp());

		wstring item = wstring(price.begin(), price.end());

		LPCWSTR cwItem = item.c_str();

		HWND retail = CreateWindow(TEXT("STATIC"), _T(""), WS_CHILD | WS_VISIBLE,
			150, 350, 150, 20, hwnd, NULL, NULL, NULL);

		//Get image file's directory and set as string
		string imageFile = string() + "image/" + list[lbItem]->getImage();

		wstring wImage = wstring(imageFile.begin(), imageFile.end());

		LPCWSTR lImage = wImage.c_str();

		ShowWindow(imageStatic, SW_SHOW);

		//Draw the image to static from imageFile using Graphics and Image
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(imageStatic, &ps);

		//Set image as pointer to allocate new image
		Image *image = new Image(lImage, true);

		Graphics graphic(GetDC(imageStatic));
		graphic.DrawImage(image, 0, 0, 240, 210);

		//Show retail price from this static
		SetWindowText(retail, cwItem);

		//Make sell button enable
		EnableWindow(sell, true);

		//Delete image to avoid overloading image's data
		delete image;
	}

	else 
	{
		//Display message if list box is not selected
		::MessageBeep(MB_ICONASTERISK);
		MessageBox(hwnd, _T("You must select a product."), _T("Must Select Product!"), MB_OK);
	}
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_BINGOOFFICE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = (HBRUSH)(COLOR_WINDOW+1);
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_BINGOOFFICE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   HWND hWnd = CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
	   CW_USEDEFAULT, 0, 1000, 725, nullptr, nullptr, hInstance, nullptr);

   if (!hWnd)
   {
      return FALSE;
   }

   SetWindowPos(hWnd, NULL, 0, 0, 1000, 725, SWP_NOZORDER);

   //Read file from product file
   ifstream file(pFile, ios::in);
   //Set line for reading data per line
   string line;

   //If the system cannot find file then exit application
   if (!file.is_open())
   {
	   MessageBox(hWnd, _T("File is not existed."), _T("Unable to Find File"), MB_OK);
	   exit(0);
   }

   //Else, read file
   else
   {
	   int count = 0;

	   //Continue until the end of the file
	   while (!file.eof())
	   {
		   //Get file's current line
		   getline(file, line);
		   //Set split from line as stringstream
		   stringstream split(line);
		   //Set splits as string to get those character before the delimiting character
		   string splits;
		   //Collect five strings from beginning to end and avoid delimiting character
		   string *separate = new string[5];
		   //Set counting to 0 for increasing the list of characters per delimiting character
		   int counting = 0;

		   //Get those characters and expliciting commas
		   while (getline(split, splits, ','))
		   {
			   //Set those characters into the separate array and increment counting by 1
			   separate[counting] = splits;
			   counting++;
		   }

		   //Check if the code has 5 characters, name and image file less than or equal to 20 characters
		   if (separate[0].length() == 5 && separate[1].length() <= 20 && separate[4].length() <= 20) 
		   {
			   //Set new product from separate characters into required type of fields from product and increment count by 1
			   list[count] = new Product(separate[0], separate[1], atoi(separate[2].c_str()), atof(separate[3].c_str()), separate[4]);
			   count++;
		   }
		   //Delete separate by preventing allocated memory
		   delete[] separate;
	   }

   }

   for (int listCount = 0; listCount < 10; listCount++)
   {
	   //Show all products' name in a listbox
	   SendMessageA(listbox, LB_ADDSTRING, 0, (LPARAM)list[listCount]->getName().c_str());
   }

   //Add srand to avoid picking the same number from rand as this was using as an argument
   //This allows to pick any numbers without the system to random generate the same number
   srand(time(0));
   //Set the SpotPrize's product for anyone who picked this product and sell higher than $20
   spot.setSCode(list[rand() % 10]->getCode());

   file.close();

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {

		case WM_CREATE:
		{
			//Create all the buttons, listbox, statics and edit boxes at the start
			sell = CreateWindowEx(0, TEXT("BUTTON"), _T("Sell Product"),
				WS_CHILD | WS_VISIBLE,
				50, 500, 150, 25, hWnd, NULL, NULL, NULL);
			//Display list box and ensure the item selected will activate the event
			listbox = CreateWindowEx(0, TEXT("LISTBOX"), _T(""),
				WS_CHILD | WS_VISIBLE | WS_BORDER | LBS_NOTIFY,
				150, 100, 200, 200, hWnd, (HMENU) 1, NULL, NULL);
			close = CreateWindow(TEXT("BUTTON"), _T("Close of Business"),
				WS_CHILD | WS_VISIBLE,
				700, 275, 150, 25, hWnd, NULL, NULL, NULL);
			//Quantity box has limited to 3 characters by length
			quantity = CreateWindow(TEXT("EDIT"), _T(""),
				WS_CHILD | WS_VISIBLE | WS_BORDER,
				120, 400, 30, 20, hWnd, NULL, NULL, NULL);
			//Display invoice number
			invoice = CreateWindow(TEXT("STATIC"), _T(""), WS_CHILD | WS_VISIBLE,
				615, 350, 150, 20, hWnd, NULL, NULL, NULL);
			//Display image
			imageStatic = CreateWindow(TEXT("STATIC"), _T(""), WS_CHILD | WS_VISIBLE,
				500, 50, 240, 210, hWnd, NULL, NULL, NULL);

			//Set initial invoice number and display in static
			string iString = to_string(iNumber);

			wstring wIString = wstring(iString.begin(), iString.end());

			LPCWSTR iChar = wIString.c_str();

			SetWindowText(invoice, iChar);

			//Set focus on listbox
			SetFocus(listbox);

			EnableWindow(sell, false);

			ShowWindow(imageStatic, SW_HIDE);

		}
		break;

		case WM_COMMAND:
			{
				switch (LOWORD(wParam)) 
				{
					//Event happened when product was clicked from listbox
					case 1:
						if (HIWORD(wParam) == LBN_SELCHANGE) 
						{
							//Display this product's details
							showProduct(hWnd);
						}
						break;
					//Event happened when button was click
					case BN_CLICKED:
						//If click on sell, then proceed to this section
						if ((HWND)lParam == sell)
						{
							showProduct(hWnd);
							//Get integer from edit box 'quantity'
							TCHAR buff[4];
							//Get the length of the quantity's box
							GetWindowTextLength(quantity);
							//Get the text and put those characters in buff
							GetWindowText(quantity, buff, 4);
							//Change the character into integer
							stock = _tstoi(buff);

							//Retrieve data from selected item
							int lbItem = (int)SendMessage(listbox, LB_GETCURSEL, 0, 0);

							//If there are avaliable stocks left by user input, then do this section
							if (list[lbItem]->stockAvailable(stock)) 
							{
								//Get price from selected item's information from listbox and set it
								sales.setPrice(list[lbItem]->getCost());
								//Get quantity from edit box
								sales.setQuantity(stock);

								//Create a static for sales value
								value = CreateWindow(TEXT("STATIC"), _T(""), WS_CHILD | WS_VISIBLE,
									600, 400, 150, 20, hWnd, NULL, NULL, NULL);

								//Calculate the sales value from the price and quantity
								float total = sales.calValue();

								//Display the sales value as money format to static
								string values = convertCost(total);

								wstring sValue = wstring(values.begin(), values.end());

								LPCWSTR cValue = sValue.c_str();

								SetWindowText(value, cValue);

								//Get the product name using the selected index from listbox
								string productItem = list[lbItem]->getName();

								//If the total is greater than 0 or not invalid input, then do this section
								if (total > 0)
								{
									//Display Message Box to prompt user to confirm sale or cancel sale, if ok, then proceed
									::MessageBeep(MB_ICONASTERISK);
									if (::MessageBox(hWnd, _T("Confirm Sale?"), _T("Are you sure?"), MB_OKCANCEL) == IDOK)
									{
										//If the sales value over 20, display customer section
										if (total > 20)
										{
											cName = CreateWindow(TEXT("STATIC"), _T("Customer Name"), WS_CHILD | WS_VISIBLE,
												50, 575, 150, 20, hWnd, NULL, NULL, NULL);
											cPhone = CreateWindow(TEXT("STATIC"), _T("Customer Phone"), WS_CHILD | WS_VISIBLE,
												50, 600, 150, 20, hWnd, NULL, NULL, NULL);
											//Use name edit box for user to enter customer's name
											name = CreateWindow(TEXT("EDIT"), _T(""),
												WS_CHILD | WS_VISIBLE | WS_BORDER,
												175, 575, 130, 20, hWnd, NULL, NULL, NULL);
											//Use phone edit box for user to enter customer's phone
											phone = CreateWindow(TEXT("EDIT"), _T(""),
												WS_CHILD | WS_VISIBLE | WS_BORDER,
												175, 600, 90, 20, hWnd, NULL, NULL, NULL);
											//Display button for collecting customer's details or cancel sales
											customer = CreateWindow(TEXT("BUTTON"), _T("Save Info"),
												WS_CHILD | WS_VISIBLE,
												50, 650, 150, 25, hWnd, NULL, NULL, NULL);
											cancel = CreateWindow(TEXT("BUTTON"), _T("Cancel"),
												WS_CHILD | WS_VISIBLE,
												250, 650, 150, 25, hWnd, NULL, NULL, NULL);

											//Preventing user from changing quantity and product selection
											EnableWindow(sell, FALSE);
											EnableWindow(listbox, FALSE);
											EnableWindow(quantity, FALSE);
										
											//Set the stock and product's price to calculate the BigSpender's sales value
											spend.setQuantity(stock);
											spend.setPrice(list[lbItem]->getCost());
											float total = spend.calValue();

											//Display discount value
											bSpend = CreateWindow(TEXT("STATIC"), _T("After Discount: "), WS_CHILD | WS_VISIBLE,
												50, 625, 150, 20, hWnd, NULL, NULL, NULL);
											discount = CreateWindow(TEXT("STATIC"), _T(""), WS_CHILD | WS_VISIBLE,
												175, 625, 150, 20, hWnd, NULL, NULL, NULL);

											string dValues = convertCost(total);
											sValue = wstring(dValues.begin(), dValues.end());
											cValue = sValue.c_str();
											SetWindowText(discount, cValue);
										}

										//If sales value $20 or less, then add invoice number and product's details to SalesInvoice array
										else
										{
											salesList[listCount] = new SalesInvoice(iNumber, productItem, sales.getPrice(), stock, total);
											//Increment invoice number by 1 and display next invoice number
											iNumber++;
											string iString = to_string(iNumber);
											wstring wIString = wstring(iString.begin(), iString.end());
											LPCWSTR iChar = wIString.c_str();
											SetWindowText(value, _T(""));
											//Increment listCount by 1 to add new elements to SalesInvoice array
											listCount++;
											//Decrease the number of stock by user from the product's stock
											list[lbItem]->makeSale(stock);
											//If invoice number has reached to 21, prevent user from adding more sales
											if (iNumber >= 21)
											{
												MessageBox(hWnd, _T("This is the final sale."), _T("Maximum Sales"), MB_OK);
												ShowWindow(sell, SW_HIDE);
												SetWindowText(invoice, _T("20 INVOICES MAX"));
											}

											//Else, display the next invoice number
											else 
											{
												SetWindowText(invoice, iChar);
											}
										}
									}


								}

								//If quantity edit box is less than 0 or invalid input, display message box
								else
								{
									::MessageBeep(MB_ICONASTERISK);
									MessageBoxA(hWnd, "Quantity must be filled in as integer and greater than 0.", "Invalid Input.", MB_OK);
								}
							}

							//If quantity is greater than product's stock, then display unavaliable message box
							else 
							{
								string lStock = string() + "There are only " + to_string(list[lbItem]->getStock()) + " stocks left.";
								::MessageBeep(MB_ICONASTERISK);
								MessageBoxA(hWnd, lStock.c_str(), "Unavaliable Number of Stocks", MB_OK);
							}
						
						}

						//If user clicks on 'customer' button
						else if ((HWND)lParam == customer) 
						{
							//Get the selected index
							int lbItem = (int)SendMessage(listbox, LB_GETCURSEL, 0, 0);

							//Get the length of the name and phone entered in the edit box for customer
							int lengthName = SendMessage(name, WM_GETTEXTLENGTH, 0, 0);
							int lengthPhone = SendMessage(phone, WM_GETTEXTLENGTH, 0, 0);

							//If both are not empty, then proceed, else display warning message box
							if (lengthName != 0)
							{
								if (lengthPhone != 0) 
								{
									//Get names up to 16, but makes name 15 characters maximum to avoid ambiguity from TCHAR
									TCHAR names[16];
									GetWindowTextLength(name);
									GetWindowText(name, names, 16);
									wstring nameString = names;
									string cuName = string(nameString.begin(), nameString.end());

									//Get phones up to 11, but same as name up to 10 characters
									TCHAR phones[11];
									GetWindowTextLength(phone);
									GetWindowText(phone, phones, 11);
									wstring phoneString = phones;
									string cuPhone = string(phoneString.begin(), phoneString.end());

									//Check through every characters in phone to ensure all are digits
									int invalid = 0;

									for (int counting = 0; counting < cuPhone.size(); counting++)
									{
										if (!isdigit(cuPhone[counting]))
										{
											invalid++;
										}
									}

									//If all are digits, proceed, else display warning message
									if (invalid == 0)
									{
										//When both name and phone are validated, check if the customer's details are correct
										::MessageBeep(MB_ICONASTERISK);
										if (::MessageBox(hWnd, _T("Confirm Customer?"), _T("Are you sure?"), MB_OKCANCEL) == IDOK)
										{
											//If the user has selected the Product of the Day, then do this section
											if (list[lbItem]->getCode() == spot.getSCode())
											{
												//Display user that this product was the spot prize
												MessageBox(hWnd, _T("You bought the product of the day"), _T("Spot Prize"), MB_OK);
												TCHAR buff[4];
												GetWindowTextLength(quantity);
												GetWindowText(quantity, buff, 4);
												stock = _tstoi(buff);

												//Calculate new sales value from Spot Prize class
												spot.setQuantity(stock);
												spot.setPrice(list[lbItem]->getCost());
												float total = spot.calValue();

												stringstream messLine;

												//Display the discount when selling the spot prize.
												messLine << "Your new discount total is " << convertCost(total) << ".";

												MessageBoxA(hWnd, messLine.str().c_str(), "Spot Prize", MB_OK);

												//Add SpotPrize's code, BigSpender's details and SalesInvoice's details to SalesInvoice array
												salesList[listCount] = new SpotPrize(spot.getSCode(), iNumber, list[lbItem]->getName(), list[lbItem]->markUp(), stock, total, cuName, cuPhone);
											}

											//If sales value greater than 20 but not spot prize
											else
											{
												//Calculate the sales value using BigSpender methods
												TCHAR buff[4];
												GetWindowTextLength(quantity);
												GetWindowText(quantity, buff, 4);
												int stock = _tstoi(buff);

												spend.setQuantity(stock);
												spend.setPrice(list[lbItem]->getCost());
												float total = spend.calValue();

												//Add the BigSpender's details and SalesInvoice's details to SalesInvoice array
												salesList[listCount] = new BigSpender(iNumber, list[lbItem]->getName(), list[lbItem]->markUp(), stock, total, cuName, cuPhone);
											}

											//When finished, listbox, quantity and sell are enabled
											EnableWindow(listbox, TRUE);
											EnableWindow(quantity, TRUE);
											EnableWindow(sell, TRUE);
											//All customer's section are hidden
											ShowWindow(cName, SW_HIDE);
											ShowWindow(name, SW_HIDE);
											ShowWindow(cPhone, SW_HIDE);
											ShowWindow(phone, SW_HIDE);
											ShowWindow(customer, SW_HIDE);
											ShowWindow(bSpend, SW_HIDE);
											ShowWindow(discount, SW_HIDE);
											ShowWindow(cancel, SW_HIDE);
											//Make the sales value's text and quantity's text empty
											SetWindowText(quantity, _T(""));
											SetWindowText(value, _T(""));

											//Increase the invoice number by 1 and decrease the product's stock by the user's stock
											iNumber++;
											string iString = to_string(iNumber);
											wstring wIString = wstring(iString.begin(), iString.end());
											LPCWSTR iChar = wIString.c_str();

											listCount++;
											list[lbItem]->makeSale(stock);

											//If invoice number is 21 or greater, stop user from proceeding
											if (iNumber >= 21)
											{
												MessageBox(hWnd, _T("This is the final sale."), _T("Maximum Sales"), MB_OK);
												ShowWindow(sell, SW_HIDE);
											}

											//Else, display the next invoice number
											else
											{
												SetWindowText(invoice, iChar);
											}

										}
									}

									//If all characters from phone are not digits, display error
									else
									{
										::MessageBeep(MB_ICONASTERISK);
										MessageBoxA(hWnd, "Phone must be in digits.", "Invalid Phone", MB_OK);
									}
								}

								//If phone is empty, display error
								else 
								{
									::MessageBeep(MB_ICONASTERISK);
									MessageBoxA(hWnd, "Phone should not be empty.", "Empty Phone", MB_OK);
								}
							}

							//If name is empty, display error
							else
							{
								::MessageBeep(MB_ICONASTERISK);
								MessageBoxA(hWnd, "Name should not be empty.", "Empty Name", MB_OK);
							}
						}

						//If the user clicks, 'Close business' button, do this section
						else if ((HWND)lParam == close) 
						{
							::MessageBeep(MB_ICONASTERISK);
							if (::MessageBox(hWnd, _T("Is it time to close business?"), _T("Save All Invoice"), MB_OKCANCEL) == IDOK)
							{
								//Add date
								time_t rawtime;
								char buffer[80];

								time(&rawtime);

								//Format date to set name in SalesInvoice in day/month/year
								strftime(buffer, 80, "%d/%m/%Y", localtime(&rawtime));
							
								//Add total sale for date to SalesInvoice object to write last line
								string date = string() + "Total sales for " + buffer;

								//Decrease Invoice Number to get the total number of invoice
								iNumber--;
								//Set the invoice number and date to SalesInvoice object
								sales.setNumber(iNumber);
								sales.setName(date);
								//Initialize total cost, sales value and quantity to 0
								float tCost = 0, tSale = 0;
								int tQuantity = 0;
								//Go through every elements in SalesInvoice array
								for (int counting = 0; counting < 20; counting++) 
								{
									//Add the total cost, sales value and quantity from the array element
									//If the array element is not empty
									if (salesList[counting] != nullptr) 
									{
										tCost += salesList[counting]->getPrice();
										tQuantity += salesList[counting]->getQuantity();
										tSale += salesList[counting]->getValue();
									}
								}

								//Set total cost, quantity and sales value to SalesInvoice object
								sales.setPrice(tCost);
								sales.setQuantity(tQuantity);
								sales.setValue(tSale);

								//Format this date for saving file as Year-Month-Day
								strftime(buffer, 80, "%F", localtime(&rawtime));

								//Write all SalesInvoice array and object in this sales file
								string fileName;
								fileName = string() + buffer + " sales.txt";

								ofstream saleFile(fileName, ios::out);

								//Write all of the elements in SalesInvoice array into file until the current element is null
								for (int counting = 0; counting < 20; counting++)
								{
									if (salesList[counting] != NULL)
									{
										saleFile << salesList[counting]->printSale() << endl;
									}
								}

								//Write SalesInvoice object into Sales file
								saleFile << sales.printSale();

								//Close Sales file
								saleFile.close();

								//Write the renewal product array and overwrite the product file
								ofstream prodFile(pFile, ios::out);

								for (int counting = 0; counting < 10; counting++)
								{
									prodFile << list[counting]->printProduct();
									if (counting < 9)
									{
										prodFile << endl;
									}
								}

								//Open Sales file using shell execute
								wstring wIString = wstring(fileName.begin(), fileName.end());
								LPCWSTR fChar = wIString.c_str();
								ShellExecute(NULL, NULL, fChar, NULL, NULL, SW_SHOW);

								//Open Product file using shell execute
								wIString = wstring(pFile.begin(), pFile.end());
								fChar = wIString.c_str();
								ShellExecute(NULL, NULL, fChar, NULL, NULL, SW_SHOW);

								//Exit application
								exit(0);
							}

						}

						//At the customer section, if press cancel, then remove customer section
						else if((HWND)lParam == cancel)
						{
							EnableWindow(listbox, TRUE);
							EnableWindow(quantity, TRUE);
							EnableWindow(sell, TRUE);
							ShowWindow(cName, SW_HIDE);
							ShowWindow(name, SW_HIDE);
							ShowWindow(cPhone, SW_HIDE);
							ShowWindow(phone, SW_HIDE);
							ShowWindow(customer, SW_HIDE);
							ShowWindow(bSpend, SW_HIDE);
							ShowWindow(discount, SW_HIDE);
							ShowWindow(cancel, SW_HIDE);
						}
					break;
				}
			}
			break;

		case WM_PAINT:
		{
			//Write all default static texts
			PAINTSTRUCT ps;
			HDC hdc = BeginPaint(hWnd, &ps);
			// TODO: Add any drawing code that uses hdc here...
			TextOut(hdc, 50, 42, _T("Bingo Office Supplies Sales System"), 35);
			TextOut(hdc, 50, 100, _T("Product List: "), 15);
			TextOut(hdc, 50, 350, _T("Retail Price: "), 15);
			TextOut(hdc, 500, 350, _T("Invoice Number: "), 17);
			TextOut(hdc, 50, 400, _T("Quantity: "), 11);
			TextOut(hdc, 500, 400, _T("Value Of Sale: "), 16);

			EndPaint(hWnd, &ps);
		}
			break;

		case WM_DESTROY:
			PostQuitMessage(0);
			break;

		case WM_CLOSE:
			//If closing application by clicking close button from application, check before closing to ensure that all data will not be saved
			::MessageBeep(MB_ICONASTERISK);
			if (MessageBox(hWnd, _T("Are you sure to quit?"), _T("Exit Without Saving"), MB_YESNO) == IDYES) 
			{
				exit(0);
			}
			break;

		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}

