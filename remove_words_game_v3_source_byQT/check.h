#ifndef CHECK_H
#define CHECK_H

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
namespace Ui {
class check;
}

class check : public QWidget
{
    Q_OBJECT

public:
    explicit check(QWidget *parent = nullptr);
    int type_flag;
    void init_UI();//初始化图形界面
    void connect_event();//信号，槽函数链接
    void showEvent(QShowEvent *);
    void close_Widget();
    void send_change_signal();
    void init_database();
    void search();
    QString string_to_type();
    QSqlDatabase db;
    QSqlDatabase db2;
    ~check();

private:
    Ui::check *ui;
    QSqlTableModel *model;
signals:
    void change_widget_signal();
private slots:
    void on_check_pushButton_clicked();
    void on_all_pushButton_clicked();
};

#endif // CHECK_H
