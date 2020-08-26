#include "smarthome_server.h"

NetworkManage::NetworkManage(QObject *parent) : QObject(parent){
    udpSocket = new QUdpSocket;
    udpSocket->bind(8080);
    QObject::connect(udpSocket, SIGNAL(readyRead()), this, SLOT(udpRecvData()));
}

NetworkManage::~NetworkManage(){
    delete udpSocket;
}

void NetworkManage::sendMsg(QString str){
    udpSocket->writeDatagram(str.toUtf8(), QHostAddress::LocalHost, 8081);
}

void NetworkManage::udpRecvData(){
    QByteArray datagram;
    qint64 len;
    while (udpSocket->hasPendingDatagrams()) {
        len = udpSocket->pendingDatagramSize();
        datagram.resize();
        udpSocket->readDatagram(datagram.data(), len);
        qDebug() << "data :" <<QString(datagram);
    }
}
