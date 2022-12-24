#include"widget.h"
#include"ui_widget.h"
#include<QMessageBox>
#include<QDateTime>
#include<QDataStream>
#include<QColorDialog>
#include<QFileDialog>
#include<QFile>
#include<QTextStream>

Widget::Widget(QWidget *parent,QString name)
    : QWidget(parent)
    , ui(new Ui::Widget)
{
    ui->setupUi(this);
    udpSocket = new QUdpSocket(this);
    //获取用户名
    uName = name;
    //端口号
    this->port = 9999;
    //绑定端口号  绑定模式  共享地址|断线重连
    //采用ShareAddress模式(即允许其它的服务连接到相同的地址和端口，特别是用在多客户端监听同一个服务器端口等时特别有效)，和ReuseAddressHint模式(重新连接服务器)
    udpSocket->bind(port,QUdpSocket::ShareAddress |QUdpSocket::ReuseAddressHint);
    //有新用户加入
    sndMsg(UsrEnter);
    //点击发送按钮发送信息
    connect(ui->sendButton,&QPushButton::clicked,this,[=](){
        sndMsg(Msg);
    });
    //点击退出按钮
    connect(ui->exitButton,&QPushButton::clicked,this,[=]()
    {
        this->close();
    });
    //字体设置
    connect(ui->fontComboBox,&QFontComboBox::currentFontChanged,this,[=](const QFont &f){
        ui->messageTextEdit->setCurrentFont(f);
        ui->messageTextEdit->setFocus();
    });
    // 字号设置
    void (QComboBox:: * cbxSingal)(const QString &text) = &QComboBox::currentIndexChanged;
    connect(ui->sizeComboBox,cbxSingal,this,[=](const QString &text){
        ui->messageTextEdit->setFontPointSize(text.toDouble());
        ui->messageTextEdit->setFocus();
    });
    //加粗
    connect(ui->boldtoolButton,&QToolButton::clicked,this,[=](bool checked){
       if(checked){
           ui->messageTextEdit->setFontWeight(QFont::Bold);
       }else{
           ui->messageTextEdit->setFontWeight(QFont::Normal);
       }
       ui->messageTextEdit->setFocus();
    });
    //倾斜
    connect(ui->italictoolButton,&QToolButton::clicked,this,[=](bool checked){
        ui->messageTextEdit->setFontItalic(checked);
        ui->messageTextEdit->setFocus();
    });
    //下划线
    connect(ui->underlinetoolButton,&QToolButton::clicked,this,[=](bool checked){
            ui->messageTextEdit->setFontUnderline(checked);
            ui->messageTextEdit->setFocus();
    });
    //设置文本颜色
    connect(ui->colortoolButton,&QToolButton::clicked,this,[=](){
        color = QColorDialog::getColor(Qt::green,this);     //color对象可以在widget.h中定义私有成员
        if(color.isValid())
        {
            ui->messageTextEdit->setTextColor(color);
            ui->messageTextEdit->setFocus();
        }
    });
    //保存聊天记录
    connect(ui->savetoolButton,&QToolButton::clicked,this,[=](){
       if(ui->messageBrowser->document()->isEmpty()){
           QMessageBox::warning(this,"警告","聊天记录为空，无法保存！",QMessageBox::Ok);
       }
       else{
           QString fName = QFileDialog::getSaveFileName(this,"保存聊天记录","聊天记录","(*.txt)");
           if(!fName.isEmpty()){
               //保存名称不为空再做保存操作
               QFile file(fName);
               file.open(QIODevice::WriteOnly | QFile::Text);
               QTextStream stream(&file);
               stream << ui->messageBrowser->toPlainText();
               file.close();
           }
       }
    });
    //清空聊天记录
    connect(ui->cleartoolButton,&QToolButton::clicked,[=](){
        ui->messageBrowser->clear();
    });

    //监听别人发送的数据
    connect(udpSocket,&QUdpSocket::readyRead,this,&Widget::ReceiveMessage);

}

