#include <cmath> 

#include "Helpers.hpp"

std::string encrypt_string(const std::string& input_string) 
{
	std::string output_string = "";

	for (const char& current_char : input_string)
	{
		output_string += static_cast<char>((current_char ^ XOR_KEY));
	}

	return output_string;
}

std::string encode_string(const std::string& input_string)
{
	std::string output_string = "";
	char last_char = FIRST_CHAR_MAGIC;
	char current_char_encoded;
	auto current_char = input_string.begin();

	for (int index = 0; current_char < input_string.end(); current_char++, index++)
	{
		current_char_encoded = *current_char ^ (lcm(index + 1, (int)pow(last_char, 2) + CHAR_ADDON_POW) % UINT8_MAX);
		output_string += current_char_encoded;
		last_char = current_char_encoded;

		//printf("\\x%02hhX", current_char_encoded);
	}

	return output_string;
}

std::string decrypt_ip(const std::string& key)
{
	std::string ip = "", encrypted_ip = ENCODED_MESSAGE;
	auto it = encrypted_ip.begin();
	size_t key_index = 0;

	for (int index = 0; it < encrypted_ip.end(); it++, index++)
	{
		key_index = index % key.length();
		ip += *it ^ key[key_index];
	}

	return ip;
}

int32_t gcd(int32_t a, int32_t b)
{
	if (b == 0)
	{
		return a;
	}

	return gcd(b, a % b);
}

int32_t lcm(int32_t a, int32_t b)
{
	return (a / gcd(a, b)) * b;
}

void __cpuid(int CPUInfo[4], int InfoType)
{
	__asm
	{
		mov    esi, CPUInfo
		mov    eax, InfoType
		xor ecx, ecx
		cpuid
		mov    dword ptr[esi + 0], eax
		mov    dword ptr[esi + 4], ebx
		mov    dword ptr[esi + 8], ecx
		mov    dword ptr[esi + 12], edx
	}
}
