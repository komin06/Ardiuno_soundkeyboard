import socket

PORT = 5000  # Arduinoと合わせる

sock = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
sock.bind(("0.0.0.0", PORT))
print(f"UDP待ち受け中... ポート {PORT}")

while True:
    data, addr = sock.recvfrom(1024)
    print(f"from {addr}: {data.decode(errors='ignore')}")
