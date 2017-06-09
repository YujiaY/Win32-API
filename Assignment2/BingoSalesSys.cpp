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
string spotCodeTemp;
float originTotal;

							 //Window Handle from buttons, labels, edit boxes and list box
HWND hWnd,
systemName, imageShow,
productListText, productList,
retailPriceText, retailPriceShow,
quantityText, quantityInput,
totalValueText, totalValueShow,
discountedPriceText, discountedPriceShow,
youSaveText, youSaveShow,
calTotal, sellProduct,
customNameText, customPhoneText,
customNameInput, customPhoneInput,
saveCustomInfo, cancelCustomInfo,
invoiceNumText, invoiceNumShow,
saveTodayRecord, closeBusiness,
setSpotText, setSpotShow, setSpotBtn;




//Set the iNumber(invoice) to 1 and listCount(number of list in salesList) and 
//stock(user's input for stock) to 0
int iNumber = 1, listCount = 0, stock = 0;




//Collect file's name and set as string for reading product and rewrite product
string pFile = "Products.txt";

HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

static TCHAR szAppName[] = TEXT("Bingo Office Supplies");
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
static int iniXSize = 800;
static int iniYSize = 600;


// Forward declarations of functions included in this code module:
//ATOM                MyRegisterClass(HINSTANCE hInstance);
//BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
//INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);








