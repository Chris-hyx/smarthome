//
//  Socket.hpp
//  Socket
//
//  Created by LH on 2020/8/18.
//  Copyright © 2020年 longmiaohao. All rights reserved.
//

#ifndef Socket_hpp
#define Socket_hpp
#include <iostream>
#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>  //addr_sin
#include <arpa/inet.h>   //inet_addr,inet_aton
#include <unistd.h>     // close
#include <sys/types.h>
#include <sstream>
#include "json/json.h"
#include <pthread.h>    //画多线程
#endif /* Socket_hpp */
#define BUFSIZE 1024
#define SERVER 0
#define CLIENT 1
#define OFF 0
#define ON 1


typedef struct Client{  //客户端结构体
    int client_fd;
    int client_port;
    std::string client_ip;
    char connection_type;   //S短链接 即连即断，L长链接客户端断开则不断
//    unsigned long pt_id;
//    std::vector<_opaque_pthread_t*>*pt;
    Client(int client_fd, int client_port, std::string client_ip, char connection_type='S'):client_fd(client_fd), client_port(client_port),client_ip(client_ip){};
    Client(){};
    ~Client(){
		close(client_fd);
	};
}Client;

typedef struct PreInfo {    //传输协议结构体
    long length; //数据长度
    char connection_type;   //S短链接 即连即断，L长链接客户端断开则不断
    PreInfo(long len, char connection_type='S'):length(len),connection_type(connection_type){};
    PreInfo(){};
}PreInfo;

typedef struct Func_args{
    std::stringstream *stream;
    Client *client;
    Func_args(std::stringstream *_stream=NULL, Client *_client=NULL):stream(_stream), client(_client){};
    ~Func_args(){
        delete stream;
        delete client;
    }
//    Func_args(){};
}Func_args;

class Socket{
private:
    int fd, port, kq;
    struct sockaddr_in sockserver_in, sockclient_in;
    std::string ip;
    Json::Reader reader;
    Json::Value read_value;
//    std::vector<_opaque_pthread_t*>pt; //线程数组
public:
    Socket();
    ~Socket();
    int getfd();
    void closefd(const int __fd);
    //json解析相关
    std::string getMapValue(const std::stringstream &stream, const std::string key);
    std::string getMapValue(const std::string &str, const std::string key);
    long getArrySize(const std::string &arry_str);
    std::string getArryValue(const std::string &arry_str, int index, const std::string key);
    //client部分
    void newClient(std::string ip, int port);
    bool connection();
    // server部分
    bool newServer(std::string ip="0.0.0.0", int port=80);
    bool bindAndListen(int LISTEN_QUEUE_SISE=10);
    void MulThreadAccept(void* (*func)(void* func_args), Func_args *func_args);
    // Http
    bool sendHttp(std::string path,std::string type,std::string data, std::stringstream &stream);
    void delHttpHeader(std::stringstream &stream);

//    void static writeTcp(int __fd, const std::stringstream &stream, int flag);
//    void static readTcp(int __fd, std::stringstream &stream, int flag); //void (*func)(Func_args*)为用户自定义函数，传入参数为Func_args结构体
    
    static void writeTcp(int __fd, const std::stringstream &stream, int flag, char connection_type='S'){
        struct PreInfo *preinfo = NULL;
        if (flag){
            preinfo = new PreInfo(stream.str().size(), connection_type);
            preinfo->length = stream.str().size();//协商长度
            write(__fd, preinfo, sizeof(PreInfo));
        }
        write(__fd, stream.str().c_str(), stream.str().size());
        delete preinfo;
    }
    
    static void readTcp(int __fd, std::stringstream &stream, int flag=ON){
        char buf[BUFSIZE] = {0};  //1kb
        long rc = 0; //缓冲区取出字节大小
        long offset = 0;
        struct PreInfo *preinfo = NULL;
        if (flag){
            preinfo = new PreInfo();
            read(__fd, preinfo, sizeof(PreInfo));
        }
#ifdef WIN32
//            while((rc = recv(fd, buf, retsize)) > 0){
//                stream<<buf;
//                                sock->closefd(client->client_fd);
//                memset(buf, 0, BUFSIZE);
//            }
#else
        stream.str("");
        while((rc = read(__fd, buf, BUFSIZE)) > 0){
            stream<<buf;
            memset(buf, 0, BUFSIZE);
            if(flag){
                offset += rc;
                if (offset >= preinfo->length)
                    break;
            }
        }
        if (rc < 0){
            perror("read:");
        }
        delete preinfo;
#endif
    }

static void readTcp(Client *client, std::stringstream &stream, int flag=ON){
    char buf[BUFSIZE] = {0};  //1kb
    long rc = 0; //缓冲区取出字节大小
    long offset = 0;
    struct PreInfo *preinfo = NULL;
    int __fd = client->client_fd;
    if (flag){
        preinfo = new PreInfo();
        read(__fd, preinfo, sizeof(PreInfo));
        client->connection_type = preinfo->connection_type;
    }
#ifdef WIN32
//        while((rc = recv(fd, buf, retsize)) > 0){
//            stream<<buf;
//                            sock->closefd(client->client_fd);
//            memset(buf, 0, BUFSIZE);
//        }
#else
    stream.str("");
    while((rc = read(__fd, buf, BUFSIZE)) > 0){
        stream<<buf;
        memset(buf, 0, BUFSIZE);
        if(flag){
            offset += rc;
            if (offset >= preinfo->length)
                break;
        }
    }
    if (rc < 0){
        perror("read:");
    }
    delete preinfo;
#endif
}
};
