#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QString>
#include <QUdpSocket>

QT_BEGIN_NAMESPACE
namespace Ui { class Widget; }
QT_END_NAMESPACE

class Widget : public QWidget
{
    Q_OBJECT

public:
    enum MsgType {Msg,UsrEnter,UsrLeft};
    Widget(QWidget *parent,QString name,qint16 portn);
    ~Widget();
    void sndMsg(MsgType type); //广播UDP消息
    void usrEnter(QString usrname);//处理新用户加入
    void usrLeft(QString usrname,QString time); //处理用户离开
    void ReceiveMessage();//接收UDP消息
    QString getName(); //获取用户名
    QString getMsg(); //获取聊天信息

signals:
   //关闭窗口发送关闭信息
    void closeWidget();
    void myNameChanged();

protected:
   //关闭事件
  void closeEvent(QCloseEvent *);

private:
    Ui::Widget *ui;
    QUdpSocket * udpSocket; //udp套接字
    qint16 port; //端口
    QString myName; //用户名
};
#endif // WIDGET_H
