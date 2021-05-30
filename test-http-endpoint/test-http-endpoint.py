#!/usr/bin/env python3

# openssl req -x509 -newkey rsa:2048 -keyout key.pem -nodes -out cert.pem -days 365

from http.server import HTTPServer, BaseHTTPRequestHandler
import ssl
import json
from time import time

HTTPS_ADDRESS = 'localhost'
HTTPS_PORT = 4443

TYPE_NONE = 0
TYPE_DOWNLINK_START = 1
TYPE_DOWNLINK_STOP = 2
TYPE_UPLINK_START = 3
TYPE_UPLINK_STOP = 4

print("Generating event list..")

json_obj = {}
json_obj['events'] = list()

current_time_unix = int(time())

json_obj['events'].append({'description': 'AoS ISS', 'type': TYPE_DOWNLINK_START, 'time_unix': current_time_unix+10})

json_obj['events'].append({'description': 'Start Questions', 'type': TYPE_UPLINK_START, 'time_unix': current_time_unix+30})

json_obj['events'].append({'description': 'Second Questions', 'type': TYPE_NONE, 'time_unix': current_time_unix+72})

json_obj['events'].append({'description': 'End Questions', 'type': TYPE_UPLINK_STOP, 'time_unix': current_time_unix+120})

json_obj['events'].append({'description': 'LoS ISS', 'type': TYPE_DOWNLINK_STOP, 'time_unix': current_time_unix+150})

json_obj['events'].append({'description': 'Debrief', 'type': TYPE_NONE, 'time_unix': current_time_unix+380})

json_bytes = json.dumps(json_obj).encode()

print(json.dumps(json_obj, indent=4))

class RequestHandler(BaseHTTPRequestHandler):

    def do_GET(self):
        self.send_response(200)
        self.end_headers()
        self.wfile.write(json_bytes)        

httpd = HTTPServer((HTTPS_ADDRESS, HTTPS_PORT), RequestHandler)

httpd.socket = ssl.wrap_socket (httpd.socket, keyfile="key.pem", certfile='cert.pem', server_side=True)

print(f"Listening on https://{HTTPS_ADDRESS}:{HTTPS_PORT}/ ..")

httpd.serve_forever()
