//////////////////////////////////////////////////////////////////////////
// TestListBoxWin32.cpp
//
// By Giovanni Dicanio <giovanni.dicanio AT gmail.com>
// 2009, June
//////////////////////////////////////////////////////////////////////////



//=======================================================================
//                              INCLUDES
//=======================================================================

#define WIN32_LEAN_AND_MEAN
#define IDC_LIST1 1005
HANDLE hList1;
#define STRICT

#include <windows.h>            // Win32 Platform SDK
#include <tchar.h>              // Support TCHAR's
#include <crtdbg.h>             // _ASSERTE
#include "resource.h"           // App GUI resources



//=======================================================================
//                      FUNCTION IMPLEMENTATIONS
//=======================================================================

//-----------------------------------------------------------------------
// FUNC: OnInitDialog
// DESC: Dialog-box Initialization.
//-----------------------------------------------------------------------
static void OnInitDialog(HWND hwndDlg)
{
	// Strings to be added to the listbox
	LPCTSTR strings[] = {
		_T("First string."),
		_T("Hello Bob"),
		_T("Another one."),
		_T("Hello from Win32 SDK."),
		NULL
	};

	// Add strings to the listbox
	LPCTSTR * pCurrString = strings;
	while (*pCurrString != NULL)
	{
		LRESULT ret = SendDlgItemMessage(
			hwndDlg, IDC_LIST1, LB_ADDSTRING,
			(WPARAM)0, (LPARAM)(*pCurrString));
		_ASSERTE(ret != LB_ERR);

		// Move to next string
		++pCurrString;
	}
}


//-----------------------------------------------------------------------
// FUNC: OnButtonClickGetSelection
// DESC: Handles click of button "Get Selection".
//       Displays listbox currently selected item.
//-----------------------------------------------------------------------
static void OnButtonClickGetSelection(HWND hwndDlg)
{
	// Get handle of listbox
	HWND hwndList1 = GetDlgItem(hwndDlg, IDC_LIST1);
	_ASSERTE(hwndList1 != NULL);

	// Get current selection index in listbox
	int itemIndex = (int)SendMessage(hwndList1, LB_GETCURSEL, (WPARAM)0, (LPARAM)0);
	if (itemIndex == LB_ERR)
	{
		// No selection
		return;
	}

	// Get length of text in listbox
	int textLen = (int)SendMessage(hwndList1, LB_GETTEXTLEN, (WPARAM)itemIndex, 0);

	// Allocate buffer to store text (consider +1 for end of string)
	TCHAR * textBuffer = new TCHAR[textLen + 1];

	// Get actual text in buffer
	SendMessage(hwndList1, LB_GETTEXT, (WPARAM)itemIndex, (LPARAM)textBuffer);

	// Show it
	MessageBox(NULL, textBuffer, _T("Selected Text:"), MB_OK);

	// Free text
	delete[] textBuffer;

	// Avoid dangling references
	textBuffer = NULL;
}


//-----------------------------------------------------------------------
// FUNC: DlgProc
// DESC: Dialog-box procedure
//-----------------------------------------------------------------------
INT_PTR CALLBACK DlgProc(HWND hWndDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	switch (msg)
	{
	case WM_INITDIALOG:
		OnInitDialog(hWndDlg);
		return TRUE;

	case WM_COMMAND:
		switch (LOWORD(wParam))
		{
		case IDC_BTN_GETSELECTION:
			OnButtonClickGetSelection(hWndDlg);
			return TRUE;

		case IDOK:
			// fall through IDCANCEL

		case IDCANCEL:
			EndDialog(hWndDlg, wParam);
			return TRUE;
		}
		break;
	}

	return FALSE;
}


//-----------------------------------------------------------------------
// FUNC: WinMain
// DESC: App Entry-point
//-----------------------------------------------------------------------
int WINAPI WinMain(
	HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPSTR lpCmdLine,
	int nCmdShow
)
{
	// Show the dialog-box
	DialogBox(
		hInstance,
		MAKEINTRESOURCE(IDD_TESTLISTBOX),
		NULL,
		DlgProc
	);

	return FALSE;

}


//////////////////////////////////////////////////////////////////////////