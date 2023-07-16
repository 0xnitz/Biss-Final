import random
import string


class Crypto:
    def __init__(self):
        self.key = "iran_was_here____123123123123"  # change to your key
        self.keep_alive = "this is keep alive close to the key"  # change to your keep alive
        self.last_encrypted_msg = ''.join(random.choice(string.printable) for _ in range(len(self.key)))

    def encrypt_msg(self, msg_plaintext):
        msg_plaintext_with_padding = Crypto.dup(msg_plaintext, len(self.key))
        encrypted = Crypto.xor(msg_plaintext_with_padding, self.last_encrypted_msg)
        encrypted = Crypto.xor(encrypted, self.key)
        self.last_encrypted_msg = encrypted
        # print(encrypted)
        return encrypted

    @staticmethod
    def dup(s, x):
        """
        input: key, len of file
        the func Duplicates the key that it will be at the same lengh as the file
        exmple: file len = 10, key="om" return- omomomomom
        output: Duplicate the key
        """
        return (s * (int(x / len(s)) + x))[:x]

    @staticmethod
    def xor(msg, key):
        """
        xor between msg and key, dup the key by len of msg
        """
        # new_key_len = len(msg)
        # key = (key * (int(new_key_len / len(key)) + new_key_len))[:new_key_len]
        return ''.join(chr(ord(x1) ^ ord(x2)) for x1, x2 in zip(msg, key))


if __name__ == "__main__":
    pass
