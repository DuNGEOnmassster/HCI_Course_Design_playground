#include "fdform.h"
#include "ui_fdform.h"
#include <QIcon>
#include <QToolButton>
#include <QMessageBox>
#include "Widget.h"

fdform::fdform(QWidget *parent, QString name) :
    QWidget(parent),
    ui(new Ui::fdform)
{
    ui->setupUi(this);
    //this->setWindowIcon(QIcon(":/img/QQtubiao.png"));
    //this->setWindowTitle("QQ 2023");

    QList<QString>nameList;
     nameList << "公共聊天室1" << "公共聊天室2" <<"公共聊天室3"<<"公共聊天室4"<<"公共聊天室5"
              <<"公共聊天室6"<<"公共聊天室7"<<"公共聊天室8"<<"公共聊天室9";
    QStringList iconNameList; //图标资源列表
     iconNameList << "qq"<< "qq" <<"qq" <<"qq"<< "qq"
                  <<"qq"<<"qq"<<"qq"<<"qq";

    QVector<QToolButton*> vToolBtn;
    for(int i = 0; i < 9; i++){
        QToolButton * btn = new QToolButton;
        btn->setText(nameList[i]);
        QString str = QString(":/img/%1.png").arg(iconNameList.at(i));
        btn->setIcon(QPixmap(str));
        btn->setIconSize(QPixmap(str).size());
        btn->setAutoRaise(true);
        //设置文字和图片一起显示
        btn->setToolButtonStyle(Qt::ToolButtonTextBesideIcon);
        ui->verticalLayout->addWidget(btn);
        vToolBtn.push_back(btn);
        //将标识默认初始化为false
        isShow.push_back(false);
    }
    myName = name;
    for(int i = 0; i < 9; i++){
        connect(vToolBtn[i], &QToolButton::clicked,[=](){
                if(isShow[i]){
                         QString str = QString("%1窗口已经被打开了").arg(vToolBtn[i]->text());
                         QMessageBox::warning(this,"警告",str);
                         return;
                     }
                 isShow[i] = true;
                 //弹出聊天对话框
                 //构造聊天窗口时候，告诉这个窗口他的名字，参数1 顶层方式弹出
                 //注意！widget构造函数 并没有这两个参数
                 qint16 portnum = 9990 + i;
                 Widget * widget = new Widget(0,myName,portnum);
                 widget->setWindowTitle(vToolBtn[i]->text());
                 widget->setWindowIcon(vToolBtn[i]->icon());
                 //widget->port = 9999;
                 widget->show();
                 //如果窗口关闭将标志位设置回来
                 connect(widget,&Widget::closeWidget,[=](){
                       isShow[i] = false;
                  });

             });
    }
}

void fdform::closeEvent(QCloseEvent * e){
       emit this->closeFdform(); //发送关闭当前窗口的自定义信号
      QWidget::closeEvent(e);
 }

fdform::~fdform()
{
    delete ui;
}
