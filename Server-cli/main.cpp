#include <QCoreApplication>
#include <QtNetwork>
#include <queue>
#include <string>
#include <QTime>
#include <QCoreApplication>
#include "server.h"
#include <iostream>

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    qDebug()<<"Server starts！\n";
    Server my_server;
    return a.exec();
}
