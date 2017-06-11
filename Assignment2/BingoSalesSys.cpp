//Programmer : Yujia Yuan
//Date : 2017-06-09
//Sale application for ¡°Bingo Office Supplies¡± 

// BingoOffice.cpp : Defines the entry point for the application.
//
#include "stdafx.h" //Precompiled Header 
#include <fstream> //Input/output stream class to operate on files.
#include <string> //This header introduces string types, character traits and a set of converting functions.
#include <iostream> //Defines the standard input/output stream objects
#include <sstream> //Header providing string stream classes
#include <stdlib.h> //Using random number for SpotPrize, to select any products
#include <time.h> //Using date to write in a file for today date
#include <shellapi.h> //For Launching Notepad Applications 
#include <strsafe.h> //Provide additional processing for proper buffer handling in the code.
#include <atlstr.h> //Use for handling image file using CString for text in HWND handles
#include<regex>// For Regex usage
//Load the class header to use those methods and constructors
#include "BingoOffice.h" 
#include "resource.h"
#include "Product.h"
#include "SalesInvoice.h" 
#include "BigSpender.h" 
#include "SpotPrize.h" 

#include <gdiplus.h> //Enables applications to use graphics and formatted text
#pragma comment (lib, "gdiplus.lib") //Getting a library for Gdiplus

using namespace Gdiplus; //Avoid repeating Gdiplus typing in source code
using namespace std; //Avoid repeating std typing in source code

#define MAX_LOADSTRING 100 //Limit the string length

// Set global variables:
Product product = Product(); //Use the method from Product
Product* productStockList[10]; //Set a list of 10 products
SalesInvoice salesInvoice = SalesInvoice(); //Using the method from SalesInvoice
BigSpender bigSpender = BigSpender(); //Using the method from BigSpender
SpotPrize spotPrize = SpotPrize(); //Using the method from SpotPrize
SalesInvoice* salesInvoiceList[20]; //Set the list of 20 invoices from SalesInvoice, BigSpender or SpotPrize
string spotCodeTemp; //Temp variable for saving Spot Prize Code
float originTotal; //For reuse of origin total price

//Check to ensure phone input is digit
regex myRegex("[[:d:]]+");

//Window Handle for buttons, labels, edit boxes and list box
HWND hWnd,
systemName, imageShow,
productListText, productList,
retailPriceText, retailPriceShow,
quantityText, quantityInput,
totalValueText, totalValueShow,
discountedPriceText, discountedPriceShow,
youSaveText, youSaveShow,
calTotalBtn, sellProductBtn,
customNameText, customPhoneText,
customNameInput, customPhoneInput,
saveCustomInfoBtn, cancelCustomInfoBtn,
invoiceNumText, invoiceNumShow,
saveTodayRecord, closeBusinessBtn,
setSpotText, setSpotShow, setSpotBtn,
hwndTotalPrice;

//Set the iNumber(invoice number) to 1 and listCount(number of list in salesInvoiceList) and 
//stock(user's input for stock) to 0
int iNumber = 1, listCount = 0, stock = 0;

//Set the source file's name and set as string for reading and rewrite 
string productFileName = "Products.txt";

//HINSTANCE hInst;                                // current instance
//WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
//WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

static TCHAR szAppName[MAX_LOADSTRING] = TEXT("Bingo Office Supplies");
static int iniXSize = 800;
static int iniYSize = 600;


// Forward declarations of functions included in this code module:
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
 

