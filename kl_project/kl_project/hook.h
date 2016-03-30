
#include <windows.h>
#include <iostream>
#include <fstream>
#include <stdlib.h>
#include <tlhelp32.h>
#include <sys/stat.h> 

using namespace std;

class dllInjector
{
public:
	string _dllPath;
	int privileges();
	BOOL injectDll(string procName);
	dllInjector();

	DWORD getPid(string procName);
	int fexist(char *filename);
};

dllInjector::dllInjector()
{
	dllInjector::privileges();
}

int dllInjector::privileges()
{
	HANDLE Token;

	TOKEN_PRIVILEGES tp;
	if (OpenProcessToken(GetCurrentProcess(), TOKEN_ADJUST_PRIVILEGES | TOKEN_QUERY, &Token))
	{
		LookupPrivilegeValue(NULL, SE_DEBUG_NAME, &tp.Privileges[0].Luid);
		tp.PrivilegeCount = 1;
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		if (AdjustTokenPrivileges(Token, 0, &tp, sizeof(tp), NULL, NULL) == 0) {
			return 1; //FAIL
		}
		else
		{
			return 0; //SUCCESS
		}
	}
	return 1;
}

BOOL dllInjector::injectDll(string procName)
{
	char dll[MAX_PATH];

	strcpy_s(dll, _dllPath.c_str());
	if (fexist(dll) != 0) return false;
	DWORD id = getPid(procName);
	if (id == 0) return false;

	HANDLE p;
	p = OpenProcess(PROCESS_ALL_ACCESS, false, id);

	if (p == NULL) return false;

	LPVOID mem = VirtualAllocEx(p, NULL, sizeof(dll), MEM_COMMIT, PAGE_READWRITE);
	WriteProcessMemory(p, mem, dll, sizeof(dll), NULL);
	HANDLE thread = CreateRemoteThread(p, NULL, 0, (LPTHREAD_START_ROUTINE)GetProcAddress(GetModuleHandle("kernel32.dll"), "LoadLibraryA"), mem, 0, NULL);
	BOOL done;
	if (thread != 0)
	{
		done = false;
	}
	else
	{
		done = true;
	}
	CloseHandle(p);
	return done;
}

DWORD dllInjector::getPid(string procName)
{
	HANDLE hsnap;
	PROCESSENTRY32 pt;
	hsnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	pt.dwSize = sizeof(PROCESSENTRY32);
	do
	{
		if (pt.szExeFile == procName)
		{
			DWORD pid = pt.th32ProcessID;
			CloseHandle(hsnap);
			return pid;
		}
	} while (Process32Next(hsnap, &pt));
	CloseHandle(hsnap);
	return 0;
}

int dllInjector::fexist(char *filename)
{
	struct stat buffer;
	if (stat(filename, &buffer)) return 1;
	return 0;
}
