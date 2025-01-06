#!/usr/bin/env python3

'''A simple http server for test the interface of httpclient.'''

import http.server

class SimpileServer(http.server.BaseHTTPRequestHandler):
    def do_GET(self):
        self.send_response(http.HTTPStatus.OK)
        self.end_headers()

        self.wfile.write("cool GET".encode())
        self.wfile.flush()

    def do_POST(self):
        self.send_response(http.HTTPStatus.OK)
        self.end_headers()

        self.wfile.write("cool POST".encode())
        self.wfile.flush()


server = http.server.ThreadingHTTPServer(("", 8000), SimpileServer)
server.serve_forever()
