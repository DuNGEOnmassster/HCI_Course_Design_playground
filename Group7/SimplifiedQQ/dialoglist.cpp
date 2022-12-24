#include "dialoglist.h"
#include "ui_dialoglist.h"
#include <QToolButton>
#include <widget.h>
#include <QMessageBox>
DialogList::DialogList(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::DialogList)
{
    ui->setupUi(this);
    //设置页面标题
    setWindowTitle("QQ");
    //设置头像
    //setWindowIcon(QPixmap(":/study/HCI/SimplifiedQQ/zjb.png"));


    //人物名称
    QStringList nameList;
    nameList<<"周骏博"<<"邓卓文"<<"潘星宇"<<"徐绍倡"<<"方雍"
           <<"稂子萱"<<"彭虹清"<<"林洋乐"<<"吴东杰"<<"李晓赫";
    //人物头像
    QList<QString> iconList;
    iconList<<"zjb"<<"dzw"<<"pxy"<<"xsc"<<"fy"
           <<"lzx"<<"phq"<<"lyl"<<"wdj"<<"lxh";

    //将9个人物用QVector进行维护
    QVector<QToolButton*> vToolbtn;
    for(int i=0;i<10;i++)
    {
        //设置列表成员
        QToolButton* btn = new QToolButton;
        //设置文字
        btn->setText(nameList[i]);
        //设置头像
        QString str = QString(":/images/%1.png").arg(iconList.at(i));
        btn->setIcon(QIcon(str));
        //设置头像大小
        btn->setIconSize(QSize(100,100));
        //设置按钮风格 透明
        btn->setAutoRaise(true);
        //设置文字和图片一起显示
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        //加到垂直布局中
        ui->vLayout->addWidget(btn);
        vToolbtn.push_back(btn);
        //10个标识默认初始化为false
        isShow.push_back(false);
    }

    //对十个按钮进行添加信号槽
    for(int i=0;i<vToolbtn.size();i++){
        connect(vToolbtn[i],&QToolButton::clicked,[=](){
            //如果被打开了，就不要再次打开
            if(isShow[i]){
                QString str = QString("%1窗口已经被打开了").arg(vToolbtn[i]->text());
                QMessageBox::warning(this,"警告",str);
                return;
            }
            isShow[i] = true;


            //弹出聊天对话框
            //构造聊天窗口时候，告诉这个窗口他的名字，参数1 顶层方式弹出
            //注意！widget构造函数 并没有这两个参数
            Widget * widget = new Widget(0,vToolbtn[i]->text());
            widget->setWindowTitle(vToolbtn[i]->text());
            widget->setWindowIcon(vToolbtn[i]->icon());
            widget->show();
            //如何窗口关闭将标志位设置回来
            connect(widget,&Widget::closeWidget,[=](){
                  isShow[i] = false;
             });
        });
    }

}

DialogList::~DialogList()
{
    delete ui;
}
