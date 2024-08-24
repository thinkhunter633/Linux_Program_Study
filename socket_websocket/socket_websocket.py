import socket
import base64
import hashlib

HOST = 'localhost'
PORT = 5000

def generate_accept_key(key):
    guid = "258EAFA5-E914-47DA-95CA-C5AB0DC85B11"
    complete_key = key + guid
    sha1 = hashlib.sha1(complete_key.encode())
    accept_key = base64.b64encode(sha1.digest()).decode('utf-8')
    return accept_key


def send_websocket_response(conn, message):
    message_bytes = bytes(message, 'utf-8')

    # 构造websocket帧
    response = bytearray()
    response.append(0x81)
    response.append(len(message_bytes))
    response.extend(message_bytes)

    # 发送websocket帧
    conn.send(response)

def handle_websocket_connection(conn):
    while True:
        data = conn.recv(1024)
        if not data:
            break

        opcode = data[0] & 0x7f
        is_masked = (data[1] & 0x80) == 0x80
        payload_length = data[1] & 0x7f

        if payload_length == 126:
            payload_length = int.from_bytes(data[2:4], byteorder='big')
            payload_offset = 4
        elif payload_length == 127:
            payload_length = int.from_bytes(data[2:10], byteorder='big')
            payload_offset = 10
        else:
            payload_offset = 2

        if is_masked:
            masking_key = data[payload_offset : payload_offset + 4]
            masked_payload = data[payload_offset + 4 : payload_offset + 4 + payload_length]
            payload = bytes([masked_payload[i] ^ masking_key[i % 4] for i in range(payload_length)])
        else:
            payload = data[payload_offset : payload_offset + payload_length]

        if len(data) < payload_offset + payload_length:
            continue

        if opcode == 8:   # 收到关闭消息
            print("Received close message.")
            conn.close()
            return
        elif opcode == 1:   # 收到文本消息
            message = str(payload, 'utf-8')
            print(f"Received text message: {message}")

            # 回复收到的消息
            send_websocket_response(conn, message)

def main():
    with socket.socket(socket.AF_INET, socket.SOCK_STREAM) as server_socket:
        server_socket.setsockopt(socket.SOL_SOCKET, socket.SO_REUSEADDR, 1)
        server_socket.bind((HOST, PORT))
        server_socket.listen()

        print(f"Listening on port {PORT}...")

        while True:
            conn, addr = server_socket.accept()
            print(f"Client connected from {addr[0]}:{addr[1]}")

            data = conn.recv(1024)
            headers = str(data, 'utf-8').split('\r\n')
            key = ''

            for header in headers:
                if header.startswith('Sec-WebSocket-Key:'):
                    key = header.split(': ')[1].strip()
        
            accept_key = generate_accept_key(key)

            response = "HTTP/1.1 101 Switching Protocols\r\n"
            response += "Upgrade: websocket\r\n"
            response += "Connection: Upgrade\r\n"
            response += f"Sec-WebSocket-Accept: {accept_key}\r\n"
            response += "\r\n"

            conn.send(bytes(response, 'utf-8'))

            handle_websocket_connection(conn)
            exit()

if __name__ == '__main__':
    main()
