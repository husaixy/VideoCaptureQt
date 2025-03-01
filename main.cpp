#include "mainwindow.h"
#include <QApplication>
#include <iostream>
#include <locale>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <cstring>
int main(int argc, char *argv[])
{


    //应用程序类只能有一个（用qt的基本类时在上面添加对应的头文件）
    QApplication a(argc, argv);
//    窗口对象
    MainWindow w;
    w.show();


    //exec（）阻塞函数，程序进入事件循环
    return a.exec();
}
