#include "registe.h"
#include "ui_registe.h"

registe::registe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registe)
{
    ui->setupUi(this);
    init_database();
    init_UI();
    connect_event();
}
void registe::init_database()
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
    query1.exec("create database if not exists word_game_v1 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;");

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("wxy");
    db.setPassword("123456");
    db.setDatabaseName("word_game_v1");
    if(!db.open())
    {
        QMessageBox::warning(this,"打开数据库错误",db.lastError().text());
        return;
    }
//    QSqlQuery query;
//    query.exec("create table user(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,experience int default 0,passed_num int default 0);");
//    query.exec("create table creater(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,create_num int default 0);");
}
void registe::init_UI()
{
    //背景大小设置
    setFixedSize(1200,500);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://resource/background1.jpg").scaled(1200,500,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    //标题图标设置
    this->setWindowTitle("单词消除游戏<注册游戏账户>");
    QIcon exe_iocn("://resource/logo.png");
    this->setWindowIcon(exe_iocn);
    //按键文本框设置
    ui->username_lineEdit->setEnabled(true);
    ui->password_lineEdit->setEnabled(true);
}
void registe::connect_event()
{
    connect(ui->cancel_pushButton,&QPushButton::clicked,this,&registe::send_change_signal);
}
void registe::showEvent(QShowEvent *)
{
    //渐变出现
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &registe::show);
    init_UI();
}
void registe::close_Widget()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &registe::close);
}
void registe::send_change_signal()
{
    emit change_widget_signal();
}
registe::~registe()
{
    delete ui;
}

void registe::on_complete_pushButton_clicked()
{
    QString username,password;
    username=ui->username_lineEdit->text();
    password=ui->password_lineEdit->text();
    if(username.isEmpty()||password.isEmpty())
    {
        QMessageBox::warning(this,"注册失败","账号或密码为空");
        return;
    }
    QString text=QString("用户名：%1\n密码：%2").arg(username).arg(password);
    int flag=QMessageBox::question(this,"您确定吗？",text);
    if(flag==QMessageBox::Yes)
    {
        ui->username_lineEdit->setEnabled(false);
        ui->password_lineEdit->setEnabled(false);
        QSqlQuery query;
        QString sql_select=QString("select username from user where username='%1';").arg(username);
        if(this->type_flag==0)
        {
            sql_select=QString("select username from user where username='%1';").arg(username);
        }
        else if (this->type_flag==1)
        {
            sql_select=QString("select username from creater where username='%1';").arg(username);
        }
        query.exec(sql_select);
        if(query.next())
        {
            QMessageBox::warning(this,"注册失败","账号已经存在");
            ui->username_lineEdit->setEnabled(true);
            ui->password_lineEdit->setEnabled(true);
            return;
        }
        else
        {
            QString sql=QString("insert into user(username,password) value('%1','%2');").arg(username).arg(password);
            if(this->type_flag==0)
            {
                sql=QString("insert into user(username,password) value('%1','%2');").arg(username).arg(password);
            }
            else if (this->type_flag==1)
            {
                sql=QString("insert into creater(username,password) value('%1','%2');").arg(username).arg(password);
            }
            query.exec(sql);
            ui->username_lineEdit->clear();
            ui->password_lineEdit->clear();
            QMessageBox::information(this,"注册成功","注册成功");
            send_change_signal();
        }
    }
}
