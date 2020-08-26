//
//  Mysql.hpp
//  Socket
//
//  Created by LH'MAC on 2020/8/23.
//  Copyright © 2020年 longmiaohao. All rights reserved.
//

#ifndef Mysql_hpp
#define Mysql_hpp

#include "mysql/mysql.h"
#include <string>
#include <stdio.h>
#include <iostream>
#include "json/json.h"
using namespace std;
#endif /* Mysql_hpp */

class Mysql{
       private:
               MYSQL *mysql; // mysql句柄
               MYSQL_RES *result; //指向查询结果的指针
               MYSQL_ROW row; //按行返回的查询信息
               MYSQL_FIELD *fields; //字段名
               int col_nums; //字段数量
               unsigned long long  row_nums; //行数
               string host;
               unsigned int port;
               string username;
               string password;
               string db;
       public:
               Json::Value json_value;
               Mysql(string host, unsigned int port, string username, string password, string db); //初始化mysql
               ~Mysql(); //释放资源
               bool connection(); //连接数据库
               bool query(string sql);
               string getJsonValue(unsigned long index=0, string key="");
               unsigned long getRowNum();
               unsigned int getColNum();
               MYSQL_FIELD *getFields();
};

