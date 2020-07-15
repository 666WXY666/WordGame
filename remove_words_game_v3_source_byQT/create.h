#ifndef CREATE_H
#define CREATE_H

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
class create;
}

class create : public QWidget
{
    Q_OBJECT

public:
    explicit create(QWidget *parent = nullptr);
    void init_UI();//初始化图形界面
    void showEvent(QShowEvent *);
    void close_Widget();
    void send_change_signal();
    void init_database();
    QString username;
    QSqlDatabase db;
    QSqlDatabase db2;
    ~create();

private:
    Ui::create *ui;
    QSqlTableModel *model;
protected:
    void keyPressEvent(QKeyEvent *event);
signals:
    void change_widget_signal();
private slots:
    void on_confirm_pushButton_clicked();
};

#endif // CREATE_H
