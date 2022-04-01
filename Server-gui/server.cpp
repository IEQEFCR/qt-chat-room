#include "server.h"
#include "ui_server.h"
#include <QtNetwork>
#include <QMessageBox>
#include <QTime>

Client::Client(QTcpSocket *socketx){ //构造函数指定ip
        socket=socketx;
        ip=socket->peerAddress().toString().toUtf8();  //这个其实没什么必要2333333，当时写脑抽了2333
        name=nullptr; 
}

Server::Server(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Server)
{
    tcpServer = new QTcpServer(this);
    cur_time = QDateTime::currentDateTime();
    if(tcpServer->listen(QHostAddress::Any,6666)==0){ 
       QMessageBox::information(this,"QT_TCP","Failed! :-("); //输出失败信息，大概率是端口被占用了
       close();
    }

    QObject::connect(tcpServer,&QTcpServer::newConnection,this,&Server::sendMessage); //如果用新连接，就建立联系，
                                                                                      //这个槽函数名字是开始写聊天室调试的时候取的，来不及改了
                                                                                      //真正转发的函数是另外一个
    client_size=0;
    ui->setupUi(this);
}

void Server::deleteDeadSocket(){ //删除关闭的客户端
    int deadNum=0;
    for(int i=1;i<=client_size;i++){
        //队列的遍历很麻烦，有时间改成链表
        if(client.front().socket->state()==QAbstractSocket::UnconnectedState){
            client.pop();
            deadNum++;
        }else {
            client.push(client.front()); //重新加到队尾
            client.pop();
        }
    }
    client_size-=deadNum;

    if(deadNum){ //这个为了更新listWidget那边显示正在连接的客户端
        ui->listWidget->clear();
        for(int i=1;i<=client_size;i++){
            client.push(client.front());
            client.pop();
            if(client.back().name==nullptr) continue; //没名字就是还没注册昵称的（昵称信息还没发过来），不显示
            ui->listWidget->addItem(client.back().ip.mid(7)+" : "+client.back().name); //添加到listWidget中，ip前面部分截掉了
        }
    }

    return ;
}

void Server::getThenSendMsg(){ //转发函数
    QByteArray msgToSend;
    deleteDeadSocket();
    for(int i=1;i<=client_size;i++){  //遍历所有客户端，找到发信息的那个
        client.push(client.front());
        client.pop();
        if(client.back().socket->isReadable()){
            msgToSend=client.back().socket->readAll();
            if(msgToSend[0]=='$'&&msgToSend[1]=='%'){ //如果发过来的是注册昵称信息，那就添加客户端名字
                client.back().name=msgToSend.mid(2);
                ui->listWidget->addItem(client.back().ip.mid(7)+" : "+client.back().name);
                return ;
            }
            if(msgToSend.isEmpty()) continue;
            msgToSend=cur_time.currentDateTime().toString().toUtf8().mid(7)+" "+client.front().name+"\n"+msgToSend;
            //加上时间信息和发送者的名字
            for(int i=1;i<=client_size;i++){ //转发到所有客户端
                client.front().socket->write(msgToSend,strlen(msgToSend));
                client.push(client.front());
                client.pop();
            }
            history_msg.push(msgToSend);//加入到历史信息
        }

    }

}

void Server::sendMessage(){ //新客户端初始化
    ui->label->setText("Successful Connection");
    deleteDeadSocket();
    client.push(Client(tcpServer->nextPendingConnection())); //获取套接字
    ++client_size;
    QObject::connect(client.back().socket,SIGNAL(readyRead()),this,SLOT(getThenSendMsg()));//转发的槽函数连接
    int msg_size=history_msg.size();//发送历史信息
    for(int i=1;i<=msg_size;i++){
        history_msg.push(history_msg.front());
        history_msg.pop();
        if(client.back().socket->isValid()){
            int state = client.back().socket->write(history_msg.back(),strlen(history_msg.back()));
            if(state == -1) QMessageBox::information(this,"QT_TCP","Failed to send message! :-(");
        }else QMessageBox::information(this,"QT_TCP","Socket is invalid :-(");
    }
}

Server::~Server(){
    delete ui;
}

void Server::on_pushButton_clicked(){ //这个是调试用的，服务端自带一个发送信息（广播）功能
    for(int i=1;i<=client_size;i++){
        client.front().socket->write(ui->lineEdit1->text().toUtf8(),strlen(ui->lineEdit1->text().toUtf8()));
        client.push(client.front());
        client.pop();
    }
}


