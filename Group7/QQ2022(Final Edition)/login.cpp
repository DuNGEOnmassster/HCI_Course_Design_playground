#include "login.h"
#include "ui_login.h"
#include <QMessageBox>
#include "widget.h"
#include "fdform.h"



login::login(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::login)
{
    //setWindowFlags(Qt::WindowType::FramelessWindowHint);

    ui->setupUi(this);
    this->setWindowIcon(QIcon(":/img/QQtubiao.png"));
    this->setWindowTitle("QQ 2023");


}

login::~login()
{
    delete ui;
}
void login::on_btnClose_clicked()
{
    this->close();
}


void login::on_btnMin_clicked()
{
    this->showMinimized();
}


void login::on_btnLogin_released()
{
    QList<QString>nameList;
     nameList << "周骏博" << "邓卓文" <<"徐绍倡"<<"潘星宇"<<"用户1"
              <<"用户2"<<"用户3"<<"用户4"<<"用户5";
    QStringList usrNameList; //账号
     usrNameList << "zjb"<< "dzw" <<"xsc" <<"pxy"<< "yonghu1"
                 <<"yonghu2"<<"yonghu3"<<"yonghu4"<<"yonghu5";
    QStringList passwordList; //密码
     passwordList << "zjb"<< "dzw" <<"xsc" <<"pxy"<< "yonghu1"
                  <<"yonghu2"<<"yonghu3"<<"yonghu4"<<"yonghu5";
    for(int i = 0; i < 4; i++){
        isShow.push_back(false);
    }
//    int i = 0;
//    while(1){
//        if(ui->username->currentText() == usrNameList[i] && ui->password->text() == passwordList[i]){

//        }
//    }
    int flag = 0;
    for(int i = 0; i < 5; i++){

        if(ui->usrname->text() == usrNameList[i] && ui->password->text() == passwordList[i]){
            if(isShow[i]){
                     QString str = QString("%1已登录").arg(nameList[i]);
                     QMessageBox::warning(this,"警告",str);
                     return;
                 }
            isShow[i] = true;
            flag = 1;
            fdform * fdlist = new fdform(0,nameList[i]);
            //Widget * widget = new Widget(0,usrNameList[i]);
            fdlist->setWindowTitle(usrNameList[i]);
            QString str = QString(":/img/%1.jpg").arg(usrNameList.at(i));
            fdlist->setWindowIcon(QPixmap(str));
            fdlist->show();
            //widget->show();
            connect(fdlist,&fdform::closeFdform,[=](){
                  isShow[i] = false;
             });
        }
        else if(i > 3 && flag == 0){
            QMessageBox::warning(this,"提示","账号或密码错误");
        }
    }
}

