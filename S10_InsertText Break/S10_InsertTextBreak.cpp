//#include <windows.h>
#include "Header.h"

#define IDC_VTID  9876
const char *szAppName = "Caret demo";
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
//extern HWND CreateVirtualTerminal(HWND hWndParent, int left, int top, int width, int height, int id);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	HWND     hWnd;
	MSG      msg;
	WNDCLASSEX wndclass;

	wndclass.cbSize = sizeof(WNDCLASSEX);

	wndclass.style = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(WHITE_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = LPCWSTR(szAppName);
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);


	if (!RegisterClassEx(&wndclass))
	{
		MessageBox(NULL, TEXT("Maybe Fuckingly This program requires Windows NT!"), 
			TEXT("Maybe Fuckingly"), MB_ICONERROR);
		return 0;
	}

	hWnd = CreateWindowEx(
		0,
		LPCWSTR(szAppName),             // window class name
		L"theForger's Tutorial Application",            // window caption
		WS_OVERLAPPEDWINDOW,  // window style
		CW_USEDEFAULT,        // initial x position
		CW_USEDEFAULT,        // initial y position
		400,              // initial x size
		300,              // initial y size
		NULL,                 // parent window handle
		NULL,                 // window menu handle
		hInstance,            // program instance handle
		NULL);                // creation parameters

	 
	if (hWnd == NULL)
	{
		MessageBox(NULL, L"Window Creation Failed!", L"Error!",
			MB_ICONEXCLAMATION | MB_OK);
		return 0;
	}


	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

  LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC         hDC;
	PAINTSTRUCT ps;

	switch (message)
	{
	case WM_CREATE:
		CreateVirtualTerminal(hWnd, 10, 10, 360, 240, IDC_VTID);
		if (hWnd == NULL)
			MessageBox(hWnd, L"Could not create edit box.", L"Error", MB_OK | MB_ICONERROR);
		return 0;

	case WM_PAINT:
		hDC = BeginPaint(hWnd, &ps);
		;
		EndPaint(hWnd, &ps);
		return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}