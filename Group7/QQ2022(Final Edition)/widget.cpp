#include "widget.h"
#include "ui_widget.h"
#include <QMessageBox>
#include <QDataStream>
#include <QDateTime>
#include <QColorDialog>
#include <QFileDialog>

Widget::Widget(QWidget *parent,QString name,qint16 portn)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    myName = name;
    this->port = portn;
    this->udpSocket = new QUdpSocket(this);
    ui->usrName->setText(QString("用户：%1").arg(myName));

    udpSocket->bind(port, QUdpSocket::ShareAddress |QUdpSocket::ReuseAddressHint);

    //
    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::ReceiveMessage);

    //连接发送按钮
    connect(ui->sendBtn,&QPushButton::clicked,[=](){
        sndMsg(Msg);
    });
    //连接退出按钮
    connect(ui->exitBtn,&QPushButton::clicked,this,[=](){
         this->close();
    });
    //新用户进入
    sndMsg(UsrEnter);

    //设置字体
    connect(ui->fontCbx,&QFontComboBox::currentFontChanged,this,[=](const QFont &font){
        ui->msgTxtEdit->setCurrentFont(font);
        ui->msgTxtEdit->setFocus();
    });
    //设置字号
    void (QComboBox:: * sizebtn)(const QString &text) = &QComboBox::currentTextChanged;
     connect(ui->sizeCbx,sizebtn,[=](const QString &text){
         ui->msgTxtEdit->setFontPointSize(text.toDouble());
         ui->msgTxtEdit->setFocus();
     });
     //加粗
    connect(ui->boldTBtn,&QToolButton::clicked,this,[=](bool checked){
        if(checked){
            ui->msgTxtEdit->setFontWeight(QFont::Bold);
        }else{
            ui->msgTxtEdit->setFontWeight(QFont::Normal);
        }
        ui->msgTxtEdit->setFocus();
    });
    //倾斜
    connect(ui->italicTBtn,&QToolButton::clicked,this,[=](bool checked){
        ui->msgTxtEdit->setFontItalic(checked);
        ui->msgTxtEdit->setFocus();
    });
    //下划线
    connect(ui->underlineTBtn,&QToolButton::clicked,this,[=](bool checked){
        ui->msgTxtEdit->setFontUnderline(checked);
        ui->msgTxtEdit->setFocus();
    });
    //设置文本颜色
    connect(ui->colorTBtn,&QToolButton::clicked,this,[=](){
        QColor color = QColorDialog::getColor(color,this);     //color对象可以在widget.h中定义私有成员
        ui->msgTxtEdit->setTextColor(color);
    });
    //保存聊天记录
    connect(ui->saveTBtn,&QToolButton::clicked,this,[=](){
        if(ui->msgBrowser->document()->isEmpty()){
            QMessageBox::warning(this,"警告","聊天记录为空，无法保存！",QMessageBox::Ok);
        }
        else{
            QString fName = QFileDialog::getSaveFileName(this,"保存聊天记录","聊天记录","(*.txt)");
            if(!fName.isEmpty()){
                //保存名称不为空再做保存操作
                QFile file(fName);
                file.open(QIODevice::WriteOnly | QFile::Text);
                QTextStream stream(&file);
                stream << ui->msgBrowser->toPlainText();
                file.close();
            }
        }
    });
    //清空聊天记录
    connect(ui->clearTBtn,&QToolButton::clicked,[=](){
         ui->msgBrowser->clear();
    });
}

void Widget::closeEvent(QCloseEvent * e){
    emit this->closeWidget(); //发送关闭当前窗口的自定义信号
    sndMsg(UsrLeft);
    //断开套接字
    udpSocket->close();
    udpSocket->destroyed();
    QWidget::closeEvent(e);
 }

Widget::~Widget()
{
    delete ui;
}

void Widget::sndMsg(MsgType type)
{
    QByteArray array;
    //创建流 可以分段 参数1流入地址 参数2只能写
    QDataStream stream(&array, QIODevice::WriteOnly);
    stream<<type<<this->getName();//流入类型和用户名
    switch(type){
        case Msg:
            if(this->ui->msgTxtEdit->toPlainText()==""){
                QMessageBox::warning(this,"警告","聊天内容不能为空");
                return;
            }
            stream<<this->getMsg();//发送的是聊天信息    发送格式   消息类型 + 用户名   + 发送内容
            break;
        case UsrEnter://发送的是新用户进入  发送格式   消息类型 + 用户名
            break;
        case UsrLeft://用户离开
            break;
        default:
            break;
    }
    //书写报文
    udpSocket->writeDatagram(array.data(),array.size(),QHostAddress::Broadcast,this->port);
}

void Widget::usrEnter(QString usrname)
{
    bool isEmpty = ui->usrTblWidget->findItems(usrname,Qt::MatchExactly).isEmpty();
     if(isEmpty)
     {
         QTableWidgetItem *usr = new QTableWidgetItem(usrname);
         //插入行
         ui->usrTblWidget->insertRow(0);
         ui->usrTblWidget->setItem(0,0,usr);
         //追加聊天记录
         ui->msgBrowser->setTextColor(Qt::gray);
         ui->msgBrowser->setCurrentFont(QFont("Times New Roman",10));
         //在线人数更新
         ui->msgBrowser->append(tr("%1 在线！").arg(usrname));
         ui->usrNumLbl->setText(tr("在线人数：%1人").arg(ui->usrTblWidget->rowCount()));
         //已经在线的各个端点也要告知新加入的端点他们自己的信息，若不这样做，在新端点用户列表中就无法显示其他已经在线的用户
         sndMsg(UsrEnter);
     }
}

void Widget::usrLeft(QString usrname, QString time)
{
    bool isEmpty = ui->usrTblWidget->findItems(usrname, Qt::MatchExactly).isEmpty();
     if(!isEmpty){
         ui->usrTblWidget->removeRow(isEmpty);
         //追加聊天记录
         ui->msgBrowser->setTextColor(Qt::gray);
         ui->msgBrowser->setCurrentFont(QFont("Times New Roman", 10));
         ui->msgBrowser->append(QString("%1 于 %2 离开！").arg(usrname).arg(time));
         ui->usrNumLbl->setText(QString("在线人数：%1人").arg(ui->usrTblWidget->rowCount()));
     }
}

void Widget::ReceiveMessage()
{
    qint64 size = udpSocket->pendingDatagramSize();
    int mysize = static_cast<int>(size);
    QByteArray array = QByteArray(mysize,0);
    udpSocket->readDatagram(array.data(),size);
    QDataStream stream(&array,QIODevice::ReadOnly);
    int msgtype;
    stream>>msgtype;
    QString name,msg;
    QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
    switch(msgtype){
    case Msg:
        stream>>name>>msg;//流出用户名 聊天内容
        ui->msgBrowser->setTextColor(Qt::blue);
        ui->msgBrowser->setCurrentFont(QFont("Times New Roman",12));
        ui->msgBrowser->append("[ " + name + " ]" + time);
        ui->msgBrowser->append(msg);
        break;
    case UsrEnter:
        stream>>name;
        usrEnter(name);
        break;
    case UsrLeft:
        stream>>name;
        usrLeft(name,time);
        break;
    }
}

QString Widget::getName()
{
    return this->myName;
}

QString Widget::getMsg()
{
    QString msg = ui->msgTxtEdit->toHtml();
    ui->msgTxtEdit->clear();
    ui->msgTxtEdit->setFocus();
    return msg;
}

