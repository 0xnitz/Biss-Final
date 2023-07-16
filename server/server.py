from socket import *
import threading
import server.logger as logger
import server.crypto as crypto
import time
import random

is_lose = False


class main_sever:
    first_msg = "give_me_the_password"

    # this is the encoded messages
    msg_list = ["give_me_the_password", "bla", "ggg", "hhh", "eee", "ccC", "qqq", "lll", "6666"]

    # this is the magic to kill all the instances of the malware
    LOSE_STRING = "ohhh_no_we_lose123456789!!!!!"

    def __init__(self):
        self.log = logger.Logger()

    def listen(self):
        t1 = threading.Thread(target=self.lose_func)
        t1.start()
        server = socket(AF_INET, SOCK_STREAM)
        server.bind(("0.0.0.0", 4445))
        server.listen(5)
        while True:
            print("waiting for connection...")
            client, addr = server.accept()
            self.log.info(f"{addr} : connected")
            t = threading.Thread(target=self.connection_instance, args=(client, addr))
            t.start()

    def connection_instance(self, client: socket, addr):
        crypt = crypto.Crypto()
        while True:
            # client.send(crypt.encrypt_msg(main_sever.first_msg).encode())
            # time.sleep(0.5)
            try:
                if not is_lose:
                    random.shuffle(main_sever.msg_list)  # TODO: this function not in use after python 3.9
                    for msg in main_sever.msg_list:
                        client.send(crypt.encrypt_msg(msg).encode())
                        time.sleep(0.5)
                    client.send(crypt.encrypt_msg(crypt.keep_alive).encode())
                    time.sleep(0.5)
                else:
                    client.send(b"lose")
                    time.sleep(0.3)
            except Exception as e:
                self.log.info(f"{addr} close the connection")
                client.close()
                break

    def lose_func(self):
        """
        create socket that receive data , if the data is the key encrypted with the key, send kill
        to whole the instnacs of the malware
        :return:
        """
        server = socket(AF_INET, SOCK_STREAM)
        server.bind(("0.0.0.0", 5000))
        server.listen(5)

        while True:
            print("waiting for connection...")
            client, addr = server.accept()
            self.log.info(f"{addr} : connected to kill server")
            t = threading.Thread(target=self.handle_lose_client, args=(client, addr))
            t.start()

    def handle_lose_client(self, client, addr):
        global is_lose

        c = crypto.Crypto()
        while True:
            data = client.recv(1024).decode()
            self.log.info(f"{addr} send data to kill server")
            data = c.xor(data, c.key)
            print(data)
            if data == main_sever.LOSE_STRING:
                is_lose = True
            break


if __name__ == "__main__":
    main_sever().listen()
