#!/usr/bin/env python3

# openssl req -x509 -newkey rsa:2048 -keyout key.pem -nodes -out cert.pem -days 365

from http.server import HTTPServer, BaseHTTPRequestHandler
import ssl
import json
from datetime import datetime, timedelta

HTTPS_ADDRESS = 'localhost'
HTTPS_PORT = 4443

TYPE_NONE = 0
TYPE_DOWNLINK_START = 1
TYPE_DOWNLINK_STOP = 2
TYPE_UPLINK_START = 3
TYPE_UPLINK_STOP = 4

print("Generating event list..")

json_array = list()

current_datetime = datetime.utcnow()

json_array.append({'description': 'AoS ISS', 'type': TYPE_DOWNLINK_START, 'time': (current_datetime + timedelta(seconds=10)).isoformat()})

json_array.append({'description': 'Start Questions', 'type': TYPE_UPLINK_START, 'time': (current_datetime + timedelta(seconds=30)).isoformat()})

json_array.append({'description': 'Second Questions', 'type': TYPE_NONE, 'time': (current_datetime + timedelta(seconds=72)).isoformat()})

json_array.append({'description': 'End Questions', 'type': TYPE_UPLINK_STOP, 'time': (current_datetime + timedelta(seconds=120)).isoformat()})

json_array.append({'description': 'LoS ISS', 'type': TYPE_DOWNLINK_STOP, 'time': (current_datetime + timedelta(seconds=150)).isoformat()})

json_array.append({'description': 'Debrief', 'type': TYPE_NONE, 'time': (current_datetime + timedelta(seconds=210)).isoformat()})

json_bytes = json.dumps(json_array).encode()

print(json.dumps(json_array, indent=4))

class RequestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(json_bytes)        

httpd = HTTPServer((HTTPS_ADDRESS, HTTPS_PORT), RequestHandler)

httpd.socket = ssl.wrap_socket (httpd.socket, keyfile="key.pem", certfile='cert.pem', server_side=True)

print(f"Listening on https://{HTTPS_ADDRESS}:{HTTPS_PORT}/ ..")

httpd.serve_forever()
