#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <string>
#include "windows.h"

#define XOR_KEY 55

#define ENCODED_KEY "\x45\xC2\x2B\xB9\xAF\x5D\xBB\xAE\x19\x0B\x1B\x74\x51\xE0\x5E\x89\x05\x28\xA5\x84"
#define ENCODED_MESSAGE "\x44\x21\x56\x50\x2d\x07\x40\x4c\x7e\x16\x11\x45\x1b\x2f\x51\x39\x69\x33\x63\x20\x46\x41\x07"

#define CALL_FUNC(func, dll_name) GetProcAddress(LoadLibraryA(dll_name), func)

#define FIRST_CHAR_MAGIC (0xc4)
#define CHAR_ADDON_POW (5)

std::string decrypt_ip(const std::string& key);

// Takes two numbers, and outputs their greatest common denominator
int32_t gcd(int32_t a, int32_t b);

// Take two numbers, and outputs their least common multiplier
int32_t lcm(int32_t a, int32_t b);

// Old encoding, xor XOR_KEY
std::string encrypt_string(const std::string& input_string);

// New encoding, with new algorithm by @Gil and @Nitzan, takes a string and output a new, fancy, encoded string
std::string encode_string(const std::string& input_string);

// If the __cpuid symbol is not defined, there is a call to cpuid with the right args
void __cpuid(int CPUInfo[4], int InfoType);

#endif /*_HELPERS_H_*/