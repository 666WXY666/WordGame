#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include<QMessageBox>
#include <QIcon>
#include<QPushButton>
#include<QPropertyAnimation>
#include <QCloseEvent>
#include<QMessageBox>
#include<QDebug>
#include<QCheckBox>
#include<QTcpServer>
#include<QTcpSocket>
#include"my_thread.h"
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    void init_UI();//初始化图形界面
    void tcpserver_connect();
    void tcpsocket_connect();
    void showEvent(QShowEvent *);
    ~Widget();

private:
    Ui::Widget *ui;
    QTcpServer *tcpserver;
    QTcpSocket *tcpsocket;
signals:
};


#endif // WIDGET_H
