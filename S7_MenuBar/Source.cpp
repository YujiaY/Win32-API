#include <windows.h>


#define IDM_FILE_NEW   1001
#define IDM_FILE_OPEN  1002
#define IDM_FILE_SAVE  1003

#define IDM_EDIT_COPY  1004
#define IDM_EDIT_PASTE 1005
#define IDM_EDIT_HL    1006

#define IDM_VIEW_FULL  1007
#define IDM_VIEW_HALF  1008
#define IDM_VIEW_PART  1009

#define IDM_FILE_OPEN_SOLUTION 10021
#define IDM_FILE_OPEN_PROJECT  10022

static TCHAR szAppName[] = TEXT("Menubar");
static HBITMAP hbmpCheck;   // handle to checked bitmap    
static HBITMAP hbmpUncheck; // handle to unchecked bitmap  
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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
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
		NULL,                 // parent window handle
		NULL,                 // window menu handle
		hInstance,            // program instance handle
		NULL);                // creation parameters

	ShowWindow(hWnd, iCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}

HMENU CreateMenuBar(void)
{
	//�ܲ˵�
	HMENU hMenu = CreateMenu();

	//�ļ��˵�
	HMENU hFileMenu = CreateMenu();
	hbmpCheck = (HBITMAP)LoadImage(GetModuleHandle(NULL), TEXT("1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	hbmpUncheck = (HBITMAP)LoadImage(GetModuleHandle(NULL), TEXT("2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	AppendMenu(hFileMenu, MF_STRING, IDM_FILE_NEW, TEXT("&New"));
	SetMenuItemBitmaps(hFileMenu, 0, MF_BYPOSITION, hbmpUncheck, hbmpCheck);
	AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL); //����һ���������뿴����Ч��
	AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL); //����һ���������뿴����Ч��
	AppendMenu(hFileMenu, MF_STRING, IDM_FILE_SAVE, TEXT("&Save"));
	
	AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL); //����һ���������뿴����Ч��
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, TEXT("File(&F)"));
	AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL); //����һ���������뿴����Ч��
	
	//�ļ��Ķ����Ӳ˵�
	HMENU hSubMenu = CreatePopupMenu();
	AppendMenu(hSubMenu, MF_STRING, IDM_FILE_OPEN_SOLUTION, TEXT("So&lution"));
	SetMenuItemBitmaps(hSubMenu, 0, MF_BYPOSITION, hbmpUncheck, hbmpCheck);
	AppendMenu(hSubMenu, MF_STRING, IDM_FILE_OPEN_PROJECT, TEXT("Pro&ject"));
	SetMenuItemBitmaps(hSubMenu, 1, MF_BYPOSITION, hbmpUncheck, hbmpCheck);

	//���ö����˵����뵽�ڶ�����λ��
	InsertMenu(hFileMenu, 1, MF_BYPOSITION | MF_POPUP, (UINT_PTR)hSubMenu, TEXT("Open"));
	SetMenuItemBitmaps(hFileMenu, 1, MF_BYPOSITION, hbmpUncheck, hbmpCheck);
	SetMenuItemBitmaps(hFileMenu, 2, MF_BYPOSITION, hbmpUncheck, hbmpCheck);
	SetMenuItemBitmaps(hFileMenu, 4, MF_BYPOSITION, hbmpUncheck, hbmpCheck); //Why 4?!?! For the "save"

	//�༭�˵�
	hFileMenu = CreateMenu();
	//hbmpCheck = (HBITMAP)LoadImage(GetModuleHandle(NULL), TEXT("1.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	//hbmpUncheck = (HBITMAP)LoadImage(GetModuleHandle(NULL), TEXT("2.bmp"), IMAGE_BITMAP, 0, 0, LR_LOADFROMFILE);
	AppendMenu(hFileMenu, MF_STRING | MF_UNCHECKED, IDM_EDIT_COPY, TEXT("&Copy"));
	AppendMenu(hFileMenu, MF_STRING, IDM_EDIT_PASTE, TEXT("&Paste"));
	SetMenuItemBitmaps(hFileMenu, 0, MF_BYPOSITION, hbmpUncheck, hbmpCheck);
	SetMenuItemBitmaps(hFileMenu, 1, MF_BYPOSITION, hbmpCheck, hbmpUncheck);
	AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
	AppendMenu(hFileMenu, MF_STRING | MF_CHECKED, IDM_EDIT_HL, TEXT("&Update"));//����һ��checkѡ��
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, TEXT("Edit(&E)"));

	//���Ų˵�
	hFileMenu = CreateMenu();
	AppendMenu(hFileMenu, MF_STRING, IDM_VIEW_HALF, TEXT("&Half"));
	AppendMenu(hFileMenu, MF_SEPARATOR, 0, NULL);
	//����һ����ɫ����ѡ�Ĳ˵����ò˵�������EnableMenuItem�����޸Ŀ�ѡ״̬
	AppendMenu(hFileMenu, MF_STRING | MF_GRAYED, IDM_VIEW_PART, TEXT("P&art"));
	AppendMenu(hMenu, MF_POPUP, (UINT_PTR)hFileMenu, TEXT("Zoom(&Z)"));
	return hMenu;
}

static LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int ItemID;

	switch (message)
	{
	case WM_CREATE:
	{
		HMENU hMenu = CreateMenuBar();
		SetMenu(hWnd, hMenu); //����ֻ�Ǵ����˲˵�����Ҫ����
	}
	return 0;

	case WM_RBUTTONUP:
	{
		POINT point;
		point.x = LOWORD(lParam);
		point.y = HIWORD(lParam);
		ClientToScreen(hWnd, &point); //������������������Ļ�ģ���Ҫת��Ϊ�ͻ�����
		HMENU hSubMenu = GetSubMenu(GetMenu(hWnd), 0); //��ȡ�˵��ĵ�0���Ӳ˵���������˵�����ʾ�����˵�
		TrackPopupMenu(hSubMenu, TPM_RIGHTBUTTON, point.x, point.y, 0, hWnd, NULL);
	}
	return 0;

	case WM_COMMAND:
		ItemID = LOWORD(wParam);
		switch (ItemID)
		{
		case IDM_FILE_NEW:
			MessageBox(hWnd, TEXT("you click new file button"), TEXT("hint"), MB_OK);
			break;
		case IDM_FILE_OPEN_SOLUTION:
			MessageBox(hWnd, TEXT("you click Open Solution button"), TEXT("hint"), MB_OK);
			break;
		case IDM_FILE_OPEN_PROJECT:
			MessageBox(hWnd, TEXT("you click Open Project button"), TEXT("hint"), MB_OK);
			break;
		case IDM_FILE_SAVE:
			MessageBox(hWnd, TEXT("you click new file button"), TEXT("hint"), MB_OK);
			break;
 
		case IDM_EDIT_HL:
		case IDM_EDIT_COPY:
		case IDM_EDIT_PASTE:
		{
			MENUITEMINFO mii = { 0 };
			mii.cbSize = sizeof(mii);
			mii.fMask = MIIM_STATE;
			HMENU hSubMenu = GetSubMenu(GetMenu(hWnd), 1);
			GetMenuItemInfo(hSubMenu, ItemID, FALSE, &mii);
			if (mii.fState & MFS_CHECKED)
			{
				mii.fState &= ~MFS_CHECKED;
			}
			else
			{
				mii.fState |= MFS_CHECKED;
			}
			SetMenuItemInfo(hSubMenu, ItemID, FALSE, &mii);
		}
		break;
		default:
			break;
		}
		return 0;

	case WM_DESTROY:
		DeleteObject(hbmpCheck);
		DeleteObject(hbmpUncheck);
		PostQuitMessage(0);
		return 0;
	}

	return DefWindowProc(hWnd, message, wParam, lParam);
}