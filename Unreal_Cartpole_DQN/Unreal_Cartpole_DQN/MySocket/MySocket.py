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
            recvData = self.cs.recv(100)

        my_json = recvData.decode('utf8').replace("'", '"')
        #print(my_json);
        
        data = json.loads(my_json, object_hook=JsonObject)
        
        print(data.Name)
        a = data.CartPos
        b = data.Angle
        c = data.CartSpeed
        d = data.AngelChange
        e = data.Done        

        print("RecvData : ", a , " ", b, " ", c, " " , d, " ", e)

        return np.array([a,b,c,d]),1,e
