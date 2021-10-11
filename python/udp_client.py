import socket
import sys
import json
from datetime import datetime
import time
import asyncio
import random


serverAddress = sys.argv[1]
serverPort = int(sys.argv[2])

nodesList = [('127.0.0.1', 6789), ('127.0.0.1', 6790), ('127.0.0.1', 6791)]

"""
timeStamp = datetime.now().strftime("%d-%b-%Y %H:%M:%S.%f")
epoch = time.time()
myinfo = {'time' : timeStamp, 'epoch' : epoch}
"""
req = {'type': 'REQUEST', 'subtype': 'GET', 'key': '23'}
clientSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)

def raiseRequest():
	req['key'] = str(random.randrange(256))
	node = random.choice(nodesList)
	time.sleep(2)
	clientSocket.sendto(str.encode(json.dumps(req)), node)

def handleResponse():
	data, addr = clientSocket.recvfrom(1024)
	print("Received response:{0}".format(data.decode('utf-8')))
	loop.call_soon(raiseRequest)

loop = asyncio.get_event_loop()
loop.add_reader(clientSocket, handleResponse)


clientSocket.sendto(str.encode(json.dumps(req)), (serverAddress, serverPort))
try:
	loop.run_forever()
finally:	
	clientSocket.close()
	loop.close()

