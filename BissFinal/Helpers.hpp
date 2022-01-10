#ifndef _HELPERS_H_
#define _HELPERS_H_

#include <string>

#define XOR_KEY 55

#define ENCODED_KEY "\x5a\xa5\x12\x45\x47\x96\xfe\x7b\x4c\xdf\x05\x4e\x34\x59\x59\xda\xc6\x29\xae\xa5\x46\x0c\x41\x6b\xd1"
#define ENCODED_IP "\x2d\x5c\x09\x4c\x5d\x2f\x4d\x5c\x0e\x4b\x59\x0b\x7f\x05\x02\x52\x2c\x09\x43\x10\x56\x53\x11\x58\x52\x59\x05\x55\x5e\x1a\x6e\x5b\x0b\x46\x5a\x1a\x47"

#define FIRST_CHAR_MAGIC (0xc4)
#define CHAR_ADDON_POW (5)

std::string decrypt_ip(const std::string& key);

// Takes two numbers, and outputs their greatest common denominator
long long gcd(long long int a, long long int b);

// Take two numbers, and outputs their least common multiplier
long long lcm(long long int a, long long int b);

// Old encoding, xor XOR_KEY
std::string encrypt_string(const std::string& input_string);

// New encoding, with new algorithm by @Gil and @Nitzan, takes a string and output a new, fancy, encoded string
std::string encode_string(const std::string& input_string);

#endif /*_HELPERS_H_*/