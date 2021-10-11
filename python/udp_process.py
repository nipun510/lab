import socket
import threading
import logging
import sys
import time
import random
import asyncio
import json


#serverLock = threading.Lock()
serverAddress = (str(sys.argv[1]), int(sys.argv[2]))
nodeName = sys.argv[3]
addressList = [("127.0.0.1", 6789), ("127.0.0.1", 6790), ("127.0.0.1", 6791)]

serverSocket = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
serverSocket.bind(serverAddress)

loop = asyncio.get_event_loop()

BUFFER_SIZE = 1024


kvStore = {}

"""
myinfo = {'time' : None, 'epoch' : 0}

def updateInfo(jsonData):
	info = json.loads(jsonData)
	if  myinfo['epoch'] < info['epoch']:
		myinfo['time'] = info['time']
		myinfo['epoch'] = info['epoch']

def onRecv1():
  data, addr = serverSocket.recvfrom(BUFFER_SIZE)
  updateInfo(data)
  print("{0}: updated time [{1}] recieved from {2}".format(serverAddress, myinfo['time'], addr))
  addressOthers = [ addr  for addr in addressList if addr != serverAddress ]
  randomNode = addressOthers[random.randrange(len(addressOthers))]	
  time.sleep(2)
  serverSocket.sendto(str.encode(json.dumps(myinfo)), randomNode)
	"""

def handleRequest(data):
	request = json.loads(data)
	key = request['key']
	res = {'type' : 'RESPONSE', 'key': request['key'], 'value' : 'default'}

	if request['type'] == 'REQUEST':
		if request['subtype'] == 'GET':
			res['value'] = int(key) + 1
		elif request['subtype'] == 'POST':
			pass
		elif request['subtype']	== 'PUT':
			pass
		else:
			assert(False)
	elif request['type'] == 'RESPONSE':
		return request		

	return res		

def sendResponse(data, addr):
	serverSocket.sendto(data.encode('utf-8'), addr)	

def onRecv():
	data, addr = serverSocket.recvfrom(BUFFER_SIZE)
	print("Recived request:{0}".format(data.decode('utf-8')))
	resp = handleRequest(data)
	loop.call_soon(sendResponse, json.dumps(resp), addr)

loop.add_reader(serverSocket, onRecv)
try:
  loop.run_forever()
finally:
  serverSocket.close()
  loop.close()  
