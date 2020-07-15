#include "registe.h"
#include "ui_registe.h"

registe::registe(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::registe)
{
    ui->setupUi(this);
    init_UI();
    connect_event();
}
void registe::init_UI()
{
    //背景大小设置
    setFixedSize(1200,500);
    this->setAutoFillBackground(true);
    QPalette palette = this->palette();
    palette.setBrush(QPalette::Background, QBrush(QPixmap("://resource/background1.jpg").scaled(1200,500,Qt::IgnoreAspectRatio,Qt::SmoothTransformation)));
    this->setPalette(palette);
    //标题图标设置
    this->setWindowTitle("单词消除游戏<注册游戏账户>");
    QIcon exe_iocn("://resource/logo.png");
    this->setWindowIcon(exe_iocn);
    //按键文本框设置
    ui->username_lineEdit->setEnabled(true);
    ui->password_lineEdit->setEnabled(true);
}
void registe::connect_event()
{
    connect(ui->cancel_pushButton,&QPushButton::clicked,this,&registe::send_change_signal);
}
void registe::showEvent(QShowEvent *)
{
    //渐变出现
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(0);
    animation->setEndValue(1);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &registe::show);
    init_UI();
}
void registe::close_Widget()
{
    QPropertyAnimation *animation = new QPropertyAnimation(this, "windowOpacity");
    animation->setDuration(200);
    animation->setStartValue(1);
    animation->setEndValue(0);
    animation->start();
    connect(animation, &QPropertyAnimation::finished, this, &registe::close);
}
void registe::send_change_signal()
{
    emit change_widget_signal();
}
registe::~registe()
{
    delete ui;
}

void registe::on_complete_pushButton_clicked()
{
    QString username,password;
    username=ui->username_lineEdit->text();
    password=ui->password_lineEdit->text();
    if(username.isEmpty()||password.isEmpty())
    {
        QMessageBox::warning(this,"注册失败","账号或密码为空");
        return;
    }
    QString text=QString("用户名：%1\n密码：%2").arg(username).arg(password);
    int flag=QMessageBox::question(this,"您确定吗？",text);
    if(flag==QMessageBox::Yes)
    {
        ui->username_lineEdit->setEnabled(false);
        ui->password_lineEdit->setEnabled(false);
        QTcpSocket *tcpsocket=new QTcpSocket(this);
        QString ip=QString("127.0.0.1");
        qint16 port=8888;
        tcpsocket->connectToHost(QHostAddress(ip),port);
        tcpsocket->write(QString("request registe").toUtf8().data());
        connect(tcpsocket,&QTcpSocket::readyRead,[=]()
            {
                QByteArray array=tcpsocket->readAll();
                QString str(array);
                QString info;
                if(str==QString("request info"))
                {
                    info=QString("%1,%2,%3").arg(this->type_flag).arg(username).arg(password);
                    tcpsocket->write(info.toUtf8().data());
                    qDebug()<<info;
                }
                else if (str==QString("error"))
                {
                    QMessageBox::warning(this,"注册失败","账号已经存在");
                    tcpsocket->disconnectFromHost();
                    tcpsocket->close();
                    ui->username_lineEdit->setEnabled(true);
                    ui->password_lineEdit->setEnabled(true);
                }
                else if (str==QString("ok"))
                {
                    ui->username_lineEdit->clear();
                    ui->password_lineEdit->clear();
                    QMessageBox::information(this,"注册成功","注册成功");
                    send_change_signal();
                    tcpsocket->disconnectFromHost();
                    tcpsocket->close();
                }

            });
    }
}
