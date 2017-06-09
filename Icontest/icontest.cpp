
// Win32 Tutorial (Toolbars)

// Alan Baylis 2004



#include <windows.h>

#include <commctrl.h>

#include "resource.h"



const char ClassName[] = "MainWindowClass";

HWND hWndToolBar;



LRESULT CALLBACK WndProc(HWND    hWnd,

	UINT    Msg,

	WPARAM  wParam,

	LPARAM  lParam)

{

	switch (Msg)

	{

	case WM_CREATE:

	{

		TBADDBITMAP tbab;

		TBBUTTON tbb[3];



		hWndToolBar = CreateWindowEx(

			0,

			TOOLBARCLASSNAME,

			NULL,

			WS_CHILD,

			0,

			0,

			0,

			0,

			hWnd,

			(HMENU)IDTB_TOOLBAR,

			(HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE),

			NULL);



		if (!hWndToolBar)

		{

			MessageBox(NULL, L"Tool Bar Failed.", L"Error", MB_OK | MB_ICONERROR);

			return 0;

		}



		SendMessage(hWndToolBar, TB_BUTTONSTRUCTSIZE, (WPARAM)sizeof(TBBUTTON), 0);



		SendMessage(hWndToolBar, TB_SETBITMAPSIZE, (WPARAM)0, (LPARAM)MAKELONG(20, 20));



		tbab.hInst = (HINSTANCE)GetWindowLong(hWnd, GWL_HINSTANCE);

		tbab.nID = IDB_TOOLBITMAP;

		SendMessage(hWndToolBar, TB_ADDBITMAP, (WPARAM)2, (LPARAM)&tbab);



		ZeroMemory(tbb, sizeof(tbb));



		tbb[0].iBitmap = 20;

		tbb[0].idCommand = TB_TEST1;

		tbb[0].fsState = TBSTATE_ENABLED;

		tbb[0].fsStyle = TBSTYLE_BUTTON;



		tbb[1].fsStyle = TBSTYLE_SEP;



		tbb[2].iBitmap = 21;

		tbb[2].idCommand = TB_TEST2;

		tbb[2].fsState = TBSTATE_ENABLED;

		tbb[2].fsStyle = TBSTYLE_BUTTON;



		SendMessage(hWndToolBar, TB_ADDBUTTONS, 3, (LPARAM)&tbb);

		ShowWindow(hWndToolBar, SW_SHOW);

	}

	break;



	case WM_COMMAND:

	{

		switch (LOWORD(wParam))

		{

		case TB_TEST1:

		{

			MessageBox(NULL, L"Toolbar Button One", L"Success", MB_OK | MB_ICONINFORMATION);

		}

		break;



		case TB_TEST2:

		{

			MessageBox(NULL, L"Toolbar Button Two", L"Success", MB_OK | MB_ICONINFORMATION);

		}

		break;

		}

		return 0;

	}

	break;



	case WM_SIZE:

		SendMessage(hWndToolBar, TB_AUTOSIZE, 0, 0);

		break;



	case WM_CLOSE:

		DestroyWindow(hWnd);

		break;



	case WM_DESTROY:

		PostQuitMessage(0);

		break;



	default:

		return (DefWindowProc(hWnd, Msg, wParam, lParam));

	}



	return 0;

}



INT WINAPI WinMain(HINSTANCE  hInstance,

	HINSTANCE  hPrevInstance,

	LPSTR      lpCmdLine,

	INT        nCmdShow)

{

	InitCommonControls();



	WNDCLASSEX    wc;



	wc.cbSize = sizeof(WNDCLASSEX);

	wc.style = 0;

	wc.lpfnWndProc = (WNDPROC)WndProc;

	wc.cbClsExtra = 0;

	wc.cbWndExtra = 0;

	wc.hInstance = hInstance;

	wc.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

	wc.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_ICON));

	wc.hCursor = LoadCursor(NULL, IDC_ARROW);

	wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);

	wc.lpszMenuName = NULL;

	wc.lpszClassName = LPCWSTR(ClassName);



	if (!RegisterClassEx(&wc))

	{

		MessageBox(NULL, L"Failed To Register The Window Class.", L"Error", MB_OK | MB_ICONERROR);

		return 0;

	}



	HWND    hWnd;



	hWnd = CreateWindowEx(

		WS_EX_CLIENTEDGE,

		LPCWSTR(ClassName),

		L"Toolbars",

		WS_OVERLAPPEDWINDOW,

		CW_USEDEFAULT,

		CW_USEDEFAULT,

		240,

		120,

		NULL,

		NULL,

		hInstance,

		NULL);



	if (!hWnd)

	{

		MessageBox(NULL, L"Window Creation Failed.", L"Error", MB_OK | MB_ICONERROR);

		return 0;

	}



	ShowWindow(hWnd, SW_SHOW);

	UpdateWindow(hWnd);



	MSG    Msg;



	while (GetMessage(&Msg, NULL, 0, 0))

	{

		TranslateMessage(&Msg);

		DispatchMessage(&Msg);

	}



	return Msg.wParam;

}