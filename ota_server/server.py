import socket
import time

HOST = '0.0.0.0'
PORT = 8080
FIRMWARE_FILE = 'firmware_v2.txt'

# Wait for ECU to come up
time.sleep(3)

server_socket = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
server_socket.bind((HOST, PORT))
server_socket.listen(1)
print("📡 OTA Server is waiting for ECU...")

conn, addr = server_socket.accept()
print(f"✅ ECU connected from {addr}")

with open(FIRMWARE_FILE, 'rb') as f:
    while chunk := f.read(1024):
        conn.sendall(chunk)

conn.shutdown(socket.SHUT_WR)
ack = conn.recv(1024)
print("✅ ECU says:", ack.decode())

conn.close()
server_socket.close()
