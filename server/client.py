import pytest
import socket

HOST_SERVER = '192.168.1.70'

PORT = 8080

testStr = 'switch'

# def test_client():
s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.connect((HOST_SERVER, PORT))
s.send(testStr.encode('UTF-8'))
data = s.recv(1024)
res = data.decode('UTF-8')
print(res)       
s.close()