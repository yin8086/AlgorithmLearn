#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Thu Sep 04 20:53:16 2014

@author: Stardrad Yin
"""

import socket,sys

class Client:
    host = 'localhost'
    port = 8888
    buf_size = 128
    
    def TcpClient(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.connect((self.host, self.port))
        
        while True:
            line = sys.stdin.readline()
            s.send(line.strip('\r\n'))
            if s.recv(self.buf_size) == 'END':
                break

        s.close()
        
    def UdpClient(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        target = ((self.host, self.port))
        
        while True:
            line = sys.stdin.readline()
            s.sendto(line.strip('\r\n'), target)
            
            # receive format (string, addr)
            if s.recvfrom(self.buf_size)[0] == 'END':
                break

        s.close()
        
    '''
    http://blog.csdn.net/hongqun/article/details/19161841
    '''
    def MultiClient(self):
  
        socks = [socket.socket(socket.AF_INET, socket.SOCK_STREAM) for i in range(20)]  

        server_address = (self.host, self.port)
        for s in socks:  
            try:  
                s.connect((self.host, self.port))  
                print >>sys.stderr, 'connecting to %s port %s' % server_address  
            except Exception as e:  
                print e.message  
               
        messages = ['This is a message']
        
        for message in messages:  
            for s in socks:  
                print >>sys.stderr, '%s: sending "%s"' % (s.getsockname(), message)  
                s.send(message)  
            for s in socks:  
                data = s.recv(1024)  
                print >>sys.stderr, '%s: received "%s"' % (s.getsockname(), data)  
                if not data:  
                    print >>sys.stderr, 'closing socket', s.getsockname()  
                    s.close()  
        
        
if __name__ == '__main__':
    c = Client()
    '''
    print 'TCP Test'
    c.TcpClient()
    print 'TCP Test OK'
    
    print 'Udp Test'
    c.UdpClient()
    print 'Udp Test OK'
    '''
    print 'MultiClient Test'
    c.MultiClient()
    print 'MultiClient Test OK'