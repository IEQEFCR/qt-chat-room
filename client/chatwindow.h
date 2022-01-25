#ifndef CHATWINDOW_H
#define CHATWINDOW_H

#include <QMainWindow>
#include <QtNetwork>

namespace Ui {
class chatWindow;
}

class chatWindow : public QMainWindow
{
    Q_OBJECT

public:
    chatWindow(QWidget *parent = nullptr);
    ~chatWindow();
    QTcpSocket my_host; //客户端的套接字
private slots:
    void on_pushButton_clicked(); //发送信息按钮对接的槽函数

private:
    Ui::chatWindow *ui;
    void readMsg(); //读取服务端收过来的信息
};

#endif // CHATWINDOW_H
