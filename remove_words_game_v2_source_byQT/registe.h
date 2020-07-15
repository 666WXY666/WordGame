#ifndef REGISTE_H
#define REGISTE_H

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
namespace Ui {
class registe;
}

class registe : public QWidget
{
    Q_OBJECT

public:
    explicit registe(QWidget *parent = nullptr);
    int type_flag;//0:游戏玩家；1：出题人
    void init_UI();//初始化图形界面
    void connect_event();//信号，槽函数链接
    void showEvent(QShowEvent *);
    void close_Widget();
    void send_change_signal();
    void init_database();
    QSqlDatabase db;
    QSqlDatabase db2;
    ~registe();

private:
    Ui::registe *ui;
signals:
    void change_widget_signal();
private slots:
    void on_complete_pushButton_clicked();
};

#endif // REGISTE_H