// For the application entry point
int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow)
{
	//Contains message information from a thread's message queue
	MSG      msg;
	//Contains the window class attributes
	WNDCLASS wndclass;

	//For image displaying
	GdiplusStartupInput gdiPlusInput;
	ULONG_PTR		token;
	GdiplusStartup(&token, &gdiPlusInput, NULL);


	//Read file from product file
	ifstream productSourceFile(productFileName, ios::in);
	//Set line buffer for reading data line by line
	string readFilebuffer;

	//If the system cannot find nominated file 
	if (!productSourceFile.is_open())
	{
		MessageBox(hWnd, TEXT("Products Source File does not exist!"), TEXT("Unable to Find File"), MB_OK);
		exit(0);
	}

	//Read from file
	else
	{
		int count = 0;

		//Loop until the end of the file
		while (!productSourceFile.eof())
		{
			//Get the current line data
			getline(productSourceFile, readFilebuffer);
			//Set split from line using stringstream function
			stringstream sFileBuffer(readFilebuffer);
			//Set splits as string to get those character before the delimiting character
			string splits;
			//Collect five strings from the beginning to the end skipping delimiting character
			string *separate = new string[5];
			//Set counting to 0 for increasing the list of characters 
			int counting = 0;

			//Get those characters divided by commas
			while (getline(sFileBuffer, splits, ','))
			{
				//Set those characters into the separate array and increase counting by 1
				separate[counting] = splits;
				counting++;
			}

			//Check if the code has 5 characters, name and image file less than or equal to 20 characters
			if (separate[0].length() == 5 && separate[1].length() <= 20 && separate[4].length() <= 20)
			{
				//Set new product from separate characters into required type of fields from product 
				productStockList[count] = new Product(separate[0], separate[1], atoi(separate[2].c_str()), atof(separate[3].c_str()), separate[4]);
				count++;
			}
			//Delete separate by preventing memory leak
			delete[] separate;
		}

	}
	//Remenber to .close read file.
	productSourceFile.close();
	
	//Define windows style
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szAppName;

	if (!RegisterClass(&wndclass))
	{
		MessageBox(NULL, TEXT("Windows Register Fails!"), szAppName, MB_ICONERROR);
		return 0;
	}

	// Creates an overlapped window.
	hWnd = CreateWindow(
		szAppName,                  // window class name
		TEXT("Bingo Office Supplies Sales System"),      // window Title
		WS_OVERLAPPEDWINDOW,        // window style
		CW_USEDEFAULT,              // initial x position
		CW_USEDEFAULT,              // initial y position
		iniXSize,              // initial x size
		iniYSize,              // initial y size
		NULL,                       // parent window handle
		NULL,                       // window menu handle
		hInstance,                // program instance handle
		NULL);                     // creation parameters


	ShowWindow(hWnd, iCmdShow);
	//The UpdateWindow function updates the client area of the specified window by sending a WM_PAINT message to the window
	UpdateWindow(hWnd);



	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

//Display the retail price and image of selected product
void showProduct(HWND hwnd)
{
	//Get the selected index from listbox
	int itemIndex = (int)SendMessage(productList, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

	//If the listbox was not selected then display error message
	if (itemIndex > -1)
	{
		//Display retail price to static text
		string price = convertCost(productStockList[itemIndex]->markUp());
		wstring item = wstring(price.begin(), price.end());
		LPCWSTR cwItem = item.c_str();

		//Get image file's directory and set as string
		string imageFile = string() + "image/" + productStockList[itemIndex]->getImage();
		wstring wImage = wstring(imageFile.begin(), imageFile.end());
		LPCWSTR lImage = wImage.c_str();
		//Show product image
		ShowWindow(imageShow, SW_SHOW);

		//Draw the image to static from imageFile using Graphics and Image
		PAINTSTRUCT ps;
		HDC hdc = BeginPaint(imageShow, &ps);

		//Set image as pointer to allocate new image
		Image *image = new Image(lImage, true);

		Graphics graphic(GetDC(imageShow));
		graphic.DrawImage(image, 0, 0, 240, 210);

		//Show retail price from this static
		SetWindowText(retailPriceShow, cwItem);

		//Show the select Spot Product
		string sSelectSpot = productStockList[itemIndex]->getName();
		wstring wSelectSpot = wstring(sSelectSpot.begin(), sSelectSpot.end());
		LPCWSTR lSelectSpot = wSelectSpot.c_str();
		//Save the code to be reused
		spotCodeTemp = productStockList[itemIndex]->getCode();;

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC14), lSelectSpot);
		//Make sell button enable
		EnableWindow(sellProductBtn, true);

		//Delete image 
		delete image;
	}

	else
	{
		//Display message if list box is not selected
		MessageBox(hWnd, TEXT("Please select a product first"), TEXT("Product Not Selected"), MB_OK);
	}
}

