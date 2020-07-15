#include "check.h"
#include "ui_check.h"

check::check(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::check)
{
    ui->setupUi(this);
    init_database();
    init_UI();
    connect_event();
}
void check::init_database()
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
    query.exec("drop table if exists check_user;");
    query.exec("drop table if exists check_creater;");
    query.exec("create table if not exists check_user(id int primary key auto_increment,username varchar(255),level int default 1,experience int default 0,passed_num int default 0);");
    query.exec("create table if not exists check_creater(id int primary key auto_increment,username varchar(255),level int default 1,experience int default 0,create_num int default 0);");
}
void check::init_UI()
{
    //背景大小设置
    setFixedSize(800,800);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://resource/background3.jpg").scaled(800,800,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    //标题图标设置
    this->setWindowTitle("单词消除游戏<查询信息>");
    QIcon exe_iocn("://resource/logo.png");
    this->setWindowIcon(exe_iocn);
    model=new QSqlTableModel(this);
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSortingEnabled(true);
    ui->type_comboBox->clear();
    ui->type_comboBox->addItem("用户名");
    ui->type_comboBox->addItem("等级");
    ui->type_comboBox->addItem("经验");
    if(type_flag==0)
    {
        ui->type_comboBox->addItem("已过关数");
        model->setHeaderData(0,Qt::Horizontal,"用户名");
        model->setHeaderData(1,Qt::Horizontal,"等级");
        model->setHeaderData(2,Qt::Horizontal,"经验");
        model->setHeaderData(3,Qt::Horizontal,"已过关数");
    }
    else if(type_flag==1)
    {
        ui->type_comboBox->addItem("已出题数");
        model->setHeaderData(0,Qt::Horizontal,"用户名");
        model->setHeaderData(1,Qt::Horizontal,"等级");
        model->setHeaderData(2,Qt::Horizontal,"经验");
        model->setHeaderData(3,Qt::Horizontal,"已出题数");
    }
    model->removeRows(0,model->rowCount());
    ui->tableView->setModel(model);
    model->select();

}
void check::connect_event()
{
    connect(ui->back_pushButton,&QPushButton::clicked,this,&check::send_change_signal);
}
void check::showEvent(QShowEvent *)
{
    //渐变出现
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &check::show);
    QSqlQuery query;
    query.exec("drop table if exists check_user;");
    query.exec("drop table if exists check_creater;");
    init_UI();
}
void check::close_Widget()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &check::close);
}
void check::send_change_signal()
{
    emit change_widget_signal();
}
check::~check()
{
    delete ui;
}
QString check::string_to_type()
{
    QString type=ui->type_comboBox->itemText(ui->type_comboBox->currentIndex());
    if(type==QString("用户名"))
    {
        return QString("username");
    }
    else if (type==QString("等级"))
    {
        return QString("level");
    }
    else if (type==QString("经验"))
    {
        return QString("experience");
    }
    else if (type==QString("已过关数"))
    {
        return QString("passed_num");
    }
    else if (type==QString("已出题数"))
    {
        return QString("create_num");
    }
    return type;
}
void check::on_check_pushButton_clicked()
{
    QString type=string_to_type();
    QString content=ui->content_lineEdit->text();
    if(content.isEmpty())
    {
        QMessageBox::warning(this,"搜索失败","搜索内容为空");
        return;
    }
    else
    {
        QTcpSocket *tcpsocket=new QTcpSocket(this);
        QString ip=QString("127.0.0.1");
        qint16 port=8888;
        tcpsocket->connectToHost(QHostAddress(ip),port);
        tcpsocket->write(QString("request check").toUtf8().data());
        connect(tcpsocket,&QTcpSocket::readyRead,[=]()
            {
                QString info;
                QByteArray array=tcpsocket->readAll();
                QString str(array);
                if(str==QString("request info"))
                {
                    info=QString("%1,%2,%3").arg(type_flag).arg(type).arg(content);
                    tcpsocket->write(info.toUtf8().data());
                    qDebug()<<info;
                }
                else if(str==QString("error"))
                {
                    QMessageBox::warning(this,"查找失败","信息不存在");
                    qDebug()<<"error";
                    tcpsocket->disconnectFromHost();
                    tcpsocket->close();
                }
                else
                {
                    QSqlQuery query0;
                    query0.exec("drop table if exists check_user;");
                    query0.exec("drop table if exists check_creater;");
                    query0.exec("create table if not exists check_user(id int primary key auto_increment,username varchar(255),level int default 1,experience int default 0,passed_num int default 0);");
                    query0.exec("create table if not exists check_creater(id int primary key auto_increment,username varchar(255),level int default 1,experience int default 0,create_num int default 0);");
                    QString username;
                    int level;
                    int experience;
                    int num;
                    qDebug()<<str;
                    for (int i=0;!str.section(',',i,i).isEmpty();)
                    {
                        username=str.section(',',i,i);
                        i++;
                        level=str.section(',',i,i).toInt();
                        i++;
                        experience=str.section(',',i,i).toInt();
                        i++;
                        num=str.section(',',i,i).toInt();
                        i++;
                        QSqlQuery query;
                        QString sql;
                        if(type_flag==0)
                        {
                            sql=QString("select * from check_user where username='%1';").arg(username);
                            query.exec(sql);
                            if (query.next())
                            {
                                QSqlQuery query1;
                                QString sql1=QString("update check_user set experience=%1 where username='%2';").arg(experience).arg(username);
                                QString sql2=QString("update check_user set level=%1 where username='%2';").arg(level).arg(username);
                                QString sql3=QString("update check_user set passed_num=%1 where username='%2';").arg(num).arg(username);
                                query1.exec(sql1);
                                query1.exec(sql2);
                                query1.exec(sql3);
                            }
                            else
                            {
                                QString sql=QString("insert into check_user(username,level,experience,passed_num) value('%1',%2,%3,%4);").arg(username).arg(level).arg(experience).arg(num);
                                query.exec(sql);
                            }
                        }
                        else
                        {
                            sql=QString("select * from check_creater where username='%1';").arg(username);
                            query.exec(sql);
                            if (query.next())
                            {
                                QSqlQuery query1;
                                QString sql1=QString("update check_creater set experience=%1 where username='%2';").arg(experience).arg(username);
                                QString sql2=QString("update check_creater set level=%1 where username='%2';").arg(level).arg(username);
                                QString sql3=QString("update check_creater set create_num=%1 where username='%2';").arg(num).arg(username);
                                query1.exec(sql1);
                                query1.exec(sql2);
                                query1.exec(sql3);
                            }
                            else
                            {
                                QString sql=QString("insert into check_creater(username,level,experience,create_num) value('%1',%2,%3,%4);").arg(username).arg(level).arg(experience).arg(num);
                                query.exec(sql);
                            }
                        }
                    }
                    model=new QSqlTableModel(this);
                    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
                    ui->tableView->setSortingEnabled(true);
                    if(type_flag==0)
                    {
                        model->setTable("check_user");
                        model->removeColumn(0);
                        model->setHeaderData(0,Qt::Horizontal,"用户名");
                        model->setHeaderData(1,Qt::Horizontal,"等级");
                        model->setHeaderData(2,Qt::Horizontal,"经验");
                        model->setHeaderData(3,Qt::Horizontal,"已过关数");
                    }
                    else
                    {
                        model->setTable("check_creater");
                        model->removeColumn(0);
                        model->setHeaderData(0,Qt::Horizontal,"用户名");
                        model->setHeaderData(1,Qt::Horizontal,"等级");
                        model->setHeaderData(2,Qt::Horizontal,"经验");
                        model->setHeaderData(3,Qt::Horizontal,"已出题数");
                    }
                    ui->tableView->setModel(model);
                    model->select();
                    tcpsocket->disconnectFromHost();
                    tcpsocket->close();
                }
            });
    }
}

