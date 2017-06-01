#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain(void)
{
	int nwCh;
	char AnsiStr[] = "hello, world!";
	wchar_t wszBuf[20] = { 0 };

	//获得转换后产生多少Unicode字符，可以作为后面实际转换时传入容纳转换结果的Unicode字符数buffer大小
	nwCh = MultiByteToWideChar(CP_ACP, 0, AnsiStr, -1, NULL, 0);
	//转换并接收结果
	MultiByteToWideChar(CP_ACP, 0, AnsiStr, -1, wszBuf, nwCh);
	wprintf(L"nwCh = %d, %s\n", nwCh, wszBuf);

	int nCh;
	char AnsiBuf[20] = { 0 };
	//获得转换后产生多少ANSI字符，可以作为后面实际转换时传入容纳转换结果的ANSI字符数buffer大小
	nCh = WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, NULL, 0, NULL, NULL);
	//转换并接收结果
	WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, AnsiBuf, nCh, NULL, NULL);
	printf("nCh = %d, %s\n", nCh, AnsiBuf);

	_tsystem(TEXT("pause"));

	return 0;
}