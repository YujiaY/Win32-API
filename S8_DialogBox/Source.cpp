#include <windows.h>
#include <tchar.h>
#include <commdlg.h>
#include <stdio.h> 
#include <string.h>




#pragma comment(linker,"\"/manifestdependency:type='win32' "\
			"name='Microsoft.Windows.Common-Controls' "\
			"version='6.0.0.0' processorArchitecture='*' "\
			"publicKeyToken='6595b64144ccf1df' language='*'\"")

#define IDC_FILE_DIALOG  1001
#define IDC_COLOR_DIALOG 1002
#define IDC_FONT_DIALOG  1003
#define IDC_SAVE_DIALOG  1004

static TCHAR szAppName[] = TEXT("CommonDialog");
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, PSTR szCmdLine, int iCmdShow)
{
	HWND     hWnd;
	MSG      msg;
	WNDCLASS wndclass;

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
		MessageBox(NULL, TEXT("This program requires Windows NT!"), szAppName, MB_ICONERROR);
		return 0;
	}

	hWnd = CreateWindow(szAppName,             // window class name
		szAppName,            // window caption
		WS_OVERLAPPEDWINDOW,  // window style
		CW_USEDEFAULT,        // initial x position
		CW_USEDEFAULT,        // initial y position
		400,              // initial x size
		300,              // initial y size
		NULL,             // parent window handle
		NULL,             // window menu handle
		hInstance,        // program instance handle
		NULL);            // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

//
 
//


