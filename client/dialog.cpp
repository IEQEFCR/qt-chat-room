#include "dialog.h"
#include "ui_dialog.h"
#include  <QMessageBox>

Dialog::Dialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::Dialog)
{
    ui->setupUi(this);
    chatwindow = new chatWindow(this); //新建一个聊天窗口，它的父指针指向现在这个登录窗口
}

Dialog::~Dialog(){
    delete ui;
}


void Dialog::on_pushButton_clicked(){
    chatwindow->my_host.connectToHost(ui->lineEdit->text().toLatin1(),ui->lineEdit_2->text().toInt()); //连接lineEdit里面用户输入的IP和端口
    chatwindow->my_host.write("$%"+ui->lineEdit_3->text().toUtf8()); /*这个是我编造的一个发送注册信息的方法，
                                                                       就是在昵称lineEdit前面加上两个奇妙字符，
                                                                       向服务端说明现在发过去的是昵称*/
    chatwindow->show(); //显示聊天窗口
    this->hide();  //把当前登录界面隐藏
}
