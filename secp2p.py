import socket
import sys
import threading

host = sys.argv[1] 
port = 8000     

def server():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        name = socket.gethostname()
        ip = socket.gethostbyname(name)
        s.bind((ip, port))
        s.listen()
        print('[*] Waiting for Connection on {}'.format(ip))
        conn, addr = s.accept()
        with conn:
            conn.sendall(b'[*] Connected to {} on port {}'.format(ip, port))
            while True:
                data = conn.recv(1024)
                print('[REMOTE] ' + data.decode())
                if data == '!close':
                    s.close()
                    print('[-] Connection closed by remote')
                    break
                conn.sendall(data)

def client():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as s:
        while True:
            try:
                s.connect((host, port))
                break
            except:
                pass

        while True:
            msg = input('# ')
            if msg == '!close':
                s.sendall(b'!close')
                s.close()
                break
            print('[YOU] ' + msg)
            s.sendall(msg.encode())


if __name__ == '__main__':
    tserver = threading.Thread(name='server', target=server)
    tclient = threading.Thread(name='client', target=client)
    tserver.start()
    tclient.start()
