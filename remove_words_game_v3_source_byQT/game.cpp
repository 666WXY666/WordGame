#include "game.h"
#include "ui_game.h"

game::game(QWidget *parent) :
    QWidget(parent),passed_word(0),
    ui(new Ui::game)
{
    ui->setupUi(this);
    discount_timer=nullptr;
    ms_timer=nullptr;
    init_database();
    init_UI();
    connect(ui->back_pushButton,&QPushButton::clicked,this,&game::send_change_signal);
}
void game::init_database()
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
}
void game::init_UI()
{
    QSqlQuery query1;
    QString sql=QString("select * from user where username='%1';").arg(username);
    query1.exec(sql);
    if(query1.next())
    {
        num=query1.value("passed_num").toInt()+1;
    }
    else
    {
        num=1;
    }
    ui->input_lineEdit->setReadOnly(true);
    ui->confirm_pushButton->setEnabled(false);
    ui->input_lineEdit->setFocus();
    ui->input_lineEdit->clear();
    ui->num_label->setText(QString("第%1关").arg(num));
    //背景大小设置
    setFixedSize(1000,600);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://resource/background5.jpg").scaled(1000,600,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    //标题图标设置
    this->setWindowTitle("单词消除游戏<游戏开始>");
    QIcon exe_iocn("://resource/logo.png");
    this->setWindowIcon(exe_iocn);
}
void game::init_timer()
{
    discount_timer=new QTimer;
    if(num<=8&&num>=1)
    {
        discount_timer->setInterval((11-num)*1000);
    }
    else
    {
        discount_timer->setInterval(2000);
    }
}
void game::init_ms_timer()
{
    ms_timer=new QTimer;
    ms_timer->setInterval(1);
}
void game::timeup()
{
    if(discount_timer->isActive())
    {
        discount_timer->stop();
    }
    if(ms_timer->isActive())
    {
        ms_timer->stop();
    }
    ui->word_label->setVisible(false);
    ui->input_lineEdit->setReadOnly(false);
    ui->input_lineEdit->setFocus();
    ui->confirm_pushButton->setEnabled(true);
    ui->back_pushButton->hide();
}
void game::ms_timeup()
{
    if(discount_timer->isActive())
    {
        ui->time_lcdNumber->display(discount_timer->remainingTime()/1000);
    }
}
void game::showEvent(QShowEvent *)
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &game::show);
    init_UI();
}
void game::close_Widget()
{
    ui->input_lineEdit->setReadOnly(true);
    ui->confirm_pushButton->setEnabled(false);
    ui->input_lineEdit->setFocus();
    if(discount_timer!=nullptr&&discount_timer->isActive())
    {
        discount_timer->stop();
    }
    if(ms_timer!=nullptr&&ms_timer->isActive())
    {
        ms_timer->stop();
    }
    passed_word=0;
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &game::close);
    ui->start_pushButton->show();
    ui->back_pushButton->show();
    ui->time_lcdNumber->display(0);
    int experience=0;
    int level=1;
    QSqlQuery query;
    QString sql_select=QString("select * from user where username='%1';").arg(username);
    query.exec(sql_select);
    if(query.next())
    {
        end_num=query.value("passed_num").toInt();
        experience=query.value("experience").toInt()+end_num-start_num;
        level=query.value("level").toInt();
    }
    for(;experience>=level;)
    {
        experience=experience-level;
        level++;
    }
    QSqlQuery query1;
    QString sql1=QString("update user set experience=%1 where username='%2';").arg(experience).arg(username);
    QString sql2=QString("update user set level=%1 where username='%2';").arg(level).arg(username);
    query1.exec(sql1);
    query1.exec(sql2);
    QTcpSocket *tcpsocket=new QTcpSocket(this);
    QString ip=QString("127.0.0.1");
    qint16 port=8888;
    tcpsocket->connectToHost(QHostAddress(ip),port);
    tcpsocket->write(QString("request update").toUtf8().data());
    connect(tcpsocket,&QTcpSocket::readyRead,[=]()
        {
        QByteArray array=tcpsocket->readAll();
        QString str(array);
        qDebug()<<str;
        if(str==QString("error"))
        {
            QMessageBox::warning(this,"错误","账号不存在");
        }
        else if(str==QString("request info"))
        {
            tcpsocket->write(QString("%1,%2,%3,%4").arg(username).arg(level).arg(experience).arg(end_num).toUtf8().data());
            tcpsocket->disconnectFromHost();
            tcpsocket->close();
        }
        });
}
void game::send_change_signal()
{
    emit change_widget_signal();
}
void game::keyPressEvent(QKeyEvent *event)
{
    if((event->key()==Qt::Key_Enter||event->key()==Qt::Key_Return)&&discount_timer!=nullptr)
    {
        if(!discount_timer->isActive())
        {
            ui->input_lineEdit->setEnabled(false);
            if(discount_timer!=nullptr&&discount_timer->isActive())
            {
                discount_timer->stop();
            }
            if(ms_timer!=nullptr&&ms_timer->isActive())
            {
                ms_timer->stop();
            }
            if(ui->word_label->text()==ui->input_lineEdit->text())
            {
                passed_word++;
                if(passed_word==num)
                {
                    num++;
                    passed_word=0;
                }
                QSqlQuery query;
                QString sql=QString("update user set passed_num=%1 where username='%2';").arg(num-1).arg(username);
                query.exec(sql);
                init_timer();
                init_ms_timer();
                discount_timer->start();
                ms_timer->start();
                connect(discount_timer,&QTimer::timeout,this,&game::timeup);
                connect(ms_timer,&QTimer::timeout,this,&game::ms_timeup);
                init_UI();
                QTcpSocket *tcpsocket=new QTcpSocket(this);
                QString ip=QString("127.0.0.1");
                qint16 port=8888;
                tcpsocket->connectToHost(QHostAddress(ip),port);
                tcpsocket->write(QString("request word").toUtf8().data());
                connect(tcpsocket,&QTcpSocket::readyRead,[=]()
                    {
                        QByteArray array=tcpsocket->readAll();
                        QString word(array);
                        qDebug()<<word;
                        if(word==QString("ERROR!"))
                        {
                            QMessageBox::warning(this,"错误","词库错误");
                            tcpsocket->disconnectFromHost();
                            tcpsocket->close();
                            send_change_signal();
                        }
                        else if(word==QString("READY!"))
                        {
                            tcpsocket->write(QString("OK!").toUtf8().data());
                        }
                        else
                        {
                            ui->word_label->setText(word);
                            ui->input_lineEdit->setReadOnly(true);
                            ui->confirm_pushButton->setEnabled(false);
                            tcpsocket->disconnectFromHost();
                            tcpsocket->close();
                        }
                    });
            }
            else
            {
                QMessageBox::warning(this,"闯关失败","你答错了");
                ui->input_lineEdit->clear();
                send_change_signal();
            }
            ui->word_label->setVisible(true);
            ui->back_pushButton->show();
            ui->input_lineEdit->setEnabled(true);
            ui->input_lineEdit->setFocus();
        }
    }
}
game::~game()
{
    delete ui;
}

