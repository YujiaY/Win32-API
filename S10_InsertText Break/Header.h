#pragma once
#include <windows.h>

static TCHAR szWinName[] = TEXT("VirtualTerminal");
#define TEXTMATRIX(x, y)  *(pTextMatrix + ((y) * nLineChars) + (x))
static TEXTMETRIC tm; //metric dimention of virtual terminal
static TCHAR *pTextMatrix; //VT character buffer
static int nCharWidth, nCharHeight; //the width and height of characeters
static int nCaretPosX, nCaretPosY; //the current position of caret
static int nVTWidth, nVTHeight; //the width and height of virtual terminal window
static int nLineChars, nRowChars; //character number in one line and row
static int nCaretOffsetY; //the offset of vertical height
static COLORREF TextColor; //text color for terminal window, that is fore color

static LRESULT CALLBACK VTWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

HWND CreateVirtualTerminal(HWND hWndParent, int left, int top, int width, int height, int id)
{
	HWND hWnd;
	WNDCLASS wndclass;

	HINSTANCE hInst = GetModuleHandle(NULL);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = VTWndProc;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.hInstance = hInst;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszMenuName = NULL;
	wndclass.lpszClassName = szWinName;
	if (!RegisterClass(&wndclass))
	{
		return 0;
	}
	hWnd = CreateWindowEx(0, szWinName, NULL, WS_CHILD | WS_VISIBLE,
		left, top, width, height, hWndParent, (HMENU)id, hInst, NULL);
	return hWnd;
}

static void DrawChar(HDC hDC, int x, int y, TCHAR *str, int num)
{
	RECT rect;
	SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
	SetTextColor(hDC, TextColor);
	SetBkMode(hDC, TRANSPARENT);
	rect.left = x;
	rect.top = y;
	rect.right = x + num * nCharWidth;
	rect.bottom = y + nCharHeight;
	FillRect(hDC, &rect, (HBRUSH)GetStockObject(BLACK_BRUSH));
	TextOut(hDC, nCaretPosX * nCharWidth, nCaretPosY * nCharHeight,
		&TEXTMATRIX(nCaretPosX, nCaretPosY), nLineChars - nCaretPosX);
}

