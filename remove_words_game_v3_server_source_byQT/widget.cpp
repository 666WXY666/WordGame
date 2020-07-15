#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    tcpserver=nullptr;
    init_UI();
    tcpserver=new QTcpServer(this);
    tcpserver->listen(QHostAddress::Any,8888);
    connect(tcpserver,&QTcpServer::newConnection,this,&Widget::tcpserver_connect);
}

void Widget::showEvent(QShowEvent *)
{
    //渐变出现
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &Widget::show);
}

void Widget::tcpserver_connect()
{
    qDebug()<<0;
    tcpsocket=tcpserver->nextPendingConnection();
    QString ip=tcpsocket->peerAddress().toString();
    qint16 port=tcpsocket->peerPort();
    QString str=QString("[%1,%2]:connect success").arg(ip).arg(port);
    ui->textBrowser->append(str);
    connect(tcpsocket,&QTcpSocket::readyRead,this,&Widget::tcpsocket_connect);
}

void Widget::tcpsocket_connect()
{
    QByteArray array=tcpsocket->readAll();
    QString str(array);
    qDebug()<<str;
    if(str==QString("request user info"))
    {
        qDebug()<<1;
        my_thread *thread=new my_thread;
        thread->tcpserver=tcpserver;
        thread->tcpsocket=tcpsocket;
        thread->flag=1;
        QString str1=QString("[%1,%2]:request user info").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
        ui->textBrowser->append(str1);
        tcpsocket->write(QString("ready").toUtf8().data());
        thread->start();
        connect(thread,&my_thread::is_done,[=]()
                {
                    QString str2=QString("[%1,%2]:send user info over").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
                    ui->textBrowser->append(str2);
                    thread->quit();
                    thread->wait();
                }
                );
    }
    else if (str==QString("request creater info"))
    {
        my_thread *thread=new my_thread;
        thread->tcpserver=tcpserver;
        thread->tcpsocket=tcpsocket;
        thread->flag=2;
        tcpsocket->write(QString("ready").toUtf8().data());
        QString str1=QString("[%1,%2]:request creater info").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
        ui->textBrowser->append(str1);
        thread->start();
        connect(thread,&my_thread::is_done,[=]()
                {
                    QString str2=QString("[%1,%2]:send creater info over").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
                    ui->textBrowser->append(str2);
                    thread->quit();
                    thread->wait();
                }
                );
    }
    else if (str==QString("request word"))
    {
        my_thread *thread=new my_thread;
        thread->tcpserver=tcpserver;
        thread->tcpsocket=tcpsocket;
        thread->flag=3;
        tcpsocket->write(QString("READY!").toUtf8().data());
        QString str1=QString("[%1,%2]:request word").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
        ui->textBrowser->append(str1);
        thread->start();
        connect(thread,&my_thread::is_done,[=]()
                {
                    QString str2=QString("[%1,%2]:send word over").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
                    ui->textBrowser->append(str2);
                    thread->quit();
                    thread->wait();
                }
                );
    }
    else if (str==QString("request update"))
    {
        my_thread *thread=new my_thread;
        thread->tcpserver=tcpserver;
        thread->tcpsocket=tcpsocket;
        thread->flag=4;
        tcpsocket->write(QString("request info").toUtf8().data());
        QString str1=QString("[%1,%2]:request update").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
        ui->textBrowser->append(str1);
        thread->start();
        connect(thread,&my_thread::is_done,[=]()
                {
                    QString str2=QString("[%1,%2]:update over").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
                    ui->textBrowser->append(str2);
                    thread->quit();
                    thread->wait();
                }
                );
    }
    else if (str==QString("request create"))
    {
        my_thread *thread=new my_thread;
        thread->tcpserver=tcpserver;
        thread->tcpsocket=tcpsocket;
        thread->flag=5;
        tcpsocket->write(QString("request word").toUtf8().data());
        QString str1=QString("[%1,%2]:request create").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
        ui->textBrowser->append(str1);
        thread->start();
        connect(thread,&my_thread::is_done,[=]()
                {
                    QString str2=QString("[%1,%2]:create over").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
                    ui->textBrowser->append(str2);
                    thread->quit();
                    thread->wait();
                }
                );
    }
    else if (str==QString("request registe"))
    {
        my_thread *thread=new my_thread;
        thread->tcpserver=tcpserver;
        thread->tcpsocket=tcpsocket;
        thread->flag=6;
        tcpsocket->write(QString("request info").toUtf8().data());
        QString str1=QString("[%1,%2]:request registe").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
        ui->textBrowser->append(str1);
        thread->start();
        connect(thread,&my_thread::is_done,[=]()
                {
                    QString str2=QString("[%1,%2]:registe over").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
                    ui->textBrowser->append(str2);
                    thread->quit();
                    thread->wait();
                }
                );
    }
    else if (str==QString("request check"))
    {
        my_thread *thread=new my_thread;
        thread->tcpserver=tcpserver;
        thread->tcpsocket=tcpsocket;
        thread->flag=7;
        tcpsocket->write(QString("request info").toUtf8().data());
        QString str1=QString("[%1,%2]:request check").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
        ui->textBrowser->append(str1);
        thread->start();
        connect(thread,&my_thread::is_done,[=]()
                {
                    QString str2=QString("[%1,%2]:check over").arg(thread->tcpsocket->peerAddress().toString()).arg(thread->tcpsocket->peerPort());
                    ui->textBrowser->append(str2);
                    thread->quit();
                    thread->wait();
                }
                );
    }
    disconnect(tcpsocket,&QTcpSocket::readyRead,this,&Widget::tcpsocket_connect);
}

Widget::~Widget()
{
    delete ui;
}

void Widget::init_UI()
{
    //背景大小设置
    setFixedSize(600,900);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://background.jpg").scaled(600,900,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    //标题图标设置
    this->setWindowTitle("单词消除游戏<服务器>");
    QIcon exe_iocn("://logo.ico");
    this->setWindowIcon(exe_iocn);
}
