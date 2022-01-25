#include <QCoreApplication>
#include <QtNetwork>
#include <queue>
#include <string>
#include <QTime>
#include <QCoreApplication>

#ifndef SERVER_H
#define SERVER_H


class Client{
public:
    QByteArray name;
    QByteArray ip;
    QTcpSocket *socket;
    Client (QTcpSocket *socketx){
        socket=socketx;
        ip=socket->peerAddress().toString().toUtf8();
        name="匿名";
    }
};

class Server:public QObject{
    Q_OBJECT
    QTcpServer *tcpServer;
public:
    Server(QObject *parent = nullptr);
private:
    QDateTime cur_time ;
    int client_size;
    std::queue<Client>client;
    std::queue<QByteArray> history_msg;
    void deleteDeadSocket();
private slots:
    void sendMessage();
    void getThenSendMsg();
};


#endif // SERVER_H