void check::on_all_pushButton_clicked()
{
//    model=new QSqlTableModel(this);
//    model->setTable("user");
//    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
//    ui->tableView->setSortingEnabled(true);
//    if(type_flag==0)
//    {
//        ui->type_comboBox->clear();
//        ui->type_comboBox->addItem("用户名");
//        ui->type_comboBox->addItem("等级");
//        ui->type_comboBox->addItem("经验");
//        ui->type_comboBox->addItem("已过关数");
//        model->setTable("user");
//        model->removeColumn(0);
//        model->removeColumn(1);
//        model->setHeaderData(0,Qt::Horizontal,"用户名");
//        model->setHeaderData(1,Qt::Horizontal,"等级");
//        model->setHeaderData(2,Qt::Horizontal,"经验");
//        model->setHeaderData(3,Qt::Horizontal,"已过关数");
//    }
//    else if(type_flag==1)
//    {
//        ui->type_comboBox->clear();
//        ui->type_comboBox->addItem("用户名");
//        ui->type_comboBox->addItem("等级");
//        ui->type_comboBox->addItem("经验");
//        ui->type_comboBox->addItem("已出题数");
//        model->setTable("creater");
//        model->removeColumn(0);
//        model->removeColumn(1);
//        model->setHeaderData(0,Qt::Horizontal,"用户名");
//        model->setHeaderData(1,Qt::Horizontal,"等级");
//        model->setHeaderData(2,Qt::Horizontal,"经验");
//        model->setHeaderData(3,Qt::Horizontal,"已出题数");
//    }
//    ui->tableView->setModel(model);
//    model->select();
}
