#ifndef SERVER_H
#define SERVER_H

#include <QDialog>
#include <QtNetwork>
#include <queue>
#include <string>
#include <QTime>

QT_BEGIN_NAMESPACE
namespace Ui { class Server; }
QT_END_NAMESPACE

class Client{  //封装的一个客户端类型
public:
    QByteArray name;
    QByteArray ip;
    QTcpSocket *socket;
    Client (QTcpSocket *socketx);  //客户端构造
};

class Server : public QDialog{
    Q_OBJECT
    QTcpServer *tcpServer;  //qt封装好的tcp服务器类
public:
    Server(QWidget *parent = nullptr);
    ~Server();
private:
    QDateTime cur_time ; //时间类，为了转发消息记录的时候打上时间
    Ui::Server *ui;
    std::queue<Client>client;//客户端队列，用队列是为了能够动态删除断开的客户端，不过遍历每个客户端有点麻烦，
                                                                            //改成链表和动态内存申请可能会好一点
    std::queue<QByteArray> history_msg;//历史消息记录
    int client_size; //客户端数量，其实那个队列可以直接调用size()，改回来有点麻烦
    
    void deleteDeadSocket();
private slots:
    void sendMessage();
    void on_pushButton_clicked();
    void getThenSendMsg();
};

#endif // SERVER_H
