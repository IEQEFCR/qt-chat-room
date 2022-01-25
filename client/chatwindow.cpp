#include "chatwindow.h"
#include "ui_chatwindow.h"
#include <QTextCodec>

chatWindow::chatWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::chatWindow)
{
    ui->setupUi(this);
    connect(&my_host,&QTcpSocket::readyRead,this,&chatWindow::readMsg); //设置有信息读取信号就执行读信息
}

chatWindow::~chatWindow()
{
    delete ui;
}

void chatWindow::readMsg(){
    char receivedData[1024]={0}; //单次应该最多接受1024/4=256个汉字（一个汉字好像占4个字节？GBK2312）
    my_host.read(receivedData,1024);//用char数组读套接字
    QByteArray temp=receivedData;  //然后用QByte转换一下
    ui->textBrowser->setHtml(ui->textBrowser->toHtml()+temp);  //向聊天窗口增加消息记录，服务器那边自动加了换行符‘\n’,这里toHtml是想支持富文本
                                                               //不过我输入窗口的富文本输入功能还没有23333，只能去复制写好的富文本
                                                               //我实测可以支持emoji，不过表情选择窗口也还没写
}

void chatWindow::on_pushButton_clicked()
{
    my_host.write(ui->textEdit->toHtml().toUtf8()); //发送
    ui->textEdit->clear(); // 发送完清空输入框
}
