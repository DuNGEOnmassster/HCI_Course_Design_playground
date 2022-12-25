#include "widget.h"
#include "ui_widget.h"


QSqlDatabase Widget::database;
QStringList Widget::list;
QString Widget::sid;
QString Widget::tid;
QString Widget::aid;

Widget::Widget(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Widget)
{
    ui->setupUi(this);

    //ui->pushButton->setEnabled(false);

    // 1.加载驱动,加载失败会自动抛异常
    database = QSqlDatabase::addDatabase("QMYSQL");
    // 2. 设置IP地址
    database.setHostName("127.0.0.1");
    // 3. 设置端口
    database.setPort(3306);
    // 4. 设置用户名
    database.setUserName("root");
    // 5. 设置该用户名密码
    database.setPassword("8512998hjc10");
    // 6. 设置用户
    database.setDatabaseName("education");
    // 7. 设置连接选项, 保持连接
    database.setConnectOptions("MYSQL_OPT_CONNECT_TIMEOUT");

    if(!database.open()) {
        QString msg = QString("数据库连接失败：\n%1").arg(database.lastError().text());
        QMessageBox::warning(this,"错误",msg,"确定");
    }
    else{
        qDebug() << "数据库连接成功";
    }
}

Widget::~Widget()
{
    delete ui;
}

void Widget::on_pbtn_login_clicked()
{
    QString acc = ui->le_acc->text();
    QString pwd = ui->le_pwd->text();
    QString iden = ui->buttonGroup->checkedButton()->text();
   if(acc == "" || pwd == ""){
       QMessageBox::information(this,"提示","账号密码不能为空",QMessageBox::Ok);

    }else if(acc != "" && pwd != ""){
        bool ispass = false;
        QSqlQuery sql_query(database);
        if(iden == "学生"){
            Widget::sid = acc;
            QString sqlstat = QString("select stu_passwd from student where stu_id = '%1'").arg(acc);
            sql_query.exec(sqlstat);
            while(sql_query.next()){
                QString sql_pwd = sql_query.value(0).toString();
                if(sql_pwd == pwd){
                    ispass = true;
                }
            }
            if(ispass){
                hm = new home(this);
                hm->show();
                //this->hide();

            }else {
                QMessageBox::warning(this,"错误","账号或密码不正确    ","确定");
            }

        }else if(iden == "老师"){
            Widget::tid = acc;
            QString sqlstat = QString("select tch_passwd from teacher where tch_id = '%1'").arg(acc);
            sql_query.exec(sqlstat);
            while(sql_query.next()){
                QString sql_pwd = sql_query.value(0).toString();
                if(sql_pwd == pwd){
                    ispass = true;
                }
            }
            if(ispass){
               ft = new function(this);
               ft->show();
               //this->hide();
            }else {
                QMessageBox::warning(this,"错误","账号或密码不正确    ","确定");
            }

        }else if(iden == "管理员"){
            Widget::aid = acc;
            QString sqlstat = QString("select password from administrator where admin_id = '%1'").arg(acc);
            sql_query.exec(sqlstat);
            while(sql_query.next()){
                QString sql_pwd = sql_query.value(0).toString();
                if(sql_pwd == pwd){
                    ispass = true;
                }
            }
            if(ispass){
                adm = new admin;
                adm->show();
                //this->hide();
            }else {
                QMessageBox::warning(this,"错误","账号或密码不正确    ","确定");
            }

        }
    }
}
