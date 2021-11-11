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

std::string encode_string(const std::string & input_string)
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

std::string decrypt_ip(std::string key)
{
	std::string ip = "", encrypted_ip = ENCODED_IP;
	auto it = encrypted_ip.begin();

	for (int index = 0; it < encrypted_ip.end(); it++, index++)
	{
		ip += *it ^ key[index % key.length()];
	}

	return ip;
}

long long gcd(long long int a, long long int b)
{
	if (b == 0)
		return a;

	return gcd(b, a % b);
}

long long lcm(long long int a, long long int b)
{
	return (a / gcd(a, b)) * b;
}
