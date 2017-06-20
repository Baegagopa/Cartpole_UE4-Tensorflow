import socket
import threading
import marshal
import numpy as np
import json
from struct import *

class JsonObject:
  def __init__(self, d):
    self.__dict__ = d

class MySocket:
    def __init__(self):
        self.HOST = '127.0.0.1' #''
        self.PORT = 8080
        self.s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        self.s.bind((self.HOST, self.PORT))
        self.s.listen(1)
        self.addr = 0
        print("Wait")
        self.s.setblocking(True)
        self.cs,self.addr = self.s.accept()
        print("Accept")

        self.cs.setblocking(True)
        

    def sendingMsg(self, action):
        data = {'Move' : action}
        jsonString = json.dumps(data)
        self.cs.send(jsonString.encode())

    def getStep(self):
        recvData = 0
        while not recvData:
            recvData = self.cs.recv(300)

        my_json = recvData.decode('utf8').replace("'", '"')
        #print(my_json);
        
        data = json.loads(my_json, object_hook=JsonObject)
        
        a = float(data.CartPos)
        b = float(data.Angle)
        c = float(data.CartSpeed)
        d = float(data.AngelChange)
        e = float(data.Done)
        f = a+b
        print("RecvData : ", a , "  ", b, "  ", c, "  " , d, "  ", f)


        #return np.array([a,b,c,d]),1,e

a = "0.1"
b = "0.2"

a_t = float(a)
b_t = float(b)

print(a_t , b_t)

print(a_t + b_t)
print(0.1 + 0.2)
#s = MySocket()
#s.getStep()
#while True:
#s.sendingMsg('1')

while True:
    a = 1


'''
import sys
import socket


TCP_IP = '127.0.0.1'
TCP_PORT = 8080
BUFFER_SIZE = 20  # Normally 1024, but we want fast response

s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
s.bind((TCP_IP, TCP_PORT))
s.listen(1)

conn, addr = s.accept()
print ('Connection address:', addr)
while 1:
    data = conn.recv(BUFFER_SIZE) # This returns immediately with no data, when client connection is run from script and doesn't send() anything, just connects.
    if not data:
        print ("broken")
        break
    print ("received data:", data)
    conn.send(data)  # echo

conn.close()
sys.exit()
'''