//
//  Socket.cpp
//  Socket
//
//  Created by LH on 2020/8/18.
//  Copyright © 2020年 longmiaohao. All rights reserved.
//

#include "Socket.hpp"


Socket::Socket(){
    fd = socket(AF_INET, SOCK_STREAM, 0); //创建套接字
    memset(&sockserver_in, 0, sizeof(sockserver_in));
    memset(&sockclient_in, 0, sizeof(sockclient_in));
}

Socket::~Socket(){
    close(fd);  //关闭套接字
}

int Socket::getfd(){
    return fd;
}
void Socket::closefd(const int __fd){
    close(__fd);
}

void Socket::newClient(std::string ip, int port=80){
    sockclient_in.sin_family = AF_INET; // ipv4
    sockclient_in.sin_port = htons(port);
    sockclient_in.sin_addr.s_addr = inet_addr((ip.c_str()));
    this->ip = ip;
    this->port = port;
}

bool Socket::connection(){
    if(connect(fd, (sockaddr *)&sockclient_in, sizeof(sockclient_in)) < 0){
        return false;
    }
    return true;
}

bool Socket::sendHttp(std::string path, std::string type,std::string data, std::stringstream &stream){
    /*
     传输数据后需要有响应数据 否则会进入recv/read阻塞
     */
    std::stringstream streams;
    streams << type << " " << path;
    streams << " HTTP/1.0\r\n";
    streams << "Host: "<< ip << ":" << port << "\r\n";
    streams << "User-Agent: Mozilla/5.0 (Windows; U; Windows NT 5.1; zh-CN; rv:1.9.2.3) Gecko/20100401 Firefox/3.6.3\r\n";
    if (type == "POST")
        streams << "Content-Type:application/x-www-form-urlencoded\r\n";
    streams << "Content-Length:" << data.length() << "\r\n";
    streams << "Connection:close\r\n\r\n";
    streams << data;
    write(fd, streams.str().c_str(),streams.str().size());
    readTcp(fd, stream, OFF);
    return true;
}

void Socket::delHttpHeader(std::stringstream &stream){
    int index = 0;
    for (int i = 0; i < stream.str().length(); i++) {
        if(stream.str()[i] == '\r' && stream.str()[i+1] == '\n' && stream.str()[i+2] == '\r' && stream.str()[i+3] == '\n'){
            index = i + 4;
        }
    }
    stream.str(stream.str().substr(index, stream.str().length()));
};

std::string Socket::getMapValue(const std::stringstream &stream, const std::string key){
    reader.parse(stream.str(), read_value);
    if (read_value.isObject()){ //解析成功
            std::stringstream objstr;
            objstr<<read_value[key];
            return objstr.str().substr(1, objstr.str().length()-3); //删除"号;
    }else if(read_value.isArray()){
        std::cout << "error, the arry string，please use the func getArryValue(const std::string &str, const int index, const std::string key)" << std::endl;
        return "false";
    }else{
        std::cout<<"parse error"<<std::endl;
        return "false";
    }
}

std::string Socket::getMapValue(const std::string &str, const std::string key){
    reader.parse(str, read_value);
    if (read_value.isObject()){ //解析成功
        std::stringstream objstr;
        objstr<<read_value[key];
        return objstr.str().substr(1, objstr.str().length()-3); //删除"号;
    }else if(read_value.isArray()){
        std::cout << "error, the arry string，please use the func getArryValue(const std::string &str, const int index, const std::string key)" << std::endl;
        return "false";
    }else{
        std::cout<<"parse error"<<std::endl;
        return "false";
    }
}

std::string Socket::getArryValue(const std::string &arry_str, int index, const std::string key){
    reader.parse(arry_str, read_value);
    if (read_value.isArray()){ //解析成功
        if (read_value.size() <= index)
            std::cout << "warning, index is out of bounds" << std::endl;
        std::stringstream objstr;
        objstr<<read_value[index][key];
        return objstr.str();
    }else if(read_value.isObject()){
        std::cout << "error, the map string，please use the func getMapValue(const std::string &str, const std::string key) or getMapValue(const std::stringstream &stream, const std::string key)" << std::endl;
        return "false";
    }else{
        std::cout<<"parse error"<<std::endl;
        return "false";
    }
}

long Socket::getArrySize(const std::string &arry_str){
    reader.parse(arry_str, read_value);
    if (read_value.isArray()){
        return read_value.size();
    }
    std::cout << "error, not arry str" << std::endl;
    return -1;
}


bool Socket::newServer(std::string ip, int port){
    this->sockserver_in.sin_family = AF_INET;
    this->sockserver_in.sin_port = htons(port);
    this->sockserver_in.sin_addr.s_addr = inet_addr(ip.c_str());
    this->ip = ip;
    this->port = port;
    return true;
}

bool Socket::bindAndListen(int LISTEN_QUEUE_SISE){
    if (bind(fd, (sockaddr *)&sockserver_in, sizeof(sockserver_in)) == 0){
        if(listen(fd, LISTEN_QUEUE_SISE) == 0){
            std::cout << "监听地址:" << ip << ", 监听端口:" << port <<std::endl;
            return true;
        }else{
            std::cout<< "listen error" << std::endl;
            perror("listen: ");
            exit(-1);
        }
    }else{
        std::cout<< "bind error" << std::endl;
        perror("bind: ");
        exit(-1);
    }
    return false;
};




void Socket::MulThreadAccept(void* (*func)(void* func_args), Func_args *func_args){
        struct sockaddr_in clientaddr;
        socklen_t clientaddr_size = sizeof(&clientaddr);
        int clientfd = accept(fd, (sockaddr*)&clientaddr, &clientaddr_size);
        std::string client_ip = inet_ntoa(clientaddr.sin_addr);
        int client_port = ntohs(clientaddr.sin_port);
        if (clientfd > 0){
//            struct kevent evt;    // 创建
//            EV_SET(&evt, clientfd, EVFILT_READ, EV_ADD | EV_ENABLE, 0, 0, NULL);  // 赋值
//            kevent(kq, &evt, 1, NULL, 0, NULL);    // 添加
            //多线程处理 传入函数 参数
//            pthread_t *p = ;
//            pt.push_back(new _opaque_pthread_t);
            struct Client *client = new struct Client(clientfd, client_port, client_ip);
            func_args->client = client;
            PreInfo preinfo;
            read(clientfd, &preinfo, sizeof(PreInfo));// 获取链接状态
            client->connection_type = preinfo.connection_type;
            pthread_t pt;
            pthread_create(&pt, NULL, func, (void *)func_args); //创建线程
        }else{
            perror("accept:");
        }
}
