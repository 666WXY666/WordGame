#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QIcon>
#include<QPushButton>
#include"registe.h"
#include"check.h"
#include"info.h"
#include<QPropertyAnimation>
#include <QCloseEvent>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QSqlError>
#include<QSqlQuery>
#include<QDebug>
#include<QCheckBox>
#include<QTcpSocket>
#include<QHostAddress>
namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    int type_flag;//0:游戏玩家；1：出题人
    void init_UI();//初始化图形界面
    void connect_event();//信号，槽函数链接
    void user_registe_show();//注册槽函数
    void creater_registe_show();//注册槽函数
    void check_user_show();
    void check_creater_show();
    void info_show();
    void check_close();
    void registe_close();
    void info_close();
    void showEvent(QShowEvent *);
    void close_Widget();
    void type_changed();
    void init_database();
    QSqlDatabase db;
    QSqlDatabase db2;
    ~Widget();

private slots:
    void on_login_pushButton_clicked();

private:
    Ui::Widget *ui;
    registe registe_widget;
    check check_widget;
    info info_widget;
};

#endif // WIDGET_H
