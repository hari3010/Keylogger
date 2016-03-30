// dllmain.cpp : Defines the entry point for the DLL application.

#include <stdio.h>
#include "stdafx.h"
#include <Shlwapi.h>

#define TIMER_VAL 105

typedef int(*sendfile)(void);

DWORD dwThread = NULL;
DWORD WINAPI MyThreadFunction(LPVOID lpParam)
{
	while (true)
	{
		sendfile fsendfile = NULL;
		HINSTANCE h_dll = NULL;

		//MessageBoxA(NULL, "Inside Time Proc", "", NULL);

		if (PathIsDirectoryEmptyA("G:\\ProjectFiles\\tosend"))
		{
			//MessageBoxA(NULL, "FILE NOT FOUND", "", NULL);
			return 0;
		}

		h_dll = LoadLibrary(L"G:\\ProjectFiles\\CSmtp.dll");
		if (h_dll == NULL)
		{
			printf("\n\ndll NOT loaded\n\n");
			return 0;
		}


		fsendfile = (sendfile)GetProcAddress(h_dll, "sendfile");
		if (fsendfile == NULL)
		{
			FreeLibrary(h_dll);
			return 0; 
		}
		if (fsendfile())
		{
			MessageBoxA(NULL, "FILE NOT SENT", "", NULL);
		}
		else
		{
			MessageBoxA(NULL, "FILE SENT", "", NULL);
		}
		DeleteFileA("G:\\ProjectFiles\\tosend\\log.txt");
		FreeLibrary(h_dll);
		Sleep(30*1000);

		
	}
	return 0;
}
BOOL APIENTRY DllMain( HMODULE hModule,
                       DWORD  ul_reason_for_call,
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	{
		HANDLE threadh = NULL;
		threadh = CreateThread(
			NULL,
			0,
			MyThreadFunction,
			NULL,
			0,
			&dwThread);

		if (threadh != NULL)
		{
			//MessageBoxA(NULL, "threadh created", "***", NULL);
		}
		else
		{
			//MessageBoxA(NULL, "threadh failed", "In my thread", NULL);
		}
	}
	case DLL_THREAD_ATTACH:
		break;
	case DLL_THREAD_DETACH:
		break;
	case DLL_PROCESS_DETACH:
		//MessageBoxA(NULL, "IN dll helper stop", "", NULL);
		ExitThread(dwThread);
		break;
	}
	return TRUE;
}

