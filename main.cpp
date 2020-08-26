//
//  main.cpp
//  Socket
//
//  Created by LH'MAC on 2020/8/18.
//  Copyright © 2020年 longmiaohao. All rights reserved.
//

#include "Socket.hpp"
#include "Mysql.hpp"

using namespace std;

//客户端数据处理函数
void* process_client_request(void* func_args)
{
    struct Func_args* _func_args = (Func_args*)func_args;
    char connectton_type = _func_args->client->connection_type;
    std::cout << "port:" << _func_args->client->client_port<<", ip:"<<_func_args->client->client_ip <<"  connection_type:"<<connectton_type<<std::endl;
    if (connectton_type == 'S'){ //短链接 即连即断
        Socket::readTcp(_func_args->client, *_func_args->stream, ON);
        std::cout << "msg: " << _func_args->stream->str()<<endl;
        std::stringstream stream;
		Mysql mysql("127.0.0.1", 3306, "root", "root", "smarthome");
		Socket json;
		string username = json.getMapValue(_func_args->stream->str(), "username");
		string password = json.getMapValue(_func_args->stream->str(), "password");
   		if(mysql.connection()){
			string sql = "select * from user where username=" +  username + " and password=" + password;
			cout<<sql<<endl;
       		mysql.query(sql);
	   		int row_nums = mysql.getRowNum();
	   		//int col_nums = mysql.getColNum();
	   		//MYSQL_FIELD *fields = mysql.getFields();
	   		//for(int i = 0; i < row_nums; i++){
	   		//	for(int j = 0 ; j < col_nums; j++){
			//		cout<<mysql.json_value[i][fields[j].name];
			//	}
	   		//}
			if(row_nums == 1)
        		stream << "{\"status\": \"01\", \"msg\": \"successful\"}";
			else
        		stream << "{\"status\": \"00\", \"msg\": \"failed\"}";

   		}
   		else{
   			cout<<"database connect failed"<<endl;
   		}

        //stream << "{\"status\": \"01\", \"msg\": \"ok\"}";
        Socket::writeTcp(_func_args->client->client_fd, stream, ON);
		cout<<"断开客户端"<<endl;
    }else if(_func_args->client->connection_type == 'L'){
        char buff[BUFSIZ] = {'0'};
        cout << _func_args->client->client_ip << " 客户端online"<<endl;
        stringstream stream;
        while(read(_func_args->client->client_fd, buff, BUFSIZ)){ //客户端断开返回0
            stream<<buff;
            cout<<stream.str()<<endl;
            memset(buff, 0, BUFSIZ);
            stream.str("");
        }
        cout << _func_args->client->client_ip << " 客户端offline"<<endl;
    }
    close(_func_args->client->client_fd); //关闭对应客户端套接字
    pthread_exit(NULL);
}



int main(int argc, const char * argv[]) {
//    if (argc != 3){
//        fprintf(stderr,"Useage: %s <ip> <port>", argv[0]);
//        exit(-1);
//    }
#if 1
    Socket *sock = new Socket();
    sock->newServer("0.0.0.0", 8181);
    if (sock->bindAndListen(100)){
        while (true) {
//            process_client_data = process_client_request;
            std::stringstream *stream = new std::stringstream(""); //一个链接一个stream
            sock->MulThreadAccept(process_client_request, new Func_args(stream));
        }
    }
    delete sock;
#endif
#if 0
   Mysql mysql("127.0.0.1", 3306, "root", "root", "smarthome");
   if(mysql.connection()){
       mysql.query("select * from user");
	   int row_nums = mysql.getRowNum();
	   int col_nums = mysql.getColNum();
	   MYSQL_FIELD *fields = mysql.getFields();
	   for(int i = 0; i < row_nums; i++){
	   		for(int j = 0 ; j < col_nums; j++){
				cout<<mysql.json_value[i][fields[j].name];
			}
	   }

   }
   else{
   	cout<<"connect failed"<<endl;
   }
#endif
    return 0;
}
