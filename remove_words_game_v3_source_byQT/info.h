#ifndef INFO_H
#define INFO_H

#include <QWidget>
#include <QIcon>
#include<QPushButton>
#include<QPropertyAnimation>
#include <QCloseEvent>
#include<QSqlDatabase>
#include<QMessageBox>
#include<QSqlError>
#include<QSqlQuery>
#include<QString>
#include<QDebug>
#include<QSqlTableModel>
#include<QTcpSocket>
#include<QHostAddress>
#include"game.h"
#include"create.h"
#include"user.h"
namespace Ui {
class info;
}

class info : public QWidget
{
    Q_OBJECT

public:
    explicit info(QWidget *parent = nullptr);
    int type_flag;
    void init_UI();//初始化图形界面
    void connect_event();//信号，槽函数链接
    void game_show();
    void game_close();
    void create_show();
    void create_close();
    void showEvent(QShowEvent *);
    void close_Widget();
    void send_change_signal();
    void init_database();
    QString string_to_type();
    QSqlDatabase db;
    QSqlDatabase db2;
    QString username;
    int level;
    player my_player;
    creater my_creater;
    ~info();

private:
    Ui::info *ui;
    game game_widget;
    class create create_widget;
signals:
    void change_widget_signal();
private slots:
    void on_start_pushButton_clicked();
};

#endif // INFO_H
