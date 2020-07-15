#include "widget.h"
#include "ui_widget.h"

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);
    init_database();
    init_UI();
    connect_event();
}
void Widget::init_database()
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
    QSqlQuery query;
    query.exec("create table if not exists user(id int primary key auto_increment,username varchar(255),level int default 1,experience int default 0,passed_num int default 0);");
    query.exec("create table if not exists creater(id int primary key auto_increment,username varchar(255),level int default 1,experience int default 0,create_num int default 0);");
//    query.exec("create table if not exists word(id int primary key auto_increment,word varchar(255),creater varchar(255));");
}
void Widget::init_UI()
{
    type_changed();
    ui->login_pushButton->setEnabled(true);
    //背景大小设置
    setFixedSize(480,950);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://resource/background.jpg").scaled(480,950,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    //标题图标设置
    this->setWindowTitle("单词消除游戏<登录>");
    QIcon exe_iocn("://resource/logo.png");
    this->setWindowIcon(exe_iocn);
}
void Widget::connect_event()
{
    connect(ui->registe_user_pushButton,&QPushButton::clicked,this,&Widget::user_registe_show);
    connect(ui->regist_creater_pushButton,&QPushButton::clicked,this,&Widget::creater_registe_show);
    connect(ui->exit_pushButton,&QPushButton::clicked,this,&Widget::close_Widget);
    connect(&registe_widget,&registe::change_widget_signal,this,&Widget::registe_close);
    connect(ui->type_checkBox,&QCheckBox::stateChanged,this,&Widget::type_changed);
    connect(ui->check_user_pushButton,&QPushButton::clicked,this,&Widget::check_user_show);
    connect(ui->check_creater_pushButton,&QPushButton::clicked,this,&Widget::check_creater_show);
    connect(&check_widget,&check::change_widget_signal,this,&Widget::check_close);
    connect(&info_widget,&info::change_widget_signal,this,&Widget::info_close);
}
void Widget::type_changed()
{
    if(ui->type_checkBox->isChecked()==true)
    {
        this->type_flag=1;
    }
    else
    {
        this->type_flag=0;
    }
}
void Widget::user_registe_show()
{
    registe_widget.type_flag=0;
    registe_widget.show();
    this->close_Widget();
}
void Widget::creater_registe_show()
{
    registe_widget.type_flag=1;
    registe_widget.show();
    this->close_Widget();
}
void Widget::registe_close()
{
    registe_widget.close_Widget();
    this->show();
}
void Widget::check_user_show()
{
    check_widget.type_flag=0;
    check_widget.show();
    this->close_Widget();
}
void Widget::check_creater_show()
{
    check_widget.type_flag=1;
    check_widget.show();
    this->close_Widget();
}
void Widget::check_close()
{
    check_widget.close_Widget();
    this->show();
}
void Widget::info_show()
{
    info_widget.show();
    this->close_Widget();
}
void Widget::info_close()
{
    info_widget.close_Widget();
    this->show();
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
    init_UI();
}
void Widget::close_Widget()
{
    ui->username_lineEdit->clear();
    ui->password_lineEdit->clear();
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &Widget::close);
}
Widget::~Widget()
{
    QSqlQuery query;
    query.exec("drop database if exists word_game_v3;");
    delete ui;
}

void Widget::on_login_pushButton_clicked()
{
    QTcpSocket *tcpsocket=new QTcpSocket(this);
    QString ip=QString("127.0.0.1");
    qint16 port=8888;
    tcpsocket->connectToHost(QHostAddress(ip),port);
    QString username,password;
    username=ui->username_lineEdit->text();
    password=ui->password_lineEdit->text();
    if(this->type_flag==0)
    {
        tcpsocket->write(QString("request user info").toUtf8().data());
    }
    else
    {
        tcpsocket->write(QString("request creater info").toUtf8().data());
    }
    connect(tcpsocket,&QTcpSocket::readyRead,[=]()
        {
            QByteArray array=tcpsocket->readAll();
            QString str(array);
            if(str==QString("ready"))
            {
                tcpsocket->write(username.toUtf8().data());
            }
            else if(str==QString("error"))
            {
                QMessageBox::warning(this,"登录失败","账号不存在");
                tcpsocket->disconnectFromHost();
                tcpsocket->close();
            }
            else
            {
                QString password_str=str.section(',',0,0);
                int num=str.section(',',1,1).toInt();
                int experience=str.section(',',2,2).toInt();
                int level=str.section(',',3,3).toInt();
                qDebug()<<str;
                if(this->type_flag==0)
                {
                    if(password==password_str)
                    {
                        info_widget.username=username;
                        info_widget.type_flag=0;
                        ui->login_pushButton->setEnabled(false);
                        QSqlQuery query;
                        QString sql_select=QString("select username from user where username='%1';").arg(username);
                        query.exec(sql_select);
                        if(query.next())
                        {
                            QSqlQuery query1;
                            QString sql1=QString("update user set experience=%1 where username='%2';").arg(experience).arg(username);
                            QString sql2=QString("update user set level=%1 where username='%2';").arg(level).arg(username);
                            QString sql3=QString("update user set passed_num=%1 where username='%2';").arg(num).arg(username);
                            query1.exec(sql1);
                            query1.exec(sql2);
                            query1.exec(sql3);
                        }
                        else
                        {
                            QString sql=QString("insert into user(username,level,experience,passed_num) value('%1',%2,%3,%4);").arg(username).arg(level).arg(experience).arg(num);
                            query.exec(sql);
                            qDebug()<<sql;
                        }
                        info_show();
                        tcpsocket->disconnectFromHost();
                        tcpsocket->close();
                    }
                    else
                    {
                        QMessageBox::warning(this,"登录失败","密码错误");
                        tcpsocket->disconnectFromHost();
                        tcpsocket->close();
                    }
                }
                else if(this->type_flag==1)
                {
                    if(password==password_str)
                    {
                        info_widget.username=username;
                        info_widget.type_flag=1;
                        ui->login_pushButton->setEnabled(false);
                        QSqlQuery query;
                        QString sql_select=QString("select username from creater where username='%1';").arg(username);
                        query.exec(sql_select);
                        qDebug()<<1111111;
                        if(query.next())
                        {
                            qDebug()<<222222;
                            QSqlQuery query1;
                            QString sql1=QString("update creater set experience=%1 where username='%2';").arg(experience).arg(username);
                            QString sql2=QString("update creater set level=%1 where username='%2';").arg(level).arg(username);
                            QString sql3=QString("update creater set create_num=%1 where username='%2';").arg(num).arg(username);
                            query1.exec(sql1);
                            query1.exec(sql2);
                            query1.exec(sql3);
                        }
                        else
                        {
                            qDebug()<<3333333;
                            QString sql=QString("insert into creater(username,level,experience,create_num) value('%1',%2,%3,%4);").arg(username).arg(level).arg(experience).arg(num);
                            query.exec(sql);
                        }
                        info_show();
                        tcpsocket->disconnectFromHost();
                        tcpsocket->close();
                    }
                    else
                    {
                        QMessageBox::warning(this,"登录失败","密码错误");
                        tcpsocket->disconnectFromHost();
                        tcpsocket->close();
                    }
                }
            }
        }
        );
}
