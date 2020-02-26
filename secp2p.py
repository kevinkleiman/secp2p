import socket
import sys

host = sys.argv[1] 
port = 8000     


def server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.bind(('localhost', port))
        s.listen()
        print('[*] Waiting for Connection'.format(port))
        conn, addr = s.accept()
        with conn:
            while True:
                connected = '[*] Connected to {}'.format(addr)
                conn.sendall(connected.encode())
                data = conn.recv(1024)
                if not data:
                    break
                conn.sendall(data)

def client():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        s.connect((host, port))
        msg = input()
        print('# ')
        s.sendall(msg.encode())


if __name__ == '__main__':
    server()
    client()
