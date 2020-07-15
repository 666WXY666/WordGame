#ifndef GAME_H
#define GAME_H

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
#include<QTimer>
#include<QKeyEvent>
namespace Ui {
class game;
}

class game : public QWidget
{
    Q_OBJECT

public:
    explicit game(QWidget *parent = nullptr);
    int num;//关卡号，从1开始
    int start_num;
    int end_num;
    int passed_word;//在一关中答对的单词数
    QString current_word;
    void init_UI();//初始化图形界面
    void showEvent(QShowEvent *);
    void close_Widget();
    void send_change_signal();
    void init_database();
    void init_timer();//计时器初始化
    void init_ms_timer();//计时器初始化
    void timeup();//超时
    void ms_timeup();
    QString username;
    QSqlDatabase db;
    QSqlDatabase db2;
    ~game();

private:
    Ui::game *ui;
    QTimer* discount_timer;//计时器
    QTimer* ms_timer;
protected:
    void keyPressEvent(QKeyEvent *event);
signals:
    void change_widget_signal();
private slots:
    void on_confirm_pushButton_clicked();
    void on_start_pushButton_clicked();
};

#endif // GAME_H
