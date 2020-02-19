#!/usr/bin/env python3
import socket
import time
IPADDR = '127.0.0.1'
PORTNUM = 8000

s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM, 0)
s.bind(('0.0.0.0',10001))
s.connect((IPADDR, PORTNUM))

for line in open('a.ZS'):
    data=int(line,16)
    if(data == 0xfafafafa):
        time.sleep(5e-3)
    s.send(data.to_bytes(4, byteorder='little'))

s.close()
