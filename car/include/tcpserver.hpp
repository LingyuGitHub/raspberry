/*
 * @Author: lingyu 
 * @Date: 2018-11-15 20:14:05 
 * @Last Modified by:   lingyu 
 * @Last Modified time: 2018-11-15 20:14:05 
 */

#ifndef _TCPSERVER_H_
#define _TCPSERVER_H_

#include <iostream>
#include <thread>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <memory.h>
#include <stdarg.h>
#include <signal.h>
#include <list>

using namespace std;


static void test(const char *msg)
{
}

static void log(int state, const char *fmt, ...)
{
    if (fmt==NULL)
        return;
    char source[1024];
    memset(source, 0, sizeof(source));
    va_list marker;
    va_start(marker, fmt);
    vsnprintf(source, 1024, fmt, marker);
    va_end(marker);
    cout<<state<<" "<<source<<endl;
}


class TcpServer
{
public:
    TcpServer(int port=-1)
    {
        if (port>0)
            m_port=port;
        m_srvfd=-1;
        m_run=false;
        m_onRecv=test;
        m_onLog=log;
    }
    ~TcpServer()
    {

    }

    void setPort(int port)
    {
        if (port>0)
            m_port=port;
    }

    int getPort()
    {
        return m_port;
    }

    //向所有的连接发送消息
    void sendMsg(const char *msg)
    {
        list<int> clifds=m_clifds;
        list<int>::iterator it;
        int fd=0;
        for (it=clifds.begin(); it!=clifds.end(); it++)
        {
            fd=*it;
            send(fd, msg, strlen(msg), 0);
        }
    }

    //设置接收消息后的回调函数
    void setOnRecv(void(*func)(const char *))
    {
        m_onRecv=func;
    }
    
    //设置日志输出的回调函数
    void setOnLog(void(*func)(int,const char*,...))
    {
        m_onLog=func;
    }

    //监听指定的端口，返回0表示监听成功
    int start(int port=-1)
    {
        signal(SIGPIPE, SIG_IGN);

        if (port>0)
            m_port=port;
        if (m_port<=0)
        {
            m_onLog(0, "Port is %d error", m_port);
            return 1;
        }

        m_clifds.clear();

        m_srvfd=socket(AF_INET, SOCK_STREAM, 0);
        sockaddr_in srvaddr;
        memset(&srvaddr, 0, sizeof(sockaddr_in));
        srvaddr.sin_family=AF_INET;
        srvaddr.sin_port=htons(m_port);
        srvaddr.sin_addr.s_addr=htonl(INADDR_ANY);

        if (bind(m_srvfd, (sockaddr*)&srvaddr, sizeof(srvaddr))==-1)
        {
            perror("Bind");
            m_onLog(0, "Bind port %d error, %s", m_port, strerror(errno));
            return 2;
        }
        if (listen(m_srvfd, 10)==-1)
        {
            perror("Listen");
            m_onLog(0, "Listen port %d error, %s", m_port, strerror(errno));
            return 3;
        }

        m_run=true;
        m_listenThread=thread(&TcpServer::onListen, this);
        m_recvThread=thread(&TcpServer::onRecv, this);
        m_onLog(0, "Start tcp server successfully");

        return 0;
    }

    void stop()
    {
        m_run=false;
        m_listenThread.join();
        m_recvThread.join();
        list<int>::iterator it;
        for (it=m_clifds.begin(); it!=m_clifds.end(); it++)
        {
            close(*it);
            m_onLog(0, "Disconnect client %d", *it);
        }
        m_clifds.clear();
        close(m_srvfd);
        m_onLog(0, "Stop tcp server");
    }


protected:
    void onListen()
    {
        sockaddr_in cliaddr;
        socklen_t addrlen=sizeof(cliaddr);
        memset(&cliaddr, 0, sizeof(sockaddr_in));
        int clifd, srvfd;
        int ret=0;
        srvfd=m_srvfd;
        int count=0;
        timeval tv;
        fd_set rfds;
        while (m_run)
        {
            //等待新的连接
            tv.tv_sec=10;
            tv.tv_usec=0;

            FD_ZERO(&rfds);
            FD_SET(srvfd, &rfds);
            ret=select(srvfd+1, &rfds, NULL, NULL, &tv);
            if (ret==-1)
            {
                m_onLog(0, "Error on select accept, %s", strerror(errno));
                stop();
                break;
            }
            else if (ret==0)
            {
            }
            else
            {
                clifd=accept(srvfd, (sockaddr*)&cliaddr, &addrlen);
                if (clifd<=0)
                {
                    m_onLog(0, "Error on accept, %s", strerror(errno));
                    stop();
                    break;
                }
                m_clifds.push_back(clifd);
                m_onLog(1, "Client %d connect", clifd);
            }

            //心跳检测，剔除断开的连接
            count++;
            if (count%1==0)
            {
                list<int>::iterator it;
                int a=0x00;
                it=m_clifds.begin();
                while (it!=m_clifds.end())
                {
                    if (send(*it, &a, sizeof(a), 0)<=0)
                    {
                        m_onLog(1, "Client %d disconnect and removed", *it);
                        close(*it);
                        it=m_clifds.erase(it);
                    }
                    else
                    {
                        it++;
                    }
                }
                count=0;
            }
        }
    }

    //异步接收所有连接的消息，对于断开的连接不处理，交给心跳检测处理
    void onRecv()
    {
        int ret=0;
        int len=0;
        char buff[1024];
        list<int>clifds;
        list<int>::iterator it;
        timeval tv;
        fd_set rfds;
        int fd=0, maxfd=0;
        while (m_run)
        {
            clifds=m_clifds;
            tv.tv_sec=0;
            tv.tv_usec=100;
            FD_ZERO(&rfds);
            for (it=clifds.begin(); it!=clifds.end(); it++)
            {
                fd=*it;
                FD_SET(fd, &rfds);
                maxfd=maxfd>fd? maxfd:fd;
            }

            ret=select(maxfd+1, &rfds, NULL, NULL, &tv);
            if (ret==-1)
            {
                m_onLog(0, "Error on select, %s", strerror(errno));
            }
            else if (ret==0)
            {
            }
            else
            {
                for (it=clifds.begin(); it!=clifds.end(); it++)
                {
                    fd=*it;
                    if (!FD_ISSET(fd, &rfds))
                        continue;
                    memset(buff, 0, sizeof(buff));
                    len=recv(fd, buff, sizeof(buff), 0);
                    if (len<0)
                    {
                        m_onLog(0, "Error on recv %d, %s", fd, strerror(errno));
                    }
                    else if (len==0)
                    {
                    }
                    else
                    {
                        m_onLog(0, "Recv %s len=%d from %d", buff, len, fd);
                        m_onRecv(buff);
                    }
                }
            }
        }
    }



private:
    int m_port;
    int m_srvfd;
    list<int> m_clifds;
    bool m_run;

    void (*m_onRecv)(const char *);
    void(*m_onLog)(int, const char*, ...);

    thread m_listenThread;
    thread m_recvThread;
};

#endif

