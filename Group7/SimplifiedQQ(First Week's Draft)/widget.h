#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QUdpSocket>

enum MsgType{Msg,UsrEnter,UsrLeft};
QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    Widget(QWidget *parent,QString name);
    ~Widget();

private:
    Ui::Widget *ui;

signals:
   //关闭窗口发送关闭信息
   void closeWidget();

protected:
   //关闭事件
  void closeEvent(QCloseEvent *);

public:
void sndMsg(MsgType type); //广播UDP消息
void usrEnter(QString username);//处理新用户加入
void usrLeft(QString usrname,QString time); //处理用户离开
QString getUsr(); //获取用户名
QString getMsg(); //获取聊天信息

private:
QUdpSocket * udpSocket; //udp套接字
qint16 port; //端口
QString uName; //用户名
QColor color;//字体颜色

void ReceiveMessage();   //接受UDP消息


};
#endif // WIDGET_H


