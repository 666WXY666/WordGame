#ifndef MY_THREAD_H
#define MY_THREAD_H

#include <QThread>
#include<QTcpServer>
#include<QTcpSocket>
#include<QSqlError>
#include<QSqlQuery>
#include<QMessageBox>
#include <QIcon>
#include<QPushButton>
#include<QPropertyAnimation>
#include <QCloseEvent>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QDebug>
#include<QCheckBox>
#include<QString>
class my_thread : public QThread
{
    Q_OBJECT
public:
    explicit my_thread(QObject *parent = nullptr);
    int flag;
    void init_database();
    QTcpSocket *tcpsocket;
    QTcpServer *tcpserver;
protected:
    void run();

signals:
    void is_done();
private:
    QSqlDatabase db;
    QSqlDatabase db2;
};

#endif // MY_THREAD_H
