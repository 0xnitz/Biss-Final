import socket

my_socket = socket.socket()

my_socket.connect(('127.0.0.1', 4445))
alive = True
while alive:
    while True:
        x = my_socket.recv(1024)
        print(x)
