#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <string>
#include "windows.h"

#define XOR_KEY 55

//#define ENCODED_KEY "\x5a\xa5\x12\x45\x47\x96\xfe\x7b\x4c\xdf\x05\x4e\x34\x59\x59\xda\xc6\x29\xae\xa5\x46\x0c\x41\x6b\xd1"
#define ENCODED_KEY "\x45\xc1\x2b\xb8\xae\x5d\xba\xad\x19\x0b\x1b\x74\x51\xdf\x5e\x88\x05\x28\xa4\x83"
//#define ENCODED_IP "\x2a\x71\x2f\x3e\x71\x0b\x4d\x78\x2d\x32\x14\x30\x1e\x25\x7b\x36\x10\x23\x63\x46\x7c\x67\x11\x75\x6e\x36\x79\x66\x2f\x5b\x31\x0a\x41\x09\x1f\x47\x5e\x7f\x15\x5c\x0b\x2b\x04\x54\x14\x5a\x4f\x11\x53\x4e\x00\x18\x4c\x18\x55\x33\x06\x13\x1c\x04\x14\x13\x7f\x05\x5f\x1f\x32\x0d\x5f\x02\x56\x53"
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