#ifndef SMARTHOME_SERVER_H
#define SMARTHOME_SERVER_H

#endif // SMARTHOME_SERVER_H
#include <QObject>
#include <QUdpSocket>

class NetworkManage : public QObject{
    Q_OBJECT
public:
    explicit NetworkManage(QObject *parent = nullptr);
    ~NetworkManage();
signals:
public slots:
    void udpRecvData(void);
    void sendMsg(QString str);
private:
    QUdpSocket *udpSocket;
};

