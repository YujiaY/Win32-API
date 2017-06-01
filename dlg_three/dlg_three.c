#include <windows.h>

#include "resource.h" 

HBRUSH g_hbrBackground = NULL;
HBRUSH g_btnBackground = NULL;

BOOL CALLBACK DlgProc(HWND hwnd, UINT Message, WPARAM wParam, LPARAM lParam)
{
	switch(Message)
	{
		case WM_INITDIALOG:
			g_hbrBackground = CreateSolidBrush(RGB(213, 123, 44));
			g_btnBackground = CreateSolidBrush(RGB(200, 223, 244));

			SendMessage(hwnd, WM_SETICON, ICON_SMALL, (LPARAM)LoadIcon(NULL, 
				MAKEINTRESOURCE(IDI_APPLICATION)));
			SendMessage(hwnd, WM_SETICON, ICON_BIG, (LPARAM)LoadIcon(NULL, 
				MAKEINTRESOURCE(IDI_APPLICATION)));
		break;

		case WM_CLOSE:
			EndDialog(hwnd, 0);
		break;
		case WM_CTLCOLORDLG:	
			return (LONG)g_hbrBackground;
		case WM_CTLCOLORBTN:
			return (LONG)g_btnBackground;

		case WM_CTLCOLORSTATIC:
		{
			HDC hdcStatic = (HDC)wParam;
			SetTextColor(hdcStatic, RGB(255, 255, 255));
			SetBkMode(hdcStatic, TRANSPARENT);
			return (LONG)g_hbrBackground;
			return (LONG)g_btnBackground;
		}
		break;
		case WM_COMMAND:
			switch(LOWORD(wParam))
			{
				case IDOK:
					EndDialog(hwnd, 0);
				break;
			}
		break;
		case WM_DESTROY:
			DeleteObject(g_hbrBackground);
			DeleteObject(g_btnBackground);
		break;
		default:
			return FALSE;
	}
	return TRUE;
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance,
	LPSTR lpCmdLine, int nCmdShow)
{
	return DialogBox(hInstance, MAKEINTRESOURCE(IDD_MAIN), NULL, DlgProc);
}
