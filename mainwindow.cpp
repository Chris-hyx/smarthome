#include "mainwindow.h"
#include "ui_mainwindow.h"
#include "success.h"
#include <QDebug>
using namespace std;



MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}


void MainWindow::on_login_clicked()
{
    QString username = ui->username->text();
    QString password = ui->password->text();
//    qDebug() << username << password;
    if(username.size() != 0 && password.size()!=0){
        ui->msg->setText("");
        if(!username.compare("lh") && !password.compare("123")){
            success *index = new success();
            index->setWindowTitle("智能家居控制系统");
            this->close();
            index->show();
        }else {
            ui->msg->setText("密码或者用户名错误");
        }
    }
}