static LRESULT CALLBACK VTWndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int x, y;
	HDC hDC;

	switch (message)
	{
	case WM_CREATE:
		hDC = GetDC(hWnd);
		SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
		GetTextMetrics(hDC, &tm);
		ReleaseDC(hWnd, hDC);
		nCharWidth = tm.tmAveCharWidth;
		nCharHeight = tm.tmHeight;
		TextColor = RGB(255, 255, 255);
		nCaretOffsetY = 12;
		return 0;

	case WM_SIZE:
		nVTWidth = LOWORD(lParam);
		nLineChars = max(1, nVTWidth / nCharWidth);
		nVTHeight = HIWORD(lParam);
		nRowChars = max(1, nVTHeight / nCharHeight);

		if (pTextMatrix != NULL)
		{
			free(pTextMatrix);
		}
		pTextMatrix = (TCHAR *)malloc(nLineChars * nRowChars);
		if (pTextMatrix)
		{
			for (y = 0; y<nRowChars; y++)
			{
				for (x = 0; x<nLineChars; x++)
				{
					TEXTMATRIX(x, y) = TEXT(' ');
				}
			}
		}
		SetCaretPos(0, nCaretOffsetY);
		return 0;

	case WM_LBUTTONDOWN:
		SetFocus(hWnd);
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_HOME:       // Home 
			nCaretPosX = 0;
			break;

		case VK_END:        // End 
			nCaretPosX = nLineChars - 1;
			break;

		case VK_PRIOR:      // Page Up 
			nCaretPosY = 0;
			break;

		case VK_NEXT:       // Page Down 
			nCaretPosY = nRowChars - 1;
			break;

		case VK_LEFT:       // Left arrow 
			nCaretPosX = max(nCaretPosX - 1, 0);
			break;

		case VK_RIGHT:      // Right arrow 
			nCaretPosX = min(nCaretPosX + 1,
				nLineChars - 1);
			break;

		case VK_UP:         // Up arrow 
			nCaretPosY = max(nCaretPosY - 1, 0);
			break;

		case VK_DOWN:       // Down arrow 
			nCaretPosY = min(nCaretPosY + 1,
				nRowChars - 1);
			break;

		case VK_DELETE:     // Delete 
							// Move all the characters that followed the 
							// deleted character (on the same line) one 
							// space back (to the left) in the matrix. 
			for (x = nCaretPosX; x < nLineChars; x++)
				TEXTMATRIX(x, nCaretPosY) = TEXTMATRIX(x + 1, nCaretPosY);
			// Replace the last character on the 
			// line with a space. 
			TEXTMATRIX(nLineChars - 1, nCaretPosY) = ' ';
			// The application will draw outside the 
			// WM_PAINT message processing, so hide the caret. 
			HideCaret(hWnd);
			// Redraw the line, adjusted for the 
			// deleted character. 
			hDC = GetDC(hWnd);
			DrawChar(hDC, nCaretPosX * nCharWidth, nCaretPosY * nCharHeight,
				&TEXTMATRIX(nCaretPosX, nCaretPosY), nLineChars - nCaretPosX / nCharWidth);
			ReleaseDC(hWnd, hDC);

			// Display the caret. 
			ShowCaret(hWnd);
			break;
		}
		// Adjust the caret position based on the 
		// virtual-key processing. 
		SetCaretPos(nCaretPosX * nCharWidth, nCaretPosY * nCharHeight + nCaretOffsetY);
		return 0;

	case WM_SHOWWINDOW:
		SetFocus(hWnd);
		break;

	case WM_SETFOCUS:
		CreateCaret(hWnd, NULL, nCharWidth, 2);
		SetCaretPos(nCaretPosX * nCharWidth, nCaretPosY * nCharHeight + nCaretOffsetY);
		ShowCaret(hWnd);
		break;

	case WM_KILLFOCUS:
	case WM_DESTROY:
		HideCaret(hWnd);
		DestroyCaret();
		break;

	case WM_CHAR:
		switch (wParam)
		{
		case 0x08: //process a backspace.
			if (nCaretPosX > 0)
			{
				nCaretPosX--;
				SendMessage(hWnd, WM_KEYDOWN, VK_DELETE, 1L);
			}
			break;
		case 0x09: //process a tab.
			do
			{
				SendMessage(hWnd, WM_CHAR, TEXT(' '), 2L);
			} while (nCaretPosX % 4 != 0);
			break;
		case 0x0D: //process a carriage return.
			nCaretPosX = 0;
			if (++nCaretPosY == nRowChars)
			{
				nCaretPosY = 0;
			}
			break;
		case 0x1B:
		case 0x0A: //process a linefeed.
			MessageBeep((UINT)-1);
			break;
		default:
			TEXTMATRIX(nCaretPosX, nCaretPosY) = (TCHAR)wParam;
			HideCaret(hWnd);
			hDC = GetDC(hWnd);
			DrawChar(hDC, nCaretPosX * nCharWidth, nCaretPosY * nCharHeight, &TEXTMATRIX(nCaretPosX, nCaretPosY), 1);
			ReleaseDC(hWnd, hDC);
			ShowCaret(hWnd);
			if (++nCaretPosX == nLineChars)
			{
				nCaretPosX = 0;
				if (++nCaretPosY == nRowChars)
				{
					nCaretPosY = 0;
				}
			}
			break;
		}
		SetCaretPos(nCaretPosX * nCharWidth, nCaretPosY * nCharHeight + nCaretOffsetY);
		return 0;

	case WM_PAINT:
	{
		PAINTSTRUCT ps;
		hDC = BeginPaint(hWnd, &ps);
		SelectObject(hDC, GetStockObject(SYSTEM_FIXED_FONT));
		SetBkMode(hDC, TRANSPARENT);
		SetTextColor(hDC, TextColor);
		for (y = 0; y<nLineChars; y++)
		{
			TextOut(hDC, 0, y * nCharHeight, &TEXTMATRIX(0, y), nLineChars);
		}
		EndPaint(hWnd, &ps);
	}
	return 0;
	default:
		break;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}