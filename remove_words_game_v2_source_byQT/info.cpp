#include "info.h"
#include "ui_info.h"

info::info(QWidget *parent) :
    QWidget(parent),level(0),my_player(username,level),my_creater(username,level),
    ui(new Ui::info)
{
    ui->setupUi(this);
    init_database();
    init_UI();
    connect_event();
}
void info::init_database()
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
    query1.exec("create database if not exists word_game_v2 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;");

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("wxy");
    db.setPassword("123456");
    db.setDatabaseName("word_game_v2");
    if(!db.open())
    {
        QMessageBox::warning(this,"打开数据库错误",db.lastError().text());
        return;
    }
//    QSqlQuery query;
//    query.exec("create table user(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,experience int default 0,passed_num int default 0);");
//    query.exec("create table creater(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,create_num int default 0);");
}
void info::init_UI()
{
    //背景大小设置
    setFixedSize(500,900);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://resource/background4.jpg").scaled(500,900,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    //标题图标设置
    this->setWindowTitle("单词消除游戏<登录成功>");
    QIcon exe_iocn("://resource/logo.png");
    this->setWindowIcon(exe_iocn);
    ui->user->setText(username);
    if(this->type_flag==0)
    {
        ui->start_pushButton->setText("开始游戏");
        ui->number_label->setText("已过关数：");
        QSqlQuery query;
        QString sql_select=QString("select * from user where username='%1';").arg(username);
        query.exec(sql_select);
        if(query.next())
        {
            ui->number->setText(QString("%1").arg(query.value("passed_num").toInt()));
            game_widget.num=query.value("passed_num").toInt()+1;
            int experience=query.value("experience").toInt();
            int level=query.value("level").toInt();
            my_player.level=level;
            my_player.experience=experience;
            my_player.level_up();
            QSqlQuery query1;
            QString sql1=QString("update user set experience=%1 where username='%2';").arg(my_player.experience).arg(username);
            QString sql2=QString("update user set level=%1 where username='%2';").arg(my_player.level).arg(username);
            query1.exec(sql1);
            query1.exec(sql2);
            ui->level->setText(QString("%1").arg(my_player.level));
            ui->experience_progressBar->setMaximum(my_player.level);
            ui->experience_progressBar->setValue(my_player.experience);
        }
    }
    else if(this->type_flag==1)
    {
        ui->number_label->setText("已出题数：");
        ui->start_pushButton->setText("开始出题");
        QSqlQuery query;
        QString sql_select=QString("select * from creater where username='%1';").arg(username);
        query.exec(sql_select);
        if(query.next())
        {
            ui->number->setText(QString("%1").arg(query.value("create_num").toInt()));
            create_widget.create_word=query.value("create_num").toInt();
            int experience=query.value("experience").toInt();
            int level=query.value("level").toInt();
            my_creater.level=level;
            my_creater.experience=experience;
            my_creater.level_up();
            QSqlQuery query1;
            QString sql1=QString("update creater set experience=%1 where username='%2';").arg(my_creater.experience).arg(username);
            QString sql2=QString("update creater set level=%1 where username='%2';").arg(my_creater.level).arg(username);
            query1.exec(sql1);
            query1.exec(sql2);
            ui->level->setText(QString("%1").arg(my_creater.level));
            ui->experience_progressBar->setMaximum(my_creater.level);
            ui->experience_progressBar->setValue(my_creater.experience);
        }
    }
}
void info::connect_event()
{
    connect(ui->back_pushButton,&QPushButton::clicked,this,&info::send_change_signal);
    connect(&game_widget,&game::change_widget_signal,this,&info::game_close);
    connect(&create_widget,&create::change_widget_signal,this,&info::create_close);
}
void info::game_show()
{
    game_widget.start_num=ui->number->text().toInt();
    game_widget.show();
    this->close_Widget();
}
void info::game_close()
{
    game_widget.close_Widget();
    this->show();
}
void info::create_show()
{
    create_widget.start_num=ui->number->text().toInt();
    create_widget.show();
    this->close_Widget();
}
void info::create_close()
{
    create_widget.close_Widget();
    this->show();
}
void info::showEvent(QShowEvent *)
{
    //渐变出现
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &info::show);
    init_UI();
}
void info::close_Widget()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &info::close);
}
void info::send_change_signal()
{
    emit change_widget_signal();
}
info::~info()
{
    delete ui;
}

void info::on_start_pushButton_clicked()
{
    if(type_flag==0)
    {
        game_widget.username=this->username;
        game_show();
    }
    else if(type_flag==1)
    {
        create_widget.username=this->username;
        create_show();
    }
}
