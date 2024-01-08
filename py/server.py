#!/usr/bin/env python # 
"""
Test WebServices HTTP Server
Usage: server
"""


from flask import Flask
from flask import send_file
from datetime import datetime
import json
import socket
import os


app = Flask(__name__, static_url_path='/static')

@app.route('/', methods=['GET', 'POST'])
def root():
    return "<HTML><BODY><H1>Flask Test Page</H1></BODY?</HTML>"


@app.route('/time', methods=['GET', 'POST'])
def time():
    a = datetime.now()
    t = {
        'year': a.year,
        'month': a.month,
        'day': a.day,
        'hour': a.hour,
        'minute': a.minute,
        'second': a.second
        }

    return json.dumps(t)


@app.route('/grn', methods=['GET'])
def grn():
    fileName = "../grn/build/src/grn.bin"
    if os.path.isfile(fileName):
        return send_file(fileName)
    else:
        abort(404)

@app.route('/blu', methods=['GET'])
def blu():
    fileName = "../blu/build/src/blu.bin"
    if os.path.isfile(fileName):
        return send_file(fileName)
    else:
        abort(404)

@app.route('/srv', methods=['GET'])
def srv():
    fileName = "./server.py"
    if os.path.isfile(fileName):
        return send_file(fileName)
    else:
        abort(404)


if __name__ == "__main__":
    app.secret_key = 'DRJONEA'
    app.config['SESSION_TYPE'] = 'filesystem'
    

    hostname = socket.gethostname()
    ip_address = socket.gethostbyname(hostname)
    print(f"Hostname: {hostname}")
    print(f"IP Address: {ip_address}")
    
    app.run(host="0.0.0.0")