static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	HDC hDC;

	switch (message)
	{
	case WM_CREATE:
		CreateWindow(TEXT("BUTTON"), TEXT("file dialog"), WS_CHILD | WS_VISIBLE, 20, 25, 160, 40, hWnd, (HMENU)IDC_FILE_DIALOG, NULL, NULL);
		CreateWindow(TEXT("BUTTON"), TEXT("color dialog"), WS_CHILD | WS_VISIBLE, 20, 85, 160, 40, hWnd, (HMENU)IDC_COLOR_DIALOG, NULL, NULL);
		CreateWindow(TEXT("BUTTON"), TEXT("font dialog"), WS_CHILD | WS_VISIBLE, 20, 145, 160, 40, hWnd, (HMENU)IDC_FONT_DIALOG, NULL, NULL);
		CreateWindow(TEXT("BUTTON"), TEXT("save dialog"), WS_CHILD | WS_VISIBLE, 20, 205, 160, 40, hWnd, (HMENU)IDC_SAVE_DIALOG, NULL, NULL);
		return 0;

	case WM_COMMAND:
	{
		int wmId = LOWORD(wParam);
		switch (wmId)
		{
		case IDC_FILE_DIALOG:
		{
			OPENFILENAME ofn;
			TCHAR ext[] = TEXT(".dat");
			TCHAR szFile[MAX_PATH] = TEXT("default");
			TCHAR szFilter[] = TEXT("文本文件 (*.txt)\0*.txt\0")
				TEXT("数据文件 (*.dat)\0*.dat\0")
				TEXT("All Files (*.*)\0*.*\0\0");

			ZeroMemory(&ofn, sizeof(ofn));
			ofn.lStructSize = sizeof(ofn);
			ofn.hwndOwner = hWnd;
			ofn.lpstrFilter = szFilter;
			ofn.nFilterIndex = 1;		  // 1 to default show *.txt type file; 2 to default show *.dat type file.
			ofn.lpstrFile = szFile;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = ext;
			ofn.Flags = OFN_PATHMUSTEXIST | OFN_OVERWRITEPROMPT;
			if (GetOpenFileName(&ofn))//GetOpenFileName, GetSaveFileName
			{
				//将选择的文件输出到窗口
				LPTSTR file = ofn.lpstrFile; //file就是保存或者打开的文件名
				hDC = GetDC(hWnd);
				TextOut(hDC, 200, 50, ofn.lpstrFile, _tcslen(ofn.lpstrFile));
				ReleaseDC(hWnd, hDC);
			}
		}
		break;
		case IDC_SAVE_DIALOG:
		{
			 
			OPENFILENAME ofn;
			WCHAR szFileName[MAX_PATH] = L"";
			BOOL bSuccess = FALSE;

			ZeroMemory(&ofn, sizeof(ofn));

			ofn.lStructSize = sizeof(OPENFILENAME);
			ofn.hwndOwner = NULL;
			ofn.lpstrFilter = L"TXT stupid Files (*.txt)\0*.txt\0PDF Smart File (*.pdf)\0*.pdf\0All silly Files (*.*)\0*.*\0";
			ofn.lpstrFile = szFileName;
			ofn.nMaxFile = MAX_PATH;
			ofn.lpstrDefExt = L"txt";
			ofn.Flags = OFN_EXPLORER | OFN_PATHMUSTEXIST | OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT;
			HANDLE hFile;
			if (GetSaveFileName(&ofn))

			{
				//printf("the path is : %s\n", ofn.lpstrFile);

				//wchar_t *hEdit = L"FuckIT?";
				WCHAR *pToMyString = L"Fucking with IT?1 Fucking with IT?2 Fucking with IT?3";
				size_t len = _tcslen(pToMyString);
				
				hFile = CreateFile(szFileName, GENERIC_WRITE, 0, NULL,
					CREATE_NEW|OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, NULL);
				DWORD dwWritten;
				SetFilePointer(hFile, 0, NULL, FILE_END);
				WCHAR crlf[] = L"\r\n";
				WriteFile(hFile, crlf, _tcslen(crlf)*2, &dwWritten, NULL);
				WriteFile(hFile, pToMyString, len*2, &dwWritten, NULL);
				/*
				//If you want to create text lines, you also need to output CR+LF characters at the end of each line 
				SetFilePointer(hFile, 0, NULL, FILE_END);
				char crlf[] = "\r\n";
				WriteFile(hFile, crlf, sizeof(crlf), &dwWritten, 0);
				//End of new line
			 
				SetFilePointer(hFile, -1, NULL, FILE_END);
				WriteFile(hFile, pToMyString, len, &dwWritten, NULL);
				*/
				/*
				if (hFile != INVALID_HANDLE_VALUE)
				{
					DWORD dwTextLength;

					dwTextLength = _tcslen(hEdit);
					// No need to bother if there's no text.
					if (dwTextLength > 0)
					{
						LPSTR pszText;
						DWORD dwBufferSize = dwTextLength + 1;

						pszText = (LPSTR)GlobalAlloc(GPTR, dwBufferSize);
						if (pszText != NULL)
						{
							if (GetWindowText(hWnd, szFileName, dwBufferSize))
							{
								DWORD dwWritten;

								if (WriteFile(hFile, pszText, dwTextLength, &dwWritten, NULL))
									bSuccess = TRUE;
							}
							GlobalFree(pszText);
						}
					}

				}*/
				CloseHandle(hFile);
			}
			 

		}break;

		case IDC_COLOR_DIALOG:
		{
			CHOOSECOLOR cc;
			static COLORREF acrCustClr[16];

			ZeroMemory(&cc, sizeof(cc));
			cc.lStructSize = sizeof(cc);
			cc.hwndOwner = hWnd;
			cc.lpCustColors = (LPDWORD)acrCustClr;
			if (ChooseColor(&cc))
			{
				//用选择的颜色画一个实心矩形
				RECT rect = { 240, 100, 340, 140 };
				hDC = GetDC(hWnd);
				HBRUSH hBrush = CreateSolidBrush(cc.rgbResult);
				SelectObject(hDC, hBrush);
				FillRect(hDC, &rect, hBrush);
				DeleteObject(hBrush);
				ReleaseDC(hWnd, hDC);
			}
		}
		break;
		

		case IDC_FONT_DIALOG:
		{
			CHOOSEFONT cf;
			LOGFONT lf;
			ZeroMemory(&cf, sizeof(cf));
			cf.lStructSize = sizeof(cf);
			cf.hwndOwner = hWnd;
			cf.lpLogFont = &lf;
			cf.Flags = CF_SCREENFONTS | CF_EFFECTS;
			if (ChooseFont(&cf))
			{
				//用选择的字体输出一行测试文本
				TCHAR str[] = TEXT("Font 测试！");
				HFONT hFont = CreateFontIndirect(cf.lpLogFont);
				DWORD rgbCurrent = cf.rgbColors;
				hDC = GetDC(hWnd);
				SelectObject(hDC, hFont);
				SetTextColor(hDC, cf.rgbColors);
				TextOut(hDC, 200, 170, str, _tcslen(str));
				DeleteObject(hFont);
				ReleaseDC(hWnd, hDC);
			}
		}
		break;

		default:
			break;
		}
	}
	return 0;

	case WM_DESTROY:
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}