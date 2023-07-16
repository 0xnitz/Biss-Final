import socket


def xor(msg, key):
    """
    xor between msg and key, dup the key by len of msg
    """
    # new_key_len = len(msg)
    # key = (key * (int(new_key_len / len(key)) + new_key_len))[:new_key_len]
    return ''.join(chr(ord(x1) ^ ord(x2)) for x1, x2 in zip(msg, key))

my_socket = socket.socket()

my_socket.connect(('127.0.0.1', 5000))
alive = True

LOSE_STRING= "ohhh_no_we_lose123456789!!!!!"
key =        "iran_was_here____123123123123"
while alive:
    while True:
        t = input("enter")
        my_socket.send(xor(LOSE_STRING,key).encode())