int WINAPI WinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	PSTR szCmdLine,
	int iCmdShow)
{
	//UNREFERENCED_PARAMETER(hPrevInstance);
	//UNREFERENCED_PARAMETER(szCmdLine);

	MSG      msg;
	WNDCLASS wndclass;


	GdiplusStartupInput gdiPlusInput;
	ULONG_PTR		token;
	GdiplusStartup(&token, &gdiPlusInput, NULL);


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
		SendMessageA(productList, LB_ADDSTRING, 0, (LPARAM)list[listCount]->getName().c_str());
	}

	//Add srand to avoid picking the same number from rand as this was using as an argument
	//This allows to pick any numbers without the system to random generate the same number
	srand(time(0));
	//Set the SpotPrize's product for anyone who picked this product and sell higher than $20


	//spot.setSCode(list[rand() % 10]->getCode());

	file.close();

	// Initialize global strings
	//LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	//LoadStringW(hInstance, IDC_BINGOOFFICE, szWindowClass, MAX_LOADSTRING);


	// MyRegisterClass(hInstance);

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

	hWnd = CreateWindow(szAppName,                  // window class name
		szAppName,                 // window caption
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
	UpdateWindow(hWnd);



	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

//Display the retail price and image of this product
void showProduct(HWND hwnd)
{
	//Get the selected index from listbox
	//int itemIndex = (int)SendMessage(productList, LB_GETCURSEL, 0, 0);
	//MessageBox(hwnd, TEXT("Are you fucking?"), TEXT("Yes!"), MB_OK);;

	int itemIndex = (int)SendMessage(productList, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);

	//If the listbox was not selected then display error message
	if (itemIndex > -1)
	{
		//Else, then display the retail price and image of the selected product

		//Display retail price to static text for retail and convert as money format
		string price = convertCost(list[itemIndex]->markUp());

		wstring item = wstring(price.begin(), price.end());

		LPCWSTR cwItem = item.c_str();

		//HWND imageShow = CreateWindow(TEXT("STATIC"), TEXT(""), WS_CHILD | WS_VISIBLE, 250, 250, 150, 200, hwnd, NULL, NULL, NULL);

		ShowWindow(imageShow, SW_SHOW);
		//Get image file's directory and set as string
		string imageFile = string() + "image/" + list[itemIndex]->getImage();

		wstring wImage = wstring(imageFile.begin(), imageFile.end());

		LPCWSTR lImage = wImage.c_str();

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
		string sSelectSpot = list[itemIndex]->getName();
		wstring wSelectSpot = wstring(sSelectSpot.begin(), sSelectSpot.end());
		LPCWSTR lSelectSpot = wSelectSpot.c_str();

		spotCodeTemp = list[itemIndex]->getCode();;

		SetWindowText(GetDlgItem(hWnd, IDC_STATIC14), lSelectSpot);
		//Make sell button enable
		EnableWindow(sellProduct, true);

		//Delete image to avoid overloading image's data
		delete image;
	}

	else
	{
		//Display message if list box is not selected
		::MessageBeep(MB_ICONASTERISK);
		MessageBox(hWnd, _T("You must select a product."), _T("Must Select Product!"), MB_OK);
	}
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HWND hwndTotalPrice;

	switch (message)
	{
	case WM_CREATE:
	{


		//
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
			55, 60, 90, 22,
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
		SendMessage(productList, LB_ADDSTRING, 0, (LPARAM)TEXT("Calculator"));
		SendMessage(productList, LB_ADDSTRING, 1, (LPARAM)TEXT("Thumb Drive 10GB"));
		SendMessage(productList, LB_ADDSTRING, 2, (LPARAM)TEXT("Hole Punch"));
		SendMessage(productList, LB_ADDSTRING, 3, (LPARAM)TEXT("Ruler"));
		SendMessage(productList, LB_ADDSTRING, 4, (LPARAM)TEXT("Eraser"));
		SendMessage(productList, LB_ADDSTRING, 5, (LPARAM)TEXT("Pen"));
		SendMessage(productList, LB_ADDSTRING, 6, (LPARAM)TEXT("Stapler"));
		SendMessage(productList, LB_ADDSTRING, 7, (LPARAM)TEXT("Box of Paper Clips"));
		SendMessage(productList, LB_ADDSTRING, 8, (LPARAM)TEXT("CD Box of 10"));
		SendMessage(productList, LB_ADDSTRING, 9, (LPARAM)TEXT("Pencil"));

		//Create Retail Price:
		retailPriceText = CreateWindow(TEXT("STATIC"),
			TEXT("Retail Price: $A"),
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
			TEXT("Total Value: $A"),
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
		//
		calTotal = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Calculate Total"),
			WS_CHILD | WS_VISIBLE,
			15, 370, 120, 20, hWnd,
			(HMENU)IDC_BUTTON1,
			NULL, NULL);
		//
		sellProduct = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Sell Product"),
			WS_CHILD | WS_VISIBLE,
			155, 370, 100, 20, hWnd,
			(HMENU)IDC_BUTTON2,
			NULL, NULL);
		//
		imageShow = CreateWindow(
			TEXT("STATIC"),
			TEXT(""),
			WS_CHILD | WS_VISIBLE,
			iniXSize / 2 - 100, 80, 240, 240,
			hWnd,
			(HMENU)IDC_PICTURE1,
			NULL, NULL);
		//
		closeBusiness = CreateWindow(
			TEXT("BUTTON"),
			TEXT("Close of Business"),
			WS_CHILD | WS_VISIBLE,
			275, 460, 220, 40,
			hWnd,
			(HMENU)IDC_BUTTON5,
			NULL, NULL);
		//
		invoiceNumText = CreateWindow(
			TEXT("STATIC"),
			TEXT("Invoice Number:"),
			WS_CHILD | WS_VISIBLE,
			275, 340, 115, 20,
			hWnd, NULL, NULL, NULL);
		invoiceNumShow = CreateWindow(
			TEXT("STATIC"), TEXT(""),
			WS_CHILD | WS_VISIBLE,
			400, 340, 150, 20,
			hWnd, NULL, NULL, NULL);
		//
		setSpotText = CreateWindow(
			TEXT("STATIC"),
			TEXT("Select Spot Prize:"),
			WS_CHILD | WS_VISIBLE,
			550, 60, 150, 22,
			hWnd, (HMENU)IDC_STATIC13, NULL, NULL);
		setSpotShow = CreateWindow(
			TEXT("STATIC"), TEXT(""),
			WS_CHILD | WS_VISIBLE,
			550, 90, 150, 20 ,
			hWnd, (HMENU)IDC_STATIC14, NULL, NULL);
		setSpotBtn = CreateWindow(
			TEXT("BUTTON"), TEXT("Set As Spot"),
			WS_CHILD | WS_VISIBLE,
			575, 120, 100, 30,
			hWnd, 
			(HMENU)IDC_BUTTON6,
			NULL, NULL);




		//Set initial invoice number and display in static
		string iString = to_string(iNumber);
		wstring wIString = wstring(iString.begin(), iString.end());
		LPCWSTR iChar = wIString.c_str();
		SetWindowText(invoiceNumShow, iChar);

		//Set focus on listbox
		SetFocus(productList);
		ShowWindow(imageShow, SW_HIDE);
		EnableWindow(sellProduct, false);
	}
	break;

	case WM_CTLCOLORSTATIC:
	{
		HDC hdc = (HDC)wParam;
		HWND hwnd = (HWND)lParam;

		if (GetDlgCtrlID(hwnd) == IDC_STATIC1)
		{
			SetTextColor(hdc, RGB(112, 129, 219));
			SetBkColor(hdc, RGB(100, 253, 72));
			return (LRESULT)CreateSolidBrush(RGB(100, 253, 72));
		}
		if (GetDlgCtrlID(hwnd) == IDC_STATIC2)
		{
			SetTextColor(hdc, RGB(112, 129, 219));
			SetBkColor(hdc, RGB(100, 253, 72));
			return (LRESULT)CreateSolidBrush(RGB(255, 255, 255));
		}
		if (GetDlgCtrlID(hwnd) == IDC_LISTBOX1)
		{
			SetTextColor(hdc, RGB(112, 129, 219));
			SetBkColor(hdc, RGB(100, 253, 72));
			return (LRESULT)CreateSolidBrush(RGB(255, 255, 255));
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

				SetWindowText(GetDlgItem(hWnd, IDC_STATIC6), TEXT(""));

			}

		}
		break;

		//Calculate the total price:
		case IDC_BUTTON1:
		{
			//hwndTotalPrice = GetDlgItem(hWnd, IDC_STATIC6);
			//SetWindowText(hwndTotalPrice, (LPCWSTR)soMuch.c_str());
			//MessageBox(hWnd, TEXT("Text Changed!"), TEXT("info"), MB_OK);

			showProduct(hWnd);
			//Get integer from edit box 'quantity'
			TCHAR buff[4];
			//Get the length of the quantity's box
			GetWindowTextLength(quantityInput);
			//Get the text and put those characters in buff
			GetWindowText(quantityInput, buff, 4);
			//Change the character into integer
			stock = _tstoi(buff);

			//Retrieve data from selected item
			int itemIndex = (int)SendMessage(productList, LB_GETCURSEL, 0, 0);
			string productItem = list[itemIndex]->getName();
			//If there are avaliable stocks left by user input, then do this section
			if (list[itemIndex]->stockAvailable(stock))
			{
				//Get price from selected item's information from listbox and set it
				sales.setPrice(list[itemIndex]->getCost());
				//Get quantity from edit box
				sales.setQuantity(stock);

				//Create a static for sales value
				//value = CreateWindow(TEXT("STATIC"), _T(""), WS_CHILD | WS_VISIBLE,
				//600, 400, 150, 20, hWnd, NULL, NULL, NULL);

				//Calculate the sales value from the price and quantity
				originTotal = sales.calValue();

				//Display the sales value as money format to static
				string Totalvalues = convertCost(originTotal);

				wstring sTotalValue = wstring(Totalvalues.begin(), Totalvalues.end());

				LPCWSTR cTotalValue = sTotalValue.c_str();

				hwndTotalPrice = GetDlgItem(hWnd, IDC_STATIC6);
				SetWindowText(totalValueShow, cTotalValue);

				if (originTotal > 20)
				{

					//Preventing user from changing quantity and product selection
					//	EnableWindow(sellProduct, FALSE);
					//	EnableWindow(productList, FALSE);
					//	EnableWindow(quantityInput, FALSE);

					//Set the stock and product's price to calculate the BigSpender's sales value
					spend.setQuantity(stock);
					spend.setPrice(list[itemIndex]->getCost());
					float dicountedTotal = spend.calValue();

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


			}
			//If quantity is greater than product's stock, then display unavaliable message box
			else
			{
				string lStock = string() + "There are only " + to_string(list[itemIndex]->getStock()) + " " + productItem + " left.";
				::MessageBeep(MB_ICONASTERISK);
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

			//Retrieve data from selected item
			int itemIndex = (int)SendMessage(productList, LB_GETCURSEL, 0, 0);
			string productItem = list[itemIndex]->getName();
			//If there are avaliable stocks left by user input, then do this section
			if (list[itemIndex]->stockAvailable(stock))
			{
				//Get price from selected item's information from listbox and set it
				sales.setPrice(list[itemIndex]->getCost());
				//Get quantity from edit box
				sales.setQuantity(stock);

				//Create a static for sales value
				//value = CreateWindow(TEXT("STATIC"), _T(""), WS_CHILD | WS_VISIBLE,
				//600, 400, 150, 20, hWnd, NULL, NULL, NULL);

				//Calculate the sales value from the price and quantity
				originTotal = sales.calValue();

				//Display the sales value as money format to static
				string Totalvalues = convertCost(originTotal);

				wstring sTotalValue = wstring(Totalvalues.begin(), Totalvalues.end());

				LPCWSTR cTotalValue = sTotalValue.c_str();

				hwndTotalPrice = GetDlgItem(hWnd, IDC_STATIC6);
				SetWindowText(totalValueShow, cTotalValue);

				if (originTotal > 0)
				{
					//Display Message Box to prompt user to confirm sale or cancel sale, if ok, then proceed
					//::MessageBeep(MB_ICONASTERISK);
					if (::MessageBox(hWnd, TEXT("Confirm Sale?"), TEXT("Are you sure?"), MB_OKCANCEL) == IDOK)
					{
						//If the sales value over 20, display customer section
						if (originTotal > 20)
						{
							//
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
							saveCustomInfo = CreateWindow(TEXT("BUTTON"), TEXT("Save Info"),
								WS_CHILD | WS_VISIBLE,
								275, 430, 110, 20, hWnd,
								(HMENU)IDC_BUTTON3, NULL, NULL);
							cancelCustomInfo = CreateWindow(TEXT("BUTTON"), TEXT("Cancel"),
								WS_CHILD | WS_VISIBLE,
								400, 430, 110, 20, hWnd,
								(HMENU)IDC_BUTTON4, NULL, NULL);


							//Preventing user from changing quantity and product selection
							EnableWindow(sellProduct, FALSE);
							EnableWindow(productList, FALSE);
							EnableWindow(quantityInput, FALSE);

							//Set the stock and product's price to calculate the BigSpender's sales value
							spend.setQuantity(stock);
							spend.setPrice(list[itemIndex]->getCost());
							float dicountedTotal = spend.calValue();

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
							salesList[listCount] = new SalesInvoice(iNumber, productItem, sales.getPrice(), stock, originTotal);
							//Increment invoice number by 1 and display next invoice number
							iNumber++;
							string iString = to_string(iNumber);
							wstring wIString = wstring(iString.begin(), iString.end());
							LPCWSTR iChar = wIString.c_str();

							// SetWindowText(totalValueShow, TEXT(""));
							//Increment listCount by 1 to add new elements to SalesInvoice array
							listCount++;
							//Decrease the number of stock by user from the product's stock
							list[itemIndex]->makeSale(stock);

							//If invoice number has reached to 21, prevent user from adding more sales
							if (iNumber >= 21)
							{
								MessageBox(hWnd, TEXT("This is the final sale."), TEXT("Maximum Sales"), MB_OK);
								ShowWindow(sellProduct, SW_HIDE);
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
					::MessageBeep(MB_ICONASTERISK);
					MessageBoxA(hWnd, "Quantity must be filled in as integer and greater than 0.", "Invalid Input.", MB_OK);
				}

			}
			//If quantity is greater than product's stock, then display unavaliable message box
			else
			{
				string lStock = string() + "There are only " + to_string(list[itemIndex]->getStock()) + " " + productItem + " left.";
				::MessageBeep(MB_ICONASTERISK);
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

			//If both are not empty, then proceed, else display warning message box
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
						if (::MessageBox(hWnd, TEXT("Confirm Customer?"), TEXT("Are you sure?"), MB_OKCANCEL) == IDOK)
						{
							//If the user has selected the Product of the Day, then do this section
							if (list[ItemIndex]->getCode() == spot.getSCode())
							{
								//Display user that this product was the spot prize
								MessageBox(hWnd, TEXT("You made the Catch of the Day"), TEXT("BINGO!"), MB_OK);
								TCHAR buff[4];
								GetWindowTextLength(quantityInput);
								GetWindowText(quantityInput, buff, 4);
								stock = _tstoi(buff);

								//Calculate new sales value from Spot Prize class
								spot.setQuantity(stock);
								spot.setPrice(list[ItemIndex]->getCost());
								float spottotal = spot.calValue();

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


								stringstream messLine;

								//Display the discount when selling the spot prize.
								messLine << "Your new discount total is " << convertCost(spottotal) << ".";

								MessageBoxA(hWnd, messLine.str().c_str(), "Spot Prize", MB_OK);

								

								//Add SpotPrize's code, BigSpender's details and SalesInvoice's details to SalesInvoice array
								salesList[listCount] = new SpotPrize(spot.getSCode(),
									iNumber,
									list[ItemIndex]->getName(),
									list[ItemIndex]->markUp(),
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

								spend.setQuantity(stock);
								spend.setPrice(list[ItemIndex]->getCost());
								float total = spend.calValue();

								//Add the BigSpender's details and SalesInvoice's details to SalesInvoice array
								salesList[listCount] = new BigSpender(
									iNumber,
									list[ItemIndex]->getName(),
									list[ItemIndex]->markUp(),
									stock,
									total,
									cuName,
									cuPhone);
							}

							//When finished, listbox, quantity and sell are enabled
							EnableWindow(productList, TRUE);
							EnableWindow(quantityInput, TRUE);
							EnableWindow(sellProduct, TRUE);
							//All customer's section are hidden
							ShowWindow(customNameText, SW_HIDE);
							ShowWindow(customPhoneText, SW_HIDE);
							ShowWindow(customNameInput, SW_HIDE);
							ShowWindow(customPhoneInput, SW_HIDE);
							ShowWindow(discountedPriceText, SW_HIDE);
							ShowWindow(discountedPriceShow, SW_HIDE);
							ShowWindow(youSaveText, SW_HIDE);
							ShowWindow(youSaveShow, SW_HIDE);
							ShowWindow(saveCustomInfo, SW_HIDE);
							ShowWindow(cancelCustomInfo, SW_HIDE);
							//Make the sales value's text and quantity's text empty
							//SetWindowText(quantity, TEXT(""));
							//SetWindowText(value, TEXT(""));

							//Increase the invoice number by 1 and decrease the product's stock by the user's stock
							iNumber++;
							string iString = to_string(iNumber);
							wstring wIString = wstring(iString.begin(), iString.end());
							LPCWSTR iChar = wIString.c_str();

							listCount++;
							list[ItemIndex]->makeSale(stock);

							//If invoice number is 21 or greater, stop user from proceeding
							if (iNumber >= 21)
							{
								MessageBox(hWnd, TEXT("This is the final sale."), TEXT("Maximum Sales"), MB_OK);
								ShowWindow(sellProduct, SW_HIDE);
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
				EnableWindow(sellProduct, TRUE);
				ShowWindow(customNameText, SW_HIDE);
				ShowWindow(customPhoneText, SW_HIDE);
				ShowWindow(customNameInput, SW_HIDE);
				ShowWindow(customPhoneInput, SW_HIDE);
				ShowWindow(discountedPriceText, SW_HIDE);
				ShowWindow(discountedPriceShow, SW_HIDE);
				ShowWindow(youSaveText, SW_HIDE);
				ShowWindow(youSaveShow, SW_HIDE);
				ShowWindow(saveCustomInfo, SW_HIDE);
				ShowWindow(cancelCustomInfo, SW_HIDE);
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
				//timeinfo = *localtime(&rawtime);
				localtime_s(&timeinfo, &rawtime);
			
				//Format date to set name in SalesInvoice in day/month/year
				//strftime(buffer, 80, "%d/%m/%Y", localtime(&rawtime));
				strftime(buffer, sizeof(buffer), "%d-%m-%Y.%X", &timeinfo);


				//Add total sale for date to SalesInvoice object to write last line
				string date = string() + "Total sales for " + buffer;

				//Decrease Invoice Number to get the total number of invoice
				iNumber--;
				//Set the invoice number and date to SalesInvoice object
				sales.setNumber(iNumber);
				sales.setName(date);
				//Initialize total cost, sales value and quantity to 0
				float tSale = 0;
				int tQuantity = 0;
				//Go through every elements in SalesInvoice array
				for (int counting = 0; counting < 20; counting++)
				{
					//Add the total cost, sales value and quantity from the array element
					//If the array element is not empty
					if (salesList[counting] != nullptr)
					{
						tQuantity += salesList[counting]->getQuantity();
						tSale += salesList[counting]->getValue();
					}
				}

				//Set total cost, quantity and sales value to SalesInvoice object
				sales.setNumber(999);
				sales.setPrice(0);
				sales.setQuantity(tQuantity);
				sales.setValue(tSale);

				//Format this date for saving file as Year-Month-Day
				
				strftime(buffer, sizeof(buffer), "%Y-%m-%d", &timeinfo);

				//Write all SalesInvoice array and object in this sales file
				string fileName;
				fileName = string() + buffer + " sales.txt";

				ofstream saleFile(fileName, ios::app);

				//Write all of the elements in SalesInvoice array into file until the current element is null
				for (int counting = 0; counting < 20; counting++)
				{
					if (salesList[counting] != NULL)
					{
						saleFile << salesList[counting]->printSale() << endl;
					}
				}

				//Write SalesInvoice object into Sales file
				saleFile << sales.printSale() << endl;

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
		break;

		//Spot Price Setting Button:
		case IDC_BUTTON6:
		{ 
			spot.setSCode(spotCodeTemp);
			//Hide Spot Price Setting Window:
			ShowWindow(setSpotText, SW_HIDE);
			ShowWindow(setSpotShow, SW_HIDE);
			ShowWindow(setSpotBtn, SW_HIDE);
				
  
		}
		break;

		}
		break;

	}


	/*	by vando
	case WM_CTLCOLORSTATIC:
	{
	HDC hdc = (HDC)wParam;
	SetTextColor(hdc, RGB(0xFF, 0x00, 0x00));
	}
	return (BOOL)((HBRUSH)GetStockObject(NULL_BRUSH));
	*/
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_CLOSE:
		//If closing application by clicking close button from application, check before closing to ensure that all data will not be saved
		::MessageBeep(MB_ICONASTERISK);
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
