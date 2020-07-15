#include "create.h"
#include "ui_create.h"

create::create(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::create)
{
    ui->setupUi(this);
    init_database();
    init_UI();
    connect(ui->back_pushButton,&QPushButton::clicked,this,&create::send_change_signal);
}
void create::init_database()
{
    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db2 = QSqlDatabase::database("qt_sql_default_connection");
    else
        db2 = QSqlDatabase::addDatabase("QMYSQL","1");
    db2.setHostName("127.0.0.1");
    db2.setUserName("wxy");
    db2.setPassword("123456");
    db2.setDatabaseName("sys");
    if(!db2.open())
    {
        QMessageBox::warning(this,"打开数据库错误",db2.lastError().text());
        return;
    }
    QSqlQuery query1(db2);
    query1.exec("create database if not exists word_game_v3 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;");

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("wxy");
    db.setPassword("123456");
    db.setDatabaseName("word_game_v3");
    if(!db.open())
    {
        QMessageBox::warning(this,"打开数据库错误",db.lastError().text());
        return;
    }
//    QSqlQuery query;
//    query.exec("create table user(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,experience int default 0,passed_num int default 0);");
//    query.exec("create table creater(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,create_num int default 0);");
//    query.exec("create table word(id int primary key auto_increment,word varchar(255));");
}
void create::init_UI()
{
    ui->input_lineEdit->setReadOnly(false);
    ui->input_lineEdit->setFocus();
    //背景大小设置
    setFixedSize(500,500);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://resource/background6.jpg").scaled(500,500,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    //标题图标设置
    this->setWindowTitle("单词消除游戏<出题人模式>");
    QIcon exe_iocn("://resource/logo.png");
    this->setWindowIcon(exe_iocn);
}
void create::showEvent(QShowEvent *)
{
    //渐变出现
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &create::show);
    init_UI();
}
void create::close_Widget()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &create::close);
}
void create::send_change_signal()
{
    emit change_widget_signal();
}
create::~create()
{
    delete ui;
}

void create::on_confirm_pushButton_clicked()
{
    ui->input_lineEdit->setReadOnly(true);
    QString input=ui->input_lineEdit->text();
    if(!input.isEmpty())
    {
        QTcpSocket *tcpsocket=new QTcpSocket(this);
        QString ip=QString("127.0.0.1");
        qint16 port=8888;
        tcpsocket->connectToHost(QHostAddress(ip),port);
        tcpsocket->write(QString("request create").toUtf8().data());
        connect(tcpsocket,&QTcpSocket::readyRead,[=]()
            {
            QByteArray array=tcpsocket->readAll();
            QString str(array);
            qDebug()<<str;
            if(str==QString("error"))
            {
                QMessageBox::warning(this,"出题失败","该单词已在题库中");
                tcpsocket->disconnectFromHost();
                tcpsocket->close();
            }
            else if(str==QString("request word"))
            {
                tcpsocket->write(QString("%1,%2").arg(input).arg(username).toUtf8().data());
            }
            else if(str==QString("ok"))
            {
                int experience=0;
                int level=1;
                int create_word=0;
                QSqlQuery query2;
                QString sql2=QString("select * from creater where username='%1';").arg(username);
                query2.exec(sql2);
                if (query2.next())
                {
                    level=query2.value("level").toInt();
                    experience=query2.value("experience").toInt()+1;
                    create_word=query2.value("create_num").toInt()+1;
                    for(;experience>=level;)
                    {
                        experience=experience-level;
                        level++;
                    }
                    QSqlQuery query3;
                    QString sql3=QString("update creater set create_num=%1 where username='%2';").arg(create_word).arg(username);
                    query3.exec(sql3);
                    QSqlQuery query4;
                    QString sql4=QString("update creater set level=%1 where username='%2';").arg(level).arg(username);
                    query4.exec(sql4);
                    QSqlQuery query5;
                    QString sql5=QString("update creater set experience=%1 where username='%2';").arg(experience).arg(username);
                    query5.exec(sql5);
                }
                tcpsocket->disconnectFromHost();
                tcpsocket->close();
            }
            });
    }
    else
    {
        QMessageBox::warning(this,"出题失败","输入为空");
    }
    ui->input_lineEdit->clear();
    init_UI();
}
void create::keyPressEvent(QKeyEvent *event)
{
    if(event->key()==Qt::Key_Enter||event->key()==Qt::Key_Return)
    {
        ui->input_lineEdit->setReadOnly(true);
        QString input=ui->input_lineEdit->text();
        if(!input.isEmpty())
        {
            QTcpSocket *tcpsocket=new QTcpSocket(this);
            QString ip=QString("127.0.0.1");
            qint16 port=8888;
            tcpsocket->connectToHost(QHostAddress(ip),port);
            tcpsocket->write(QString("request create").toUtf8().data());
            connect(tcpsocket,&QTcpSocket::readyRead,[=]()
                {
                QByteArray array=tcpsocket->readAll();
                QString str(array);
                qDebug()<<str;
                if(str==QString("error"))
                {
                    QMessageBox::warning(this,"出题失败","该单词已在题库中");
                }
                else if(str==QString("request word"))
                {
                    tcpsocket->write(QString("%1,%2").arg(input).arg(username).toUtf8().data());
                }
                else if(str==QString("ok"))
                {
                    int experience=0;
                    int level=1;
                    int create_word=0;
                    QSqlQuery query2;
                    QString sql2=QString("select * from creater where username='%1';").arg(username);
                    query2.exec(sql2);
                    if (query2.next())
                    {
                        level=query2.value("level").toInt();
                        experience=query2.value("experience").toInt()+1;
                        create_word=query2.value("create_num").toInt()+1;
                        for(;experience>=level;)
                        {
                            experience=experience-level;
                            level++;
                        }
                        QSqlQuery query3;
                        QString sql3=QString("update creater set create_num=%1 where username='%2';").arg(create_word).arg(username);
                        query3.exec(sql3);
                        QSqlQuery query4;
                        QString sql4=QString("update creater set level=%1 where username='%2';").arg(level).arg(username);
                        query4.exec(sql4);
                        QSqlQuery query5;
                        QString sql5=QString("update creater set experience=%1 where username='%2';").arg(experience).arg(username);
                        query5.exec(sql5);
                    }
                    tcpsocket->disconnectFromHost();
                    tcpsocket->close();
                }
                });
        }
        else
        {
            QMessageBox::warning(this,"出题失败","输入为空");
        }
        ui->input_lineEdit->clear();
        init_UI();
    }
}