//广播UDP消息
void Widget::sndMsg(MsgType type)
{
  //发送的消息分3种类型
  //发送的数据做分段处理 第一段类型  第二段具体内容
  QByteArray array;
  QDataStream stream(&array,QIODevice::WriteOnly);
  stream<<type<<getUsr();   //将消息类型 和 用户名 放入到流中
  switch(type){
  case Msg:  //普通消息发送
      if(ui->messageTextEdit->toPlainText() == "")  //判断如果用户没有输入内容，不发任何消息
      {
          QMessageBox::warning(this,"警告","发送内容不能为空");
          return ;
      }
      //第二段数据，具体说的
      stream<<getMsg();   //发送的是聊天信息    发送格式   消息类型 + 用户名   + 发送内容
      break;
  case UsrEnter:   //发送的是新用户进入  发送格式   消息类型 + 用户名
      break;
  case UsrLeft:    //用户离开
      break;
  default:
      break;
  }
  //书写报文    广播发送QHostAddress::Broadcaset
  udpSocket->writeDatagram(array,QHostAddress::Broadcast,port);
}

//获取聊天信息
QString Widget::getMsg(){
 QString msg = ui->messageTextEdit->toHtml();
 //清空输入框
 ui->messageTextEdit->clear();
 ui->messageTextEdit->setFocus();
 return msg;
}

//接受UDP消息
void Widget::ReceiveMessage(){
//拿到数据报文
 //获取长度
 qint64 size = udpSocket->pendingDatagramSize();
 QByteArray array = QByteArray(size,0);
 udpSocket->readDatagram(array.data(),size);
 //解析数据
 //第一段类型   第二段 数据
 QDataStream stream (&array,QIODevice::ReadOnly);
 int msgType;   //读取到类型
 stream >> msgType; //用户类型获取
 QString usrName,msg;  //用户名、信息
 //获取当前时间
 QString time = QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss");
 switch (msgType){
 case Msg:    //普通聊天
     stream >> usrName  >>msg;
     //追加聊天记录
     ui->messageBrowser->setTextColor(Qt::blue);
     ui->messageBrowser->setCurrentFont(QFont("Times New Roman",12));
     ui->messageBrowser->append("[ " + usrName + " ]" + time);
     ui->messageBrowser->append(msg);
     break;
 case UsrEnter:
     //更新右侧TableWidget
     stream >> usrName ;
     usrEnter(usrName);
     break;
 case UsrLeft:
     stream >> usrName;
     usrLeft(usrName,time);
     break;
 default:
     break;
 }
}

//获取用户名
QString Widget::getUsr()
{
 return this->uName;
}

//处理新用户加入
void Widget::usrEnter(QString usrname)
{
 bool isEmpty = ui->userTableWidget->findItems(usrname,Qt::MatchExactly).isEmpty();
 if(isEmpty)
 {
     QTableWidgetItem *usr = new QTableWidgetItem(usrname);
     //插入行
     ui->userTableWidget->insertRow(0);
     ui->userTableWidget->setItem(0,0,usr);
     //追加聊天记录
     ui->messageBrowser->setTextColor(Qt::gray);
     ui->messageBrowser->setCurrentFont(QFont("Times New Roman",10));
     //在线人数更新
     ui->messageBrowser->append(tr("%1 在线！").arg(usrname));
     ui->userNumberlabel->setText(tr("在线人数：%1人").arg(ui->userTableWidget->rowCount()));
     //已经在线的各个端点也要告知新加入的端点他们自己的信息，若不这样做，在新端点用户列表中就无法显示其他已经在线的用户
     sndMsg(UsrEnter);
 }
}

//用户离开
void Widget::usrLeft(QString usrname,QString time)
{
 //更新右侧tableWidget
 bool isEmpty = ui->userTableWidget->findItems(usrname, Qt::MatchExactly).isEmpty();
 if(!isEmpty){
     ui->userTableWidget->removeRow(isEmpty);
     //追加聊天记录
     ui->messageBrowser->setTextColor(Qt::gray);
     ui->messageBrowser->setCurrentFont(QFont("Times New Roman", 10));
     ui->messageBrowser->append(QString("%1 于 %2 离开！").arg(usrname).arg(time));
     ui->userNumberlabel->setText(QString("在线人数：%1人").arg(ui->userTableWidget->rowCount()));
 }
}

void Widget::closeEvent(QCloseEvent * e)
{
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

