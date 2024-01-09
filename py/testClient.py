import requests
import os
import json
import hashlib

url = ('http://vmu22a:5000/srv')


##This redirects over HTTP so isn't actually a HTTP request

data = {
    'segSize': 500, 
    'segNum': 5
}


for i in range(0, 8):
    data['segNum'] = i
    print("############################################")
    print("Segment %d"%i)


    x = requests.get(url, params=data)
    
    if (not x.ok):
        print("HTTP ERROR Code %d"%x.status_code)
    else:
        md = hashlib.md5(x.content)
        print("MD5 %s"%(md.hexdigest()))
        print("Length %d"%len(x.content))
        print(x.content)