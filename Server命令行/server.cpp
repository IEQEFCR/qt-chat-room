#include "server.h"


Server::Server(QObject *parent) : QObject(parent)
{
    tcpServer = new QTcpServer;
    cur_time = QDateTime::currentDateTime();
    if(!tcpServer->listen(QHostAddress::Any,7777)){
          qDebug()<<"QT_TCP Failed! :-(\n";
    }
    QObject::connect(tcpServer,&QTcpServer::newConnection,this,&Server::sendMessage);
    client_size=0;
}

void Server::deleteDeadSocket(){
    int deadNum=0;
    for(int i=1;i<=client_size;i++){
        if(client.front().socket->state()==QAbstractSocket::UnconnectedState){
            client.pop();
            deadNum++;
        }else {
            client.push(client.front());
            client.pop();
        }
    }
    client_size-=deadNum;

    if(deadNum){
     //   ui->listWidget->clear();
        for(int i=1;i<=client_size;i++){
            client.push(client.front());
            client.pop();
            if(client.back().name==nullptr) continue;
            //ui->listWidget->addItem(client.back().ip.mid(7)+" : "+client.back().name);
        }
    }

    return ;
}

void Server::getThenSendMsg(){
    QByteArray msgToSend;
    deleteDeadSocket();

    for(int i=1;i<=client_size;i++){
        client.push(client.front());
        client.pop();
        if(client.back().socket->isReadable()){
            msgToSend=client.back().socket->readAll();
            if(msgToSend[0]=='$'&&msgToSend[1]=='%'){
                client.back().name=msgToSend.mid(2);
             //   ui->listWidget->addItem(client.back().ip.mid(7)+" : "+client.back().name);
                return ;
            }
            if(msgToSend.isEmpty()) continue;
            msgToSend=cur_time.currentDateTime().toString().toUtf8().mid(7)+" "+client.front().name+"\n"+msgToSend;

            for(int i=1;i<=client_size;i++){
                client.front().socket->write(msgToSend,strlen(msgToSend));
                client.push(client.front());
                client.pop();
             }
            history_msg.push(msgToSend);
        }

    }
}

void Server::sendMessage(){
    qDebug()<<"A new connection!\n";
    deleteDeadSocket();
    client.push(Client(tcpServer->nextPendingConnection()));

    ++client_size;
    QObject::connect(client.back().socket,&QTcpSocket::readyRead,this,&Server::getThenSendMsg);
    int msg_size=history_msg.size();//send history_msg
    for(int i=1;i<=msg_size;i++){
        history_msg.push(history_msg.front());
        history_msg.pop();
        if(client.back().socket->isValid()){
            int state = client.back().socket->write(history_msg.back(),strlen(history_msg.back()));
         //   if(state == -1) QMessageBox::information(this,"QT_TCP","Failed to send message! :-(");
        }else {

       //     QMessageBox::information(this,"QT_TCP","Socket is invalid :-(");
        }
    }

}
