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
    query1.exec("create database if not exists word_game_v1 DEFAULT CHARACTER SET utf8 COLLATE utf8_general_ci;");

    if(QSqlDatabase::contains("qt_sql_default_connection"))
        db = QSqlDatabase::database("qt_sql_default_connection");
    else
        db = QSqlDatabase::addDatabase("QMYSQL");
    db.setHostName("127.0.0.1");
    db.setUserName("wxy");
    db.setPassword("123456");
    db.setDatabaseName("word_game_v1");
    if(!db.open())
    {
        QMessageBox::warning(this,"打开数据库错误",db.lastError().text());
        return;
    }
//    QSqlQuery query;
//    query.exec("create table user(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,experience int default 0,passed_num int default 0);");
//    query.exec("create table creater(id int primary key auto_increment,username varchar(255),password varchar(255),level int default 1,create_num int default 0);");
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
    model->setTable("user");
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSortingEnabled(true);
    ui->type_comboBox->clear();
    ui->type_comboBox->addItem("用户名");
    ui->type_comboBox->addItem("等级");
    ui->type_comboBox->addItem("经验");
    if(type_flag==0)
    {
        ui->type_comboBox->addItem("已过关数");
        model->setTable("user");
        model->removeColumn(0);
        model->removeColumn(1);
        model->setHeaderData(0,Qt::Horizontal,"用户名");
        model->setHeaderData(1,Qt::Horizontal,"等级");
        model->setHeaderData(2,Qt::Horizontal,"经验");
        model->setHeaderData(3,Qt::Horizontal,"已过关数");
    }
    else if(type_flag==1)
    {
        ui->type_comboBox->addItem("已出题数");
        model->setTable("creater");
        model->removeColumn(0);
        model->removeColumn(1);
        model->setHeaderData(0,Qt::Horizontal,"用户名");
        model->setHeaderData(1,Qt::Horizontal,"等级");
        model->setHeaderData(2,Qt::Horizontal,"经验");
        model->setHeaderData(3,Qt::Horizontal,"已出题数");
    }
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
        model=new QSqlTableModel(this);
        model->setTable("user");
        ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
        ui->tableView->setSortingEnabled(true);
        if(type_flag==0)
        {
            model->setTable("user");
            model->removeColumn(0);
            model->removeColumn(1);
            model->setHeaderData(0,Qt::Horizontal,"用户名");
            model->setHeaderData(1,Qt::Horizontal,"等级");
            model->setHeaderData(2,Qt::Horizontal,"经验");
            model->setHeaderData(3,Qt::Horizontal,"已过关数");
        }
        else if(type_flag==1)
        {
            model->setTable("creater");
            model->removeColumn(0);
            model->removeColumn(1);
            model->setHeaderData(0,Qt::Horizontal,"用户名");
            model->setHeaderData(1,Qt::Horizontal,"等级");
            model->setHeaderData(2,Qt::Horizontal,"经验");
            model->setHeaderData(3,Qt::Horizontal,"已出题数");
        }
        QString str=QString("%1='%2'").arg(type).arg(content);
        model->setFilter(str);
        ui->tableView->setModel(model);
        model->select();
    }
}

void check::on_all_pushButton_clicked()
{
    model=new QSqlTableModel(this);
    model->setTable("user");
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
    ui->tableView->setSortingEnabled(true);
    if(type_flag==0)
    {
        ui->type_comboBox->clear();
        ui->type_comboBox->addItem("用户名");
        ui->type_comboBox->addItem("等级");
        ui->type_comboBox->addItem("经验");
        ui->type_comboBox->addItem("已过关数");
        model->setTable("user");
        model->removeColumn(0);
        model->removeColumn(1);
        model->setHeaderData(0,Qt::Horizontal,"用户名");
        model->setHeaderData(1,Qt::Horizontal,"等级");
        model->setHeaderData(2,Qt::Horizontal,"经验");
        model->setHeaderData(3,Qt::Horizontal,"已过关数");
    }
    else if(type_flag==1)
    {
        ui->type_comboBox->clear();
        ui->type_comboBox->addItem("用户名");
        ui->type_comboBox->addItem("等级");
        ui->type_comboBox->addItem("经验");
        ui->type_comboBox->addItem("已出题数");
        model->setTable("creater");
        model->removeColumn(0);
        model->removeColumn(1);
        model->setHeaderData(0,Qt::Horizontal,"用户名");
        model->setHeaderData(1,Qt::Horizontal,"等级");
        model->setHeaderData(2,Qt::Horizontal,"经验");
        model->setHeaderData(3,Qt::Horizontal,"已出题数");
    }
    ui->tableView->setModel(model);
    model->select();
}
