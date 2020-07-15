#include "my_thread.h"

my_thread::my_thread(QObject *parent) : QThread(parent)
{
    qDebug()<<3;
    flag=0;
    init_database();
}
void my_thread::run()
{
    qDebug()<<4;
    qDebug()<<"flag"<<flag;
    connect(tcpsocket,&QTcpSocket::readyRead,[=]()
    {
        qDebug()<<"0000000";
        switch (flag)
        {
        case 1:
        {
            qDebug()<<5;
            QByteArray array=tcpsocket->readAll();
            QString username(array);
            QSqlQuery query;
            QString sql=QString("select * from user where username='%1';").arg(username);
            query.exec(sql);
            if (query.next())
            {
                int passed_num=query.value("passed_num").toInt();
                int experience=query.value("experience").toInt();
                int level=query.value("level").toInt();
                QString password=query.value("password").toString();
                QString info=QString("%1,%2,%3,%4").arg(password).arg(passed_num).arg(experience).arg(level);
                qDebug()<<info;
                tcpsocket->write(info.toUtf8().data());
            }
            else
            {
                qDebug()<<6;
                tcpsocket->write(QString("error").toUtf8().data());
            }
            break;
        }
        case 2:
        {
            qDebug()<<"creater";
            QByteArray array=tcpsocket->readAll();
            QString username(array);
            QSqlQuery query;
            QString sql=QString("select * from creater where username='%1';").arg(username);
            query.exec(sql);
            if (query.next())
            {
                int create_num=query.value("create_num").toInt();
                int experience=query.value("experience").toInt();
                int level=query.value("level").toInt();
                QString password=query.value("password").toString();
                QString info=QString("%1,%2,%3,%4").arg(password).arg(create_num).arg(experience).arg(level);
                qDebug()<<info;
                tcpsocket->write(info.toUtf8().data());
            }
            else
            {
                qDebug()<<"error";
                tcpsocket->write(QString("error").toUtf8().data());
            }
            break;
        }
        case 3:
        {
            QByteArray array=tcpsocket->readAll();
            QString username(array);
            if(array==QString("OK!"))
            {
                QSqlQuery query;
                query.exec(QString("SELECT *FROM word AS t1 JOIN (SELECT ROUND(RAND() * (SELECT MAX(id) FROM word)) AS id) AS t2 WHERE t1.id >= t2.id ORDER BY t1.id ASC LIMIT 1;"));
                if(query.next())
                {
                    qDebug()<<query.value("word").toString();
                    tcpsocket->write(query.value("word").toString().toUtf8().data());
                }
                else
                {
                    qDebug()<<"error";
                    tcpsocket->write(QString("ERROR!").toUtf8().data());
                }
            }
            qDebug()<<"11111";
            break;
        }
        case 4:
        {
            qDebug()<<"update info";
            QByteArray array=tcpsocket->readAll();
            QString str(array);
            QString username=str.section(',',0,0);
            int level=str.section(',',1,1).toInt();
            int experience=str.section(',',2,2).toInt();
            int passed_num=str.section(',',3,3).toInt();
            qDebug()<<username<<experience<<level<<passed_num;
            QSqlQuery query;
            QString sql=QString("select * from user where username='%1';").arg(username);
            query.exec(sql);
            if (query.next())
            {
                QSqlQuery query1;
                QString sql1=QString("update user set experience=%1 where username='%2';").arg(experience).arg(username);
                QString sql2=QString("update user set level=%1 where username='%2';").arg(level).arg(username);
                QString sql3=QString("update user set passed_num=%1 where username='%2';").arg(passed_num).arg(username);
                query1.exec(sql1);
                query1.exec(sql2);
                query1.exec(sql3);
            }
            else
            {
                qDebug()<<"error";
                tcpsocket->write(QString("error").toUtf8().data());
            }
            break;
        }
        case 5:
        {
            qDebug()<<"create word";
            QByteArray array=tcpsocket->readAll();
            QString str(array);
            qDebug()<<str;
            QString word=str.section(',',0,0);
            QString username=str.section(',',1,1);
            int create_word=0;
            int experience=0;
            int level=1;
            QSqlQuery query;
            QString sql=QString("select * from word where word='%1';").arg(word);
            query.exec(sql);
            if (query.next())
            {
                qDebug()<<"error";
                tcpsocket->write(QString("error").toUtf8().data());
            }
            else
            {
                QSqlQuery query1;
                QString sql1=QString("insert into word(word,creater) value('%1','%2');").arg(word).arg(username);
                query1.exec(sql1);
                QSqlQuery query2;
                QString sql2=QString("select * from creater where username='%1';").arg(username);
                query2.exec(sql2);
                if (query2.next())
                {
                    level=query2.value("level").toInt();
                    experience=query2.value("experience").toInt()+1;
                    create_word=query2.value("create_num").toInt()+1;
                    QSqlQuery query3;
                    QString sql3=QString("update creater set create_num=%1 where username='%2';").arg(create_word).arg(username);
                    query3.exec(sql3);
                }
                for(;experience>=level;)
                {
                    experience=experience-level;
                    level++;
                }
                QSqlQuery query4;
                QString sql4=QString("update creater set level=%1 where username='%2';").arg(level).arg(username);
                query4.exec(sql4);
                QSqlQuery query5;
                QString sql5=QString("update creater set experience=%1 where username='%2';").arg(experience).arg(username);
                query5.exec(sql5);
                tcpsocket->write(QString("ok").toUtf8().data());
            }
            break;
        }
        case 6:
        {
            qDebug()<<"registe";
            QByteArray array=tcpsocket->readAll();
            QString str(array);
            qDebug()<<str;
            QString type=str.section(',',0,0);
            QString username=str.section(',',1,1);
            QString password=str.section(',',2,2);
            QSqlQuery query;
            QString sql;
            if(type.toInt()==0)
            {
                sql=QString("select * from user where username='%1';").arg(username);
            }
            else
            {
                sql=QString("select * from creater where username='%1';").arg(username);
            }
            query.exec(sql);
            if (query.next())
            {
                qDebug()<<"error";
                tcpsocket->write(QString("error").toUtf8().data());
            }
            else
            {
                QSqlQuery query1;
                QString sql1;
                if(type.toInt()==0)
                {
                    sql1=QString("insert into user(username,password) value('%1','%2');").arg(username).arg(password);
                }
                else
                {
                    sql1=QString("insert into creater(username,password) value('%1','%2');").arg(username).arg(password);
                }
                query1.exec(sql1);
                tcpsocket->write(QString("ok").toUtf8().data());
            }
            break;
        }
        case 7:
        {
            qDebug()<<"check";
            QByteArray array=tcpsocket->readAll();
            QString str(array);
            qDebug()<<str;
            QString type_flag=str.section(',',0,0);
            QString type=str.section(',',1,1);
            QString content=str.section(',',2,2);
            QSqlQuery query;
            QString sql;
            if(type_flag.toInt()==0)
            {
                sql=QString("select * from user where %1='%2';").arg(type).arg(content);
            }
            else
            {
                sql=QString("select * from creater where %1='%2';").arg(type).arg(content);
            }
            query.exec(sql);
            if(query.next())
            {
                QString data;
                if(type_flag.toInt()==0)
                {
                    data=data+query.value("username").toString()+QString(",")+query.value("level").toString()+QString(",")+
                            query.value("experience").toString()+QString(",")+query.value("passed_num").toString()+QString(",");
                }
                else
                {
                    data=data+query.value("username").toString()+QString(",")+query.value("level").toString()+QString(",")+
                            query.value("experience").toString()+QString(",")+query.value("create_num").toString()+QString(",");
                }
                while (query.next())
                {
                    data=data+query.value("username").toString()+QString(",")+query.value("level").toString()+QString(",")+
                            query.value("experience").toString()+QString(",")+query.value("create_num").toString()+QString(",");
                }
                qDebug()<<data;
                tcpsocket->write(data.toUtf8().data());
            }
            else
            {
                qDebug()<<"error";
                tcpsocket->write(QString("error").toUtf8().data());
            }
            break;
        }
        }
        emit is_done();
    }
    );
}

void my_thread::init_database()
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
        qDebug()<<"打开数据库错误"<<db2.lastError().text();
        return;
    }
    QSqlQuery query1(db2);
    query1.exec("create database if not exists word_game_v3_server DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;");

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("wxy");
    db.setPassword("123456");
    db.setDatabaseName("word_game_v3_server");
    if(!db.open())
    {
        qDebug()<<"打开数据库错误"<<db.lastError().text();
        return;
    }
    QSqlQuery query;
    query.exec("create table if not exists user(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,experience int default 0,passed_num int default 0);");
    query.exec("create table if not exists creater(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,experience int default 0,create_num int default 0);");
    query.exec("create table if not exists word(id int primary key auto_increment,word varchar(255),creater varchar(255));");
}