void game::on_confirm_pushButton_clicked()
{
    ui->input_lineEdit->setEnabled(false);
    if(discount_timer!=nullptr&&discount_timer->isActive())
    {
        discount_timer->stop();
    }
    if(ms_timer!=nullptr&&ms_timer->isActive())
    {
        ms_timer->stop();
    }
    if(ui->word_label->text()==ui->input_lineEdit->text())
    {
        passed_word++;
        if(passed_word==num)
        {
            num++;
            passed_word=0;
        }
        QSqlQuery query;
        QString sql=QString("update user set passed_num=%1 where username='%2';").arg(num-1).arg(username);
        query.exec(sql);
        init_timer();
        init_ms_timer();
        discount_timer->start();
        ms_timer->start();
        connect(discount_timer,&QTimer::timeout,this,&game::timeup);
        connect(ms_timer,&QTimer::timeout,this,&game::ms_timeup);
        init_UI();
        QTcpSocket *tcpsocket=new QTcpSocket(this);
        QString ip=QString("127.0.0.1");
        qint16 port=8888;
        tcpsocket->connectToHost(QHostAddress(ip),port);
        tcpsocket->write(QString("request word").toUtf8().data());
        connect(tcpsocket,&QTcpSocket::readyRead,[=]()
            {
                QByteArray array=tcpsocket->readAll();
                QString word(array);
                qDebug()<<word;
                if(word==QString("ERROR!"))
                {
                    QMessageBox::warning(this,"错误","词库错误");
                    tcpsocket->disconnectFromHost();
                    tcpsocket->close();
                    send_change_signal();
                }
                else if(word==QString("READY!"))
                {
                    tcpsocket->write(QString("OK!").toUtf8().data());
                }
                else
                {
                    ui->word_label->setText(word);
                    ui->input_lineEdit->setReadOnly(true);
                    ui->confirm_pushButton->setEnabled(false);
                    tcpsocket->disconnectFromHost();
                    tcpsocket->close();
                }
            });
    }
    else
    {
        QMessageBox::warning(this,"闯关失败","你答错了");
        ui->input_lineEdit->clear();
        send_change_signal();
    }
    ui->word_label->setVisible(true);
    ui->back_pushButton->show();
    ui->input_lineEdit->setEnabled(true);
    ui->input_lineEdit->setFocus();
}

void game::on_start_pushButton_clicked()
{
    QTcpSocket *tcpsocket=new QTcpSocket(this);
    QString ip=QString("127.0.0.1");
    qint16 port=8888;
    tcpsocket->connectToHost(QHostAddress(ip),port);
    tcpsocket->write(QString("request word").toUtf8().data());
    connect(tcpsocket,&QTcpSocket::readyRead,[=]()
        {
            QByteArray array=tcpsocket->readAll();
            QString word(array);
            qDebug()<<word;
            if(word==QString("ERROR!"))
            {
                QMessageBox::warning(this,"错误","词库错误");
            }
            else if(word==QString("READY!"))
            {
                tcpsocket->write(QString("OK!").toUtf8().data());
            }
            else
            {
                qDebug()<<"mumumu"<<word;
                ui->word_label->setText(word);
                init_timer();
                init_ms_timer();
                discount_timer->start();
                ms_timer->start();
                connect(discount_timer,&QTimer::timeout,this,&game::timeup);
                connect(ms_timer,&QTimer::timeout,this,&game::ms_timeup);
                ui->start_pushButton->hide();
                tcpsocket->disconnectFromHost();
                tcpsocket->close();
            }
        });

}
