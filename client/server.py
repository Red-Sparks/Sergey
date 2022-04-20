import pytest
import socket

HOST_SERVER = '192.168.1.65'
PORT = 8080

testStr = 'All is working'

# def test_server():
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((HOST_SERVER, PORT))
s.listen()
conn, addr = s.accept()
print('connected:', addr) 
i = 0
while True:
        data = conn.recv(1024)
        print(data.decode('UTF-8'))
        conn.send((testStr + str(i)).encode('UTF-8'))
        i = i + 1
# conn.close()
# s.close()