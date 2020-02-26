import socket
import sys

host = sys.argv[1] 
port = 8000     


def server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind((host, port))
        s.listen()
        print('[*] Waiting for Connection'.format(port))
        conn, addr = s.accept()
        with conn:
            while True:
                conn.sendall('[*] Connected to ' + ip)
                data = conn.recv(1024)
                if not data:
                    break
                conn.sendall(data)

def client():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        print('# ')
        msg = input()
        s.sendall(msg)


if __name__ == '__main__':
    server()
    client()