//processes messages sent to main window
static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{


	switch (message)
	{
	case WM_CREATE:
	{


		//Create App name window
		systemName = CreateWindow(
			TEXT("STATIC"),
			TEXT("Bingo Office Supplies Sales System"),
			WS_CHILD | WS_VISIBLE,
			iniXSize / 2 - 100, 10, 236, 22,
			hWnd,
			(HMENU)IDC_STATIC1,
			NULL, NULL);
		//Create Product List Text: 
		productListText = CreateWindow(TEXT("STATIC"),
			TEXT("Products List:"),
			WS_CHILD | WS_VISIBLE,
			55, 50, 90, 22,
			hWnd,
			(HMENU)IDC_STATIC2,
			NULL, NULL);

		// Create product list:
		productList = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			TEXT("LISTBOX"),
			NULL,
			WS_CHILD | WS_VISIBLE | LBS_HASSTRINGS | LBS_NOTIFY,
			10, 80, 220, 170,
			hWnd,
			(HMENU)IDC_LISTBOX1,
			NULL, NULL);
		for (int listCount = 0; listCount < 10; listCount++)
		{
			//Show all products' name in a listbox
			SendMessageA(productList, LB_ADDSTRING, 0, (LPARAM)productStockList[listCount]->getName().c_str());
		}
		
		//Create Retail Price:
		retailPriceText = CreateWindow(TEXT("STATIC"),
			TEXT("Retail Price: "),
			WS_CHILD | WS_VISIBLE,
			15, 280, 110, 20,
			hWnd,
			(HMENU)IDC_STATIC3,
			NULL, NULL);
		//Create Product Price Display window:
		retailPriceShow = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE,
			145, 280, 90, 20,
			hWnd,
			(HMENU)IDC_STATIC4,
			NULL, NULL);
		//Create Quantity window:
		quantityText = CreateWindow(TEXT("STATIC"),
			TEXT("      Quantity:"),
			WS_CHILD | WS_VISIBLE,
			15, 310, 110, 20,
			hWnd,
			(HMENU)IDC_STATIC5,
			NULL, NULL);
		//Create Editing Quantity window:
		quantityInput = CreateWindowEx(
			WS_EX_CLIENTEDGE,
			TEXT("Edit"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE,
			145, 310, 25, 20,
			hWnd,
			(HMENU)IDC_EDIT1,
			NULL, NULL);
		//Create Total Value window:
		totalValueText = CreateWindow(TEXT("STATIC"),
			TEXT("Total Value: "),
			WS_CHILD | WS_VISIBLE,
			15, 340, 110, 20,
			hWnd,
			(HMENU)IDC_STATIC5,
			NULL, NULL);
		//Create Total Value display window:
		totalValueShow = CreateWindow(TEXT("STATIC"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE,
			145, 340, 110, 20,
			hWnd,
			(HMENU)IDC_STATIC6,
			NULL, NULL);
		//Create Total price caculate button
		calTotalBtn = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Calculate Total"),
			WS_CHILD | WS_VISIBLE,
			15, 370, 120, 20, hWnd,
			(HMENU)IDC_BUTTON1,
			NULL, NULL);
		//Create selling button
		sellProductBtn = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Sell Product"),
			WS_CHILD | WS_VISIBLE,
			155, 370, 100, 20, hWnd,
			(HMENU)IDC_BUTTON2,
			NULL, NULL);
		//Create image show window
		imageShow = CreateWindow(
			TEXT("STATIC"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE,
			iniXSize / 2 - 100, 80, 240, 240,
			hWnd,
			(HMENU)IDC_PICTURE1,
			NULL, NULL);
		//Create Close Business Button
		closeBusinessBtn = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Close of Business"),
			WS_CHILD | WS_VISIBLE,
			275, 460, 220, 40,
			hWnd,
			(HMENU)IDC_BUTTON5,
			NULL, NULL);
		//Create invoice number text window
		invoiceNumText = CreateWindow(
			TEXT("STATIC"),
			TEXT("Invoice Number:"),
			WS_CHILD | WS_VISIBLE,
			275, 340, 115, 20,
			hWnd, 
			(HMENU)IDC_STATIC3, NULL, NULL);
		//Create invoice number show window
		invoiceNumShow = CreateWindow(
			TEXT("STATIC"), TEXT(""),
			WS_CHILD | WS_VISIBLE,
			400, 340, 150, 20,
			hWnd, NULL, NULL, NULL);
		////Create Spot Prize text show window
		setSpotText = CreateWindow(
			TEXT("STATIC"),
			TEXT("Select Spot Prize:"),
			WS_CHILD | WS_VISIBLE,
			550, 60, 150, 22,
			hWnd, 
			(HMENU)IDC_STATIC3, NULL, NULL);
		//Create Spot Prize name show window
		setSpotShow = CreateWindow(
			TEXT("STATIC"), TEXT(""),
			WS_CHILD | WS_VISIBLE,
			550, 90, 150, 20 ,
			hWnd, (HMENU)IDC_STATIC14, NULL, NULL);
		//Create Spot Prize Setting Button
		setSpotBtn = CreateWindow(
			TEXT("BUTTON"), TEXT("Set As SpotPrize"),
			WS_CHILD | WS_VISIBLE,
			575, 120, 120, 30,
			hWnd, 
			(HMENU)IDC_BUTTON6,
			NULL, NULL);

		//Set initial invoice number and display
		string iString = to_string(iNumber);
		wstring wIString = wstring(iString.begin(), iString.end());
		LPCWSTR iChar = wIString.c_str();
		SetWindowText(invoiceNumShow, iChar);

		//Set focus on listbox
		SetFocus(productList);
		ShowWindow(imageShow, SW_HIDE);
		EnableWindow(sellProductBtn, false);
	}
	break;

	//Set Text & background color
	case WM_CTLCOLORSTATIC:
	{
		HDC hdc = (HDC)wParam;
		HWND hwnd = (HWND)lParam;

		if (GetDlgCtrlID(hwnd) == IDC_STATIC1|| GetDlgCtrlID(hwnd) == IDC_STATIC3 || 
			GetDlgCtrlID(hwnd) == IDC_STATIC2 || GetDlgCtrlID(hwnd) == IDC_STATIC5)
		{
			SetTextColor(hdc, RGB(112, 129, 219));
			SetBkColor(hdc, RGB(100, 253, 72));
			return (LRESULT)CreateSolidBrush(RGB(100, 253, 72));
		}
	}
	break;



	case WM_COMMAND:
	{
		switch (LOWORD(wParam))
		{
		case IDC_LISTBOX1:
		{
			if (HIWORD(wParam) == LBN_SELCHANGE)
			{
				//Display this product's details and empty total:
				showProduct(hWnd);
				//Clear Total Value if it exist.
				SetWindowText(GetDlgItem(hWnd, IDC_STATIC6), TEXT(""));

			}

		}
		break;

		//Calculate the total price:
		case IDC_BUTTON1:
		{
			showProduct(hWnd);
			//Get integer from edit box 'quantity'
			TCHAR buff[4];
			//Get the length of the quantity's input box
			GetWindowTextLength(quantityInput);
			//Get the text and put those characters in buff
			GetWindowText(quantityInput, buff, 4);
			//Change the character into integer
			stock = _tstoi(buff);

			//Retrieve data from ListBox for selected item
			int itemIndex = (int)SendMessage(productList, LB_GETCURSEL, 0, 0);
			string productName = productStockList[itemIndex]->getName();
			//If there are avaliable stocks left by user input, then do this section
			if (productStockList[itemIndex]->stockAvailable(stock))
			{
				//Get price from selected item's information from listbox and set it
				salesInvoice.setPrice(productStockList[itemIndex]->getCost());
				//Get quantity from edit box
				salesInvoice.setQuantity(stock);

				//Calculate the sales value from the price and quantity
				originTotal = salesInvoice.calValue();

				//Display the sales value as money format to static
				string Totalvalues = convertCost(originTotal);
				wstring sTotalValue = wstring(Totalvalues.begin(), Totalvalues.end());
				LPCWSTR cTotalValue = sTotalValue.c_str();

				hwndTotalPrice = GetDlgItem(hWnd, IDC_STATIC6);
				SetWindowText(totalValueShow, cTotalValue);

				if (originTotal > 20)
				{
					//Set the stock and product's price to calculate the BigSpender's sales value
					bigSpender.setQuantity(stock);
					bigSpender.setPrice(productStockList[itemIndex]->getCost());
					float dicountedTotal = bigSpender.calValue();

					//Display discount value
					discountedPriceText = CreateWindow(TEXT("STATIC"), TEXT("After Discount: "), WS_CHILD | WS_VISIBLE,
						15, 400, 110, 20, hWnd, (HMENU)IDC_STATIC9, NULL, NULL);
					discountedPriceShow = CreateWindow(TEXT("STATIC"), TEXT(""), WS_CHILD | WS_VISIBLE,
						155, 400, 90, 20, hWnd, (HMENU)IDC_STATIC10, NULL, NULL);

					youSaveText = CreateWindow(TEXT("STATIC"), TEXT("You've saved: "), WS_CHILD | WS_VISIBLE,
						15, 430, 110, 20, hWnd, (HMENU)IDC_STATIC11, NULL, NULL);
					youSaveShow = CreateWindow(TEXT("STATIC"), TEXT(""), WS_CHILD | WS_VISIBLE,
						155, 430, 90, 20, hWnd, (HMENU)IDC_STATIC12, NULL, NULL);
					
					//Caculate new totol prize and saved money
					string dDiscountedValues = convertCost(dicountedTotal);
					wstring sDiscountedValue = wstring(dDiscountedValues.begin(), dDiscountedValues.end());
					LPCWSTR cDiscountedValue = sDiscountedValue.c_str();
					SetWindowText(discountedPriceShow, cDiscountedValue);
					float fSavedMoney = originTotal - dicountedTotal;
					string dSavedMoney = convertCost(fSavedMoney);
					wstring sSavedMoney = wstring(dSavedMoney.begin(), dSavedMoney.end());
					LPCWSTR cSavedMoney = sSavedMoney.c_str();
					SetWindowText(youSaveShow, cSavedMoney);
					
				}
				
			}
			//If quantity is greater than product's stock, then display unavaliable message box
			else
			{
				string lStock = string() + "There are only " + to_string(productStockList[itemIndex]->getStock()) + " " + productName + " left.";
				
				MessageBoxA(hWnd, lStock.c_str(), "Not Enough Stocks", MB_OK);
			}
		}
		break;

		//Sell out the products:
		case IDC_BUTTON2:
		{

			showProduct(hWnd);
			//Get integer from edit box 'quantity'
			TCHAR buff[4];
			//Get the length of the quantity's box
			GetWindowTextLength(quantityInput);
			//Get the text and put those characters in buff
			GetWindowText(quantityInput, buff, 4);
			//Change the character into integer
			stock = _tstoi(buff);

			//Get index from selected item
			int itemIndex = (int)SendMessage(productList, LB_GETCURSEL, 0, 0);
			string productName = productStockList[itemIndex]->getName();

			//Check if there are avaliable stocks
			if (productStockList[itemIndex]->stockAvailable(stock))
			{
				//Get price from selected item's information from listbox and set it
				salesInvoice.setPrice(productStockList[itemIndex]->getCost());
				//Get quantity from edit box
				salesInvoice.setQuantity(stock);

				//Calculate the sales value from the price and quantity
				originTotal = salesInvoice.calValue();

				//Display the sales value as money format to static
				string Totalvalues = convertCost(originTotal);
				wstring sTotalValue = wstring(Totalvalues.begin(), Totalvalues.end());
				LPCWSTR cTotalValue = sTotalValue.c_str();

				SetWindowText(totalValueShow, cTotalValue);

				if (originTotal > 0)
				{
					//Display Message Box to confirm sale or net, if ok, then proceed
					if (::MessageBox(hWnd, TEXT("Confirm Sale?"), TEXT("Are you sure?"), MB_OKCANCEL) == IDOK)
					{
						//If the sales value over 20, display customer info record box
						if (originTotal > 20)
						{
							//Create info windows
							customNameText = CreateWindow(TEXT("STATIC"), TEXT("Customer Name:"), WS_CHILD | WS_VISIBLE,
								275, 370, 115, 20, hWnd, NULL, NULL, NULL);
							customPhoneText = CreateWindow(TEXT("STATIC"), TEXT("Customer Phone:"), WS_CHILD | WS_VISIBLE,
								275, 400, 115, 20, hWnd, NULL, NULL, NULL);
							//Use name edit box for user to enter customer's name
							customNameInput = CreateWindow(TEXT("EDIT"), TEXT(""),
								WS_CHILD | WS_VISIBLE | WS_BORDER,
								400, 370, 180, 20, hWnd, NULL, NULL, NULL);
							//Use phone edit box for user to enter customer's phone
							customPhoneInput = CreateWindow(TEXT("EDIT"), TEXT(""),
								WS_CHILD | WS_VISIBLE | WS_BORDER,
								400, 400, 180, 20, hWnd, NULL, NULL, NULL);
							//Display button for collecting customer's details or cancel sales
							saveCustomInfoBtn = CreateWindow(TEXT("BUTTON"), TEXT("Save Info"),
								WS_CHILD | WS_VISIBLE,
								275, 430, 110, 20, hWnd,
								(HMENU)IDC_BUTTON3, NULL, NULL);
							cancelCustomInfoBtn = CreateWindow(TEXT("BUTTON"), TEXT("Cancel"),
								WS_CHILD | WS_VISIBLE,
								400, 430, 110, 20, hWnd,
								(HMENU)IDC_BUTTON4, NULL, NULL);


							//Preventing user from changing quantity and product selection
							EnableWindow(sellProductBtn, FALSE);
							EnableWindow(productList, FALSE);
							EnableWindow(quantityInput, FALSE);

							//Set the stock and product's price to calculate the BigSpender's sales value
							bigSpender.setQuantity(stock);
							bigSpender.setPrice(productStockList[itemIndex]->getCost());
							float dicountedTotal = bigSpender.calValue();

							//Display discount value
							discountedPriceText = CreateWindow(TEXT("STATIC"), TEXT("After Discount: "), WS_CHILD | WS_VISIBLE,
								15, 400, 110, 20, hWnd, (HMENU)IDC_STATIC9, NULL, NULL);
							discountedPriceShow = CreateWindow(TEXT("STATIC"), TEXT(""), WS_CHILD | WS_VISIBLE,
								155, 400, 90, 20, hWnd, (HMENU)IDC_STATIC10, NULL, NULL);

							youSaveText = CreateWindow(TEXT("STATIC"), TEXT("You've saved: "), WS_CHILD | WS_VISIBLE,
								15, 430, 110, 20, hWnd, (HMENU)IDC_STATIC11, NULL, NULL);
							youSaveShow = CreateWindow(TEXT("STATIC"), TEXT(""), WS_CHILD | WS_VISIBLE,
								155, 430, 90, 20, hWnd, (HMENU)IDC_STATIC12, NULL, NULL);


							string dDiscountedValues = convertCost(dicountedTotal);
							wstring sDiscountedValue = wstring(dDiscountedValues.begin(), dDiscountedValues.end());
							LPCWSTR cDiscountedValue = sDiscountedValue.c_str();
							SetWindowText(discountedPriceShow, cDiscountedValue);
							float fSavedMoney = originTotal - dicountedTotal;
							string dSavedMoney = convertCost(fSavedMoney);
							wstring sSavedMoney = wstring(dSavedMoney.begin(), dSavedMoney.end());
							LPCWSTR cSavedMoney = sSavedMoney.c_str();
							SetWindowText(youSaveShow, cSavedMoney);


						}

						//If sales value $20 or less, then add invoice number and product's details to SalesInvoice array
						else
						{
							salesInvoiceList[listCount] = new SalesInvoice(iNumber, productName, salesInvoice.getPrice(), stock, originTotal);
							//Increment invoice number by 1 and display next  number
							iNumber++;
							string iString = to_string(iNumber);
							wstring wIString = wstring(iString.begin(), iString.end());
							LPCWSTR iChar = wIString.c_str();

							//Increment listCount by 1 to add new elements to SalesInvoice array
							listCount++;
							//Decrease the saled number from the product's stock
							productStockList[itemIndex]->makeSale(stock);

							//If invoice number has reached 21, prevent user from adding more sales
							if (iNumber > 20)
							{
								MessageBox(hWnd, TEXT("This is the final sale."), TEXT("Maximum Sales"), MB_OK);
								ShowWindow(sellProductBtn, SW_HIDE);
								SetWindowText(invoiceNumShow, TEXT("20 INVOICES MAX"));
							}

							//Else, display the next invoice number
							else
							{
								SetWindowText(invoiceNumShow, iChar);
							}
						}
					}
				}
				//If quantity edit box is less than 0 or invalid input, display message box
				else
				{
					MessageBoxA(hWnd, "Quantity Must Be an integer greater than 0.", "Invalid Input", MB_OK);
				}

			}
			//If quantity is greater than product's stock, then display stock unavaliable message box
			else
			{
				string lStock = string() + "There are only " + to_string(productStockList[itemIndex]->getStock()) + " " + productName + " left.";
				MessageBoxA(hWnd, lStock.c_str(), "Not Enough Stocks", MB_OK);
			}


		}
		break;

		//If save cumtom info:
		case IDC_BUTTON3:
		{

			//Get the selected index
			int ItemIndex = (int)SendMessage(productList, LB_GETCURSEL, 0, 0);

			//Get the length of the name and phone entered in the edit box for customer
			int lengthName = SendMessage(customNameInput, WM_GETTEXTLENGTH, 0, 0);
			int lengthPhone = SendMessage(customPhoneInput, WM_GETTEXTLENGTH, 0, 0);

			//If both are not empty, then proceed
			if (lengthName != 0)
			{
				if (lengthPhone != 0)
				{
					//Get names up to 16, but makes name 15 characters maximum to avoid ambiguity from TCHAR
					TCHAR names[16];
					GetWindowTextLength(customNameInput);
					GetWindowText(customNameInput, names, 16);
					wstring nameString = names;
					string cuName = string(nameString.begin(), nameString.end());

					//Get phones up to 11, but same as name up to 10 characters
					TCHAR phones[11];
					GetWindowTextLength(customPhoneInput);
					GetWindowText(customPhoneInput, phones, 11);
					wstring phoneString = phones;
					string cuPhone = string(phoneString.begin(), phoneString.end());

					
					
					//If all are digits, proceed, else display warning message
					if (regex_match(cuPhone, myRegex))
					{
						//When both name and phone are validated, check if the customer's details are correct
						if (::MessageBox(hWnd, TEXT("Are you sure?"), TEXT("Confirm Customer?"), MB_OKCANCEL) == IDOK)
						{
							//If the user has selected the Product of the Day, then do this section
							if (productStockList[ItemIndex]->getCode() == spotPrize.getSCode())
							{
								//Display user that this product was the spot prize
								MessageBox(hWnd, TEXT("You made the Catch of the Day"), TEXT("BINGO!"), MB_OK);
								TCHAR buff[4];
								GetWindowTextLength(quantityInput);
								GetWindowText(quantityInput, buff, 4);
								stock = _tstoi(buff);

								//Calculate new sales value from Spot Prize class
								spotPrize.setQuantity(stock);
								spotPrize.setPrice(productStockList[ItemIndex]->getCost());
								float spottotal = spotPrize.calValue();

								//Change the prize and saved money:
								string sSpottotal = convertCost(spottotal);
								wstring wSpottotal = wstring(sSpottotal.begin(), sSpottotal.end());
								LPCWSTR lSpottotal = wSpottotal.c_str();

								float spotSavedMoney = originTotal - spottotal;
								string sSpotSavedMoney = convertCost(spotSavedMoney);
								wstring wSpotSavedMoney = wstring(sSpotSavedMoney.begin(), sSpotSavedMoney.end());
								LPCWSTR lSpotSavedMoney = wSpotSavedMoney.c_str();
								SetWindowText(youSaveShow, lSpotSavedMoney);


								SetWindowText(discountedPriceText, TEXT("Spot Prize"));
								SetWindowText(discountedPriceShow, lSpottotal);


								stringstream messageLine;

								//Display the discount when selling the spot prize.
								messageLine << "Your new discount total is " << convertCost(spottotal) << ".";

								MessageBoxA(hWnd, messageLine.str().c_str(), "Spot Prize", MB_OK);

								//Add SpotPrize's code, BigSpender's details and SalesInvoice's details to SalesInvoice array
								salesInvoiceList[listCount] = new SpotPrize(spotPrize.getSCode(),
									iNumber,
									productStockList[ItemIndex]->getName(),
									productStockList[ItemIndex]->markUp(),
									stock,
									spottotal,
									cuName,
									cuPhone);
							}

							//If sales value greater than 20 but not spot prize
							else
							{
								//Calculate the sales value using BigSpender methods
								TCHAR buff[4];
								GetWindowTextLength(quantityInput);
								GetWindowText(quantityInput, buff, 4);
								int stock = _tstoi(buff);

								bigSpender.setQuantity(stock);
								bigSpender.setPrice(productStockList[ItemIndex]->getCost());
								float total = bigSpender.calValue();

								//Add the BigSpender's details and SalesInvoice's details to SalesInvoice array
								salesInvoiceList[listCount] = new BigSpender(
									iNumber,
									productStockList[ItemIndex]->getName(),
									productStockList[ItemIndex]->markUp(),
									stock,
									total,
									cuName,
									cuPhone);
							}

							//When finished, listbox, quantity and sell are enabled
							EnableWindow(productList, TRUE);
							EnableWindow(quantityInput, TRUE);
							EnableWindow(sellProductBtn, TRUE);
							//All customer's section are hidden
							ShowWindow(customNameText, SW_HIDE);
							ShowWindow(customPhoneText, SW_HIDE);
							ShowWindow(customNameInput, SW_HIDE);
							ShowWindow(customPhoneInput, SW_HIDE);
							ShowWindow(saveCustomInfoBtn, SW_HIDE);
							ShowWindow(cancelCustomInfoBtn, SW_HIDE);

							ShowWindow(discountedPriceText, SW_HIDE);
							ShowWindow(discountedPriceShow, SW_HIDE);
							ShowWindow(youSaveText, SW_HIDE);
							ShowWindow(youSaveShow, SW_HIDE);

							//Increase the invoice number by 1 and decrease the product's stock by the user's stock
							iNumber++;
							string iString = to_string(iNumber);
							wstring wIString = wstring(iString.begin(), iString.end());
							LPCWSTR iChar = wIString.c_str();

							listCount++;
							productStockList[ItemIndex]->makeSale(stock);

							//If invoice number is 21 or greater, stop user from proceeding
							if (iNumber >= 21)
							{
								MessageBox(hWnd, TEXT("This is the final sale."), TEXT("Maximum Sales"), MB_OK);
								ShowWindow(sellProductBtn, SW_HIDE);
							}

							//Else, display the next invoice number
							else
							{
								SetWindowText(invoiceNumShow, iChar);
							}

						}
					}

					//If all characters from phone are not digits, display error
					else
					{
						MessageBoxA(hWnd, "Phone must be in digits.", "Invalid Phone", MB_OK);
					}
				}

				//If phone is empty, display error
				else
				{
					MessageBoxA(hWnd, "Phone should not be empty.", "Empty Phone", MB_OK);
				}
			}

			//If name is empty, display error
			else
			{
				MessageBoxA(hWnd, "Name should not be empty.", "Empty Name", MB_OK);
			}


		}
		break;
		//If cancel saving cumtom info:
		case IDC_BUTTON4:
		{
			if (::MessageBoxA(hWnd, "Cancel saving custom info?", "Exit Without Saving", MB_OKCANCEL) == IDOK)
			{

				EnableWindow(productList, TRUE);
				EnableWindow(quantityInput, TRUE);
				EnableWindow(sellProductBtn, TRUE);
				ShowWindow(customNameText, SW_HIDE);
				ShowWindow(customPhoneText, SW_HIDE);
				ShowWindow(customNameInput, SW_HIDE);
				ShowWindow(customPhoneInput, SW_HIDE);
				ShowWindow(discountedPriceText, SW_HIDE);
				ShowWindow(discountedPriceShow, SW_HIDE);
				ShowWindow(youSaveText, SW_HIDE);
				ShowWindow(youSaveShow, SW_HIDE);
				ShowWindow(saveCustomInfoBtn, SW_HIDE);
				ShowWindow(cancelCustomInfoBtn, SW_HIDE);
			}

		}
		break;
		//
		case IDC_BUTTON5:
		{
			if (::MessageBox(hWnd, TEXT("Are you sure to close business?"), TEXT("Save And Exit"), MB_OKCANCEL) == IDOK)
			{
				//Add date
				time_t rawtime = time(0);
				char buffer[100];
				struct tm  timeinfo;
				localtime_s(&timeinfo, &rawtime);
			
				//Format date to set name in SalesInvoice in day/month/year
				strftime(buffer, sizeof(buffer), "%d-%m-%Y.%X", &timeinfo);


				//Add total sale for date to SalesInvoice object to write last line
				string date = string() + "Total sales for " + buffer;

				//Decrease Invoice Number to get the total number of invoice
				iNumber--;
				//Set the invoice number and date to SalesInvoice object
				salesInvoice.setNumber(iNumber);
				salesInvoice.setName(date);
				//Initialize total cost, sales value and quantity to 0
				float tSale = 0;
				int tQuantity = 0;
				//Go through every elements in SalesInvoice array
				for (int counting = 0; counting < 20; counting++)
				{
					//Add the total cost, sales value and quantity from the array element
					//If the array element is not empty
					if (salesInvoiceList[counting] != nullptr)
					{
						tQuantity += salesInvoiceList[counting]->getQuantity();
						tSale += salesInvoiceList[counting]->getValue();
					}
				}

				//Set total cost, quantity and sales value to SalesInvoice object
				salesInvoice.setNumber(999);
				salesInvoice.setPrice(0);
				salesInvoice.setQuantity(tQuantity);
				salesInvoice.setValue(tSale);

				//Format this date for saving file as Year-Month-Day
				
				strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);

				//Write all SalesInvoice array and object in this sales file
				string saleSaveFileName;
				saleSaveFileName = string() + buffer + " sales.txt";

				ofstream saleFile(saleSaveFileName, ios::app);

				//Write all of the elements in SalesInvoice array into file until the current element is null
				for (int counting = 0; counting < 20; counting++)
				{
					if (salesInvoiceList[counting] != NULL)
					{
						saleFile << salesInvoiceList[counting]->printSale() << endl;
					}
				}

				//Write SalesInvoice object into Sales file
				saleFile << salesInvoice.printSale() << endl;

				//Close Sales file
				saleFile.close();

				//Write the renewal product array and overwrite the product file
				ofstream productSaveFile(productFileName, ios::out);

				for (int counting = 0; counting < 10; counting++)
				{
					productSaveFile << productStockList[counting]->printProduct();
					if (counting < 9)
					{
						productSaveFile << endl;
					}
				}

				//Open Sales file using shell execute
				wstring wIString = wstring(saleSaveFileName.begin(), saleSaveFileName.end());
				LPCWSTR fChar = wIString.c_str();
				ShellExecute(NULL, NULL, fChar, NULL, NULL, SW_SHOW);

				//Open Product file using shell execute
				wIString = wstring(productFileName.begin(), productFileName.end());
				fChar = wIString.c_str();
				ShellExecute(NULL, NULL, fChar, NULL, NULL, SW_SHOW);

				//Exit application
				exit(0);
			}

		}
		break;

		//Spot Price Setting Button:
		case IDC_BUTTON6:
		{ 
			spotPrize.setSCode(spotCodeTemp);
			//Hide Spot Price Setting Window:
			ShowWindow(setSpotText, SW_HIDE);
			ShowWindow(setSpotShow, SW_HIDE);
			ShowWindow(setSpotBtn, SW_HIDE);
				
  
		}
		break;

		}
		break;

	}

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		//If closing application by clicking close button from application, 
		//check before closing to ensure that all data will not be saved
		if (MessageBox(hWnd, TEXT("Are you sure to quit WITHOUT SAVING?"), TEXT("Exit Without Saving"), MB_YESNO) == IDYES)
		{
			exit(0);
		}
		break;


	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}
