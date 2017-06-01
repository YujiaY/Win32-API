#include <windows.h>
#include <tchar.h>
#include <stdio.h>

int _tmain(void)
{
	int nwCh;
	char AnsiStr[] = "hello, world!";
	wchar_t wszBuf[20] = { 0 };

	//���ת�����������Unicode�ַ���������Ϊ����ʵ��ת��ʱ��������ת�������Unicode�ַ���buffer��С
	nwCh = MultiByteToWideChar(CP_ACP, 0, AnsiStr, -1, NULL, 0);
	//ת�������ս��
	MultiByteToWideChar(CP_ACP, 0, AnsiStr, -1, wszBuf, nwCh);
	wprintf(L"nwCh = %d, %s\n", nwCh, wszBuf);

	int nCh;
	char AnsiBuf[20] = { 0 };
	//���ת�����������ANSI�ַ���������Ϊ����ʵ��ת��ʱ��������ת�������ANSI�ַ���buffer��С
	nCh = WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, NULL, 0, NULL, NULL);
	//ת�������ս��
	WideCharToMultiByte(CP_ACP, 0, wszBuf, -1, AnsiBuf, nCh, NULL, NULL);
	printf("nCh = %d, %s\n", nCh, AnsiBuf);

	_tsystem(TEXT("pause"));

	return 0;
}