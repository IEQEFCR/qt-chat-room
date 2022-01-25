#ifndef DIALOG_H
#define DIALOG_H

#include <QDialog>
#include <QtNetwork>
#include "chatwindow.h" //主聊天窗口

QT_BEGIN_NAMESPACE
namespace Ui { class Dialog; }
QT_END_NAMESPACE

class Dialog : public QDialog
{
    Q_OBJECT

public:
    Dialog(QWidget *parent = nullptr);
    ~Dialog();

private slots:
    void on_pushButton_clicked();  //登录按钮对接的槽函数
private:
    Ui::Dialog *ui;
    chatWindow *chatwindow; //聊天窗口
};
#endif // DIALOG_H
