import socket
print("Hello! Input your value.")
send = input("input: ")

sock = socket.socket()
sock.connect(('10.124.7.159', 8888))
sock.send(send.encode())

data = sock.recv(1024).decode()

print(data)

sock.close()