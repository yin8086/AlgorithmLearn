#!/usr/bin/env python
# -*- coding: utf-8 -*-
"""
Created on Thu Sep 04 21:32:05 2014

@author: Stardrad Yin
"""

import socket, sys
import select
import Queue


class Server:
    host = 'localhost'
    port = 8888    
    buf_size = 128
    
    def TcpServer(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_STREAM)
        s.bind((self.host, self.port))
        s.listen(1)
        
        
        clientSock, clientAddr = s.accept()
        print 'Message from', clientAddr
        while True:
            mes = clientSock.recv(self.buf_size)
            if mes != '-1':
                print mes
                clientSock.send('GO')
            else:
                clientSock.send('END')
                break
        clientSock.close()
        s.close()
        
    def UdpServer(self):
        s = socket.socket(socket.AF_INET, socket.SOCK_DGRAM)
        target = (self.host, self.port)
        s.bind(target)
        
        while True:
            mes,addr = s.recvfrom(self.buf_size)
            if mes != '-1':
                print mes
                s.sendto('GO', addr)
            else:
                s.sendto('END', addr)
                break
        s.close()
        
    '''
    http://blog.csdn.net/hongqun/article/details/19161841
    '''
    def SelectServer(self):
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
        server.setblocking(0)  
        
        server_address = (self.host, self.port)  
        print >>sys.stderr, 'starting up on %s port %s' % server_address  
        server.bind(server_address)  
        
        server.listen(5)  
        
        #初始化读取数据的监听列表，最开始时我们希望从server这个套接字上读取数据  
        inputs = [ server ]  
        
        #初始化写入数据的监听列表，最开始时并没有客户端连接进来，所以列表为空  
        outputs = [ ]  
        
        #要发往客户端的数据  
        message_queues = {}  
        
        while inputs:  
        
            print >>sys.stderr, 'waiting for the next event'  
            #调用select监听所有监听列表中的套接字，并将准备好的套接字加入到对应的列表中  
            readable, writable, exceptional = select.select(inputs,  
                                                            outputs,  
                                                            inputs)  
        
            
            #处理可读的套接字  
            """ 
                如果server这个套接字可读，则说明有新链接到来 
                此时在server套接字上调用accept，生成一个与客户端通讯的套接字 
                并将与客户端通讯的套接字加入inputs列表，下一次可以通过select检查该链接是否可读 
                然后在发往客户端的缓冲中加入一项，键名为：与客户端通讯的套接字，键值为：空队列 
                """  
            """  
                若可读的套接字不是server套接字，有两种情况：一种是有数据到来，另外一种是链接断开 
                如果有数据到来，先接收数据，然后将收到的数据填入发往客户端的缓存区中的对应位置，最后 
                将于客户端通讯的套接字加入到写数据的监听列表；  
                如果套接字可读，但是没有接收到数据，则说明客户端已经断开。这时要关闭与客户端连接的套接字，并进行资源清理 
                """  
            
            for s in readable:  
                if s is server:  
                    connection, client_address = s.accept()  
                    print >>sys.stderr, '  connection from', client_address  
                    connection.setblocking(0)  
                    inputs.append(connection)  
                    message_queues[connection] = Queue.Queue()  
                    
                else:  
                    data = s.recv(1024)  
                    if data:  
                        print >>sys.stderr, '  received "%s" from %s' % (data, s.getpeername()) 
                        message_queues[s].put(data) 
                        # 收完数据就准备写
                        if s not in outputs:  
                            outputs.append(s)  
                            
                    else:  
                        print >>sys.stderr, '  closing', s.getpeername() 
                        if s in outputs:  
                            outputs.remove(s)  
                        inputs.remove(s)  
                        s.close()  
                        del message_queues[s]  
        
            #处理可写的套接字  
            """ 
            在发送缓冲区中取出响应的数据 ，发往客户端。 
            如果没有数据需要写，则将套接字从发送对列中移除，select中不再监视  
            """  
            for s in writable:  
                try:  
                    next_msg = message_queues[s].get_nowait()  
                except Queue.Empty:  
                    print >>sys.stderr, '  ', s.getpeername(), 'queue empty'  
                    outputs.remove(s)  
                except KeyError:
                    pass
                else:  
                    print >>sys.stderr, '  sending "%s" to %s' % (next_msg, s.getpeername())  
                    s.send(next_msg)  
        
            # 处理异常情况  
            for s in exceptional:  
                print >>sys.stderr, 'exception condition on', s.getpeername()  
                inputs.remove(s)  
                if s in outputs:  
                    outputs.remove(s)  
                s.close()  
                del message_queues[s]  
        
        #end while  
        
    '''
    http://blog.csdn.net/hongqun/article/details/19161841
    '''
    def PollServer(self):
        
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
        server.setblocking(0)  
        
        server_address = (self.host, self.port)  
        print >>sys.stderr, 'starting up on %s port %s' % server_address  
        server.bind(server_address)  
        
        server.listen(5)  
        
        message_queues = {}  
  
        TIMEOUT = 1000  
        
        """ 
        poll的底层实现比select高效，但是poll在windows下不被支持 
        """  
        
        # poll监听5种事件  
        # POLLIN:输入准备好  
        # POLLPRI:带外数据可读  
        # POLLOUT:准备好接受数据  
        # POLLERR:有错误发生  
        # POLLHUP:通道关闭  
        # POLLVAL:通道未打开  
        
        
        
        #只读事件组合  
        READ_ONLY = ( select.POLLIN |  
                    select.POLLPRI |  
                    select.POLLHUP |  
                    select.POLLERR )  
        #读写事件组合  
        READ_WRITE = READ_ONLY | select.POLLOUT  
        
        
        #得到poll对象  
        poller = select.poll()  
        #在poll对象中注册server套接字，并让poller监听该套接字的READ_ONLY事件  
        poller.register(server, READ_ONLY)  
        
        
        #由于poller.poll返回的是元素为(fd,flag)的列表，所以我们必须保存fd与相应socket对象的映射  
        fd_to_socket = { server.fileno(): server,  
                    }  
        
        while True:  
            #监听已注册的socket事件，返回元素为(描述符，事件标志)的列表  
            print >>sys.stderr, 'waiting for the next event'  
            events = poller.poll(TIMEOUT)  
        
            for fd, flag in events:  
                s = fd_to_socket[fd]  
                
                #处理输入事件  
                if flag & (select.POLLIN | select.POLLPRI):  
                    #当有新连接到来  
                    if s is server:  
                        connection, client_address = s.accept()  
                        print >>sys.stderr, '  connection', client_address  
                        connection.setblocking(0)  
                        fd_to_socket[ connection.fileno() ] = connection  
                        #将与客户端通讯的socket对象注册到poll对象中，并让poll监听该对象的只读属性  
                        poller.register(connection, READ_ONLY)  
                        message_queues[connection] = Queue.Queue()  
                        
                    #客户端有数据到来  
                    else:  
                        data = s.recv(1024)  
                        if data:  
                            print >>sys.stderr, '  received "%s" from %s' % (data, s.getpeername())  
                            message_queues[s].put(data)  
                            # 让poll对象监听该套接字对象的读写属性  
                            poller.modify(s, READ_WRITE)  
                        #客户端断开  
                        else:  
                            print >>sys.stderr, '  closing', s.getpeername() 
                            poller.unregister(s)  
                            s.close()  
                            del message_queues[s]  
                            
        
                                
                #套接字可写  
                elif flag & select.POLLOUT:  
                    # Socket is ready to send data, if there is any to send.  
                    try:  
                        next_msg = message_queues[s].get_nowait()  
                    except Queue.Empty:  
                        # No messages waiting so stop checking  
                        print >>sys.stderr, s.getpeername(), 'queue empty'  
                        #如果没数据可写，则不监听写事件  
                        poller.modify(s, READ_ONLY)  
                    else:  
                        print >>sys.stderr, '  sending "%s" to %s' % (next_msg, s.getpeername())  
                        s.send(next_msg)  
                        
                #通道关闭             
                elif flag & select.POLLHUP:  
                    # Client hung up  
                    print >>sys.stderr, '  closing', client_address, '(HUP)'  
                    poller.unregister(s)  
                    s.close()  
                    del message_queues[s]  
                
                #发生错误  
                elif flag & select.POLLERR:  
                    print >>sys.stderr, '  exception on', s.getpeername()  
                    poller.unregister(s)  
                    s.close()  
                    del message_queues[s]
                    
                    
    def EpollServer(self):
        
        server = socket.socket(socket.AF_INET, socket.SOCK_STREAM)  
        server.setblocking(0)  
        
        server_address = (self.host, self.port)  
        print >>sys.stderr, 'starting up on %s port %s' % server_address  
        server.bind(server_address)  
        
        server.listen(5)  
        
        message_queues = {}  
  
        TIMEOUT = 20
        
        """ 
        Epoll 采用回调法加上内存共享法，提高poll效率
         
        EPOLLIN ：表示对应的文件描述符可以读（包括对端SOCKET正常关闭）；
        EPOLLOUT：表示对应的文件描述符可以写；
        EPOLLPRI：表示对应的文件描述符有紧急的数据可读（这里应该表示有带外数据到来）；
        EPOLLERR：表示对应的文件描述符发生错误；
        EPOLLHUP：表示对应的文件描述符被挂断；
        EPOLLET： 将EPOLL设为边缘触发(Edge Triggered)模式，这是相对于水平触发(Level Triggered)来说的。
        EPOLLONESHOT：只监听一次事件，当监听完这次事件之后，如果还需要继续监听这个socket的话，需要再次把这个socket加入到EPOLL队列里
        """
        
        
        #只读事件组合  
        READ_ONLY = ( select.EPOLLIN | select.EPOLLET )  
        #读写事件组合  
        READ_WRITE = READ_ONLY | select.EPOLLOUT  
        
        
        #得到poll对象  
        poller = select.epoll()  
        #在poll对象中注册server套接字，并让poller监听该套接字的READ_ONLY事件  
        poller.register(server, READ_ONLY)  
        
        
        #由于poller.poll返回的是元素为(fd,flag)的列表，所以我们必须保存fd与相应socket对象的映射  
        fd_to_socket = { server.fileno(): server,  
                    }  
        
        while True:  
            #监听已注册的socket事件，返回元素为(描述符，事件标志)的列表  
            print >>sys.stderr, 'waiting for the next event'  
            events = poller.poll(TIMEOUT) 
        
            for fd, flag in events:  
                s = fd_to_socket[fd]  
                
                #处理输入事件  
                if flag & select.EPOLLIN:  
                    #当有新连接到来  
                    if s is server:  
                        connection, client_address = s.accept()  
                        print >>sys.stderr, '  connection', client_address  
                        connection.setblocking(0)  
                        fd_to_socket[ connection.fileno() ] = connection  
                        #将与客户端通讯的socket对象注册到poll对象中，并让poll监听该对象的只读属性  
                        poller.register(connection, READ_ONLY)  
                        message_queues[connection] = Queue.Queue()  
                        
                    #客户端有数据到来  
                    else:  
                        data = s.recv(1024)  
                        if data:  
                            print >>sys.stderr, '  received "%s" from %s' % (data, s.getpeername())  
                            message_queues[s].put(data)  
                            # 让poll对象监听该套接字对象的读写属性  
                            poller.modify(s, READ_WRITE)  
                        #客户端断开  
                        else:  
                            print >>sys.stderr, '  closing', s.getpeername() 
                            poller.unregister(s)  
                            s.close()  
                            del message_queues[s]  
                            
        
                                
                #套接字可写  
                elif flag & select.POLLOUT:  
                    # Socket is ready to send data, if there is any to send.  
                    try:  
                        next_msg = message_queues[s].get_nowait()  
                    except Queue.Empty:  
                        # No messages waiting so stop checking  
                        print >>sys.stderr, s.getpeername(), 'queue empty'  
                        #如果没数据可写，则不监听写事件  
                        poller.modify(s, READ_ONLY)  
                    else:  
                        print >>sys.stderr, '  sending "%s" to %s' % (next_msg, s.getpeername())  
                        s.send(next_msg)  
                        
                #通道关闭             
                elif flag & select.EPOLLHUP:  
                    # Client hung up  
                    print >>sys.stderr, '  closing', client_address, '(HUP)'  
                    poller.unregister(s)  
                    s.close()  
                    del message_queues[s]  
                
                #发生错误  
                elif flag & select.EPOLLERR:  
                    print >>sys.stderr, '  exception on', s.getpeername()  
                    poller.unregister(s)  
                    s.close()  
                    del message_queues[s]  
        
if __name__ == '__main__':
    s = Server()
    '''
    print 'TCP Test'
    s.TcpServer()
    print 'TCP Test OK'
    
    print 'Udp Test'
    s.UdpServer()
    print 'Udp Test OK'
    
    
    print 'SelectServer Test'
    s.SelectServer()
    print 'SelectServer Test OK'
    
    
    print 'PollServer Test'
    s.PollServer()
    print 'PollServer Test OK'
    '''
    
    print 'PollServer Test'
    s.EpollServer()
    print 'PollServer Test OK'
    
        


    
    
    
    
