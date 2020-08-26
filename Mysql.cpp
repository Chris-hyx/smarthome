//
//  Mysql.cpp
//  Socket
//
//  Created by LH'MAC on 2020/8/23.
//  Copyright © 2020年 longmiaohao. All rights reserved.
//

#include "Mysql.hpp"


Mysql::Mysql(string host, unsigned int port, string username, string password,string db):host(host),port(port),username(username),password(password), db(db){
       this->mysql = mysql_init(NULL);
       result = NULL;
       row = NULL;
};

Mysql::~Mysql(){
       if(mysql)
               mysql_close(mysql); //释放资源
};

bool Mysql::connection(){   //连接数据库
       mysql = mysql_real_connect(mysql, host.c_str(), username.c_str(), password.c_str(), db.c_str(), port, NULL, 0);
       if(mysql){
               return true;
       }
       perror(mysql_error(mysql));
       return false;
};

bool Mysql::query(string sql){
       if(mysql_query(mysql, sql.c_str())){
               perror(mysql_error(mysql));
               return false;
       }
       result = mysql_store_result(mysql); //储存查询结果
       if(result){
               //获取结果集中总共的字段数，即列数
               this->col_nums = mysql_num_fields(result); //字段数量 列数
               this->row_nums = mysql_num_rows(result); //记录数量 行数
               this->fields = mysql_fetch_fields(result);
               for(unsigned int i = 0; i < row_nums; i++){
                       row = mysql_fetch_row(result); //获取一条
                       if(row){
                               for(int j = 0; j < col_nums; j++){
                                       json_value[i][fields[j].name] = row[j];
                               }
                       }
               }
       }
       return true;
};

string Mysql::getJsonValue(unsigned long index, string key){
       if(index >= this->row_nums){
               cout<<"index is out of bounds"<<endl;
               return "";
       }
       if (key == "")
               cout<<"warning, key is no set"<<endl;
       return json_value[index][key].asString();
};

unsigned long Mysql::getRowNum(){
       return this->row_nums;
};

unsigned int Mysql::getColNum(){
       return this->col_nums;
};

MYSQL_FIELD* Mysql::getFields(){
       return this->fields;
};

//int main(void){
//       Mysql mysql("127.0.0.1", 3306, "root", "root", "my_blog");
//       if(mysql.connection()){
//               mysql.query("select * from user_userinfo");
//       }
//       return 0;
//};

