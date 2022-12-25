#include "function.h"
#include "ui_function.h"
#include "widget.h"

function::function(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::function)
{
    ui->setupUi(this);
    t_id = Widget::tid;

    //登记
    model3 = new QSqlTableModel;
    model3 -> setTable("selection");
    ui->tableView_3->setModel(model3);

    model3->setHeaderData(0,Qt::Horizontal,"学号");
    model3->setHeaderData(1,Qt::Horizontal,"课程号");
    model3->setHeaderData(2,Qt::Horizontal,"成绩");
    //model3->setHeaderData(3,Qt::Horizontal,"是否补考");

    model3->setEditStrategy(QSqlTableModel::OnManualSubmit); //设置为提交后修改才能生效
    ui->tabWidget->setCurrentIndex(0);

    connect(ui->tabWidget,&QTabWidget::currentChanged,[=](int a){
        if(a==1){
            //每个老师能看到自己带的课的成绩信息
            model3->setFilter(QString("course_id in (select course_id from teaching where tch_id = '%1')").arg(t_id));
            model3->select();
        }
    });
}

function::~function()
{
    delete ui;
}

//工作量界面的查询按钮
void function::on_inquiry_clicked()
{
    QString account = ui->account->text();
    //工作量
    QStandardItemModel *model = new QStandardItemModel;
    model->setColumnCount(4);
    model->setHorizontalHeaderItem(0,new QStandardItem("课程编号"));
    model->setHorizontalHeaderItem(1,new QStandardItem("课程名称"));
    model->setHorizontalHeaderItem(2,new QStandardItem("开课时间"));
    model->setHorizontalHeaderItem(3,new QStandardItem("学时"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);//表头信息居中

    QSqlQuery sql_query;
    QString del = QString("select C.course_id,C.course_name,T.since,C.course_hour "
                          "from course C,teaching T "
                          "where T.tch_id = '%1' and T.course_id = C.course_id").arg(account);
    sql_query.exec(del);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        long row = 0;
        while(sql_query.next())
        {
            course_id = sql_query.value(0).toString();
            course_name = sql_query.value(1).toString();
            course_hours = sql_query.value(3).toString();
            since = sql_query.value(2).toString();

            QStringList list;
            list<<course_id<<course_name<<since<<course_hours;
            for(int i = 0;i<list.length();i++){
                 model->setItem(row,i,new QStandardItem(list.at(i)));
            }
            row ++;

        }
    }


}

//显示全部按钮
void function::on_show_all_clicked()
{
    this->ui->account->clear();
    //工作量
    QStandardItemModel *model = new QStandardItemModel;
    model->setColumnCount(4);
    model->setHorizontalHeaderItem(0,new QStandardItem("课程编号"));
    model->setHorizontalHeaderItem(1,new QStandardItem("课程名称"));
    model->setHorizontalHeaderItem(2,new QStandardItem("开课时间"));
    model->setHorizontalHeaderItem(3,new QStandardItem("学时"));
    ui->tableView->setModel(model);
    ui->tableView->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);//表头信息居中

    QSqlQuery sql_query;
    QString del = QString("select C.course_id,C.course_name,T.since,C.course_hour "
                          "from course C,teaching T "
                          "where T.course_id = C.course_id");
    sql_query.exec(del);
    if(!sql_query.exec())
    {
        qDebug()<<sql_query.lastError();
    }
    else
    {
        long row = 0;
        while(sql_query.next())
        {
            course_id = sql_query.value(0).toString();
            course_name = sql_query.value(1).toString();
            course_hours = sql_query.value(3).toString();
            since = sql_query.value(2).toString();

            QStringList list;
            list<<course_id<<course_name<<since<<course_hours;
            for(int i = 0;i<list.length();i++){
                 model->setItem(row,i,new QStandardItem(list.at(i)));
            }
            row ++;

        }
    }
}

//统计成绩界面的确定按钮
void function::on_tongji_clicked()
{
    QString dept = ui->dept->text();
    QString course = ui->course->text();
    //统计
    QStandardItemModel *model1 = new QStandardItemModel;
    model1->setColumnCount(4);
    model1->setHorizontalHeaderItem(0,new QStandardItem("姓名"));
    model1->setHorizontalHeaderItem(1,new QStandardItem("学号"));
    model1->setHorizontalHeaderItem(2,new QStandardItem("科目"));
    model1->setHorizontalHeaderItem(3,new QStandardItem("成绩"));
    ui->tableView_2 ->setModel(model1);
    ui->tableView_2->horizontalHeader()->setDefaultAlignment(Qt::AlignCenter);//表头信息居中

   QSqlQuery sql_query;
   QString del = QString("select S1.stu_name,S1.stu_id,C.course_name,S2.score "
                         "from student S1,course C,selection S2 "
                         "where S1.stu_id = S2.stu_id and S1.stu_dept = '%1' and C.course_name = '%2' and C.course_id = S2.course_id").arg(dept).arg(course);
   qDebug()<<del;
   sql_query.exec(del);
   if(!sql_query.exec())
   {
       qDebug()<<sql_query.lastError();
   }
   else
   {
       long row = 0;
       while(sql_query.next())
       {
           stu_name = sql_query.value(0).toString();
           stu_id = sql_query.value(1).toString();
           stu_course = sql_query.value(2).toString();
           score = sql_query.value(3).toString();

           qDebug()<<stu_name;
           qDebug()<<stu_id;

           QStringList list;
           list<<stu_name<<stu_id<<stu_course<<score;
           for(int i = 0;i<list.length();i++){
                model1->setItem(row,i,new QStandardItem(list.at(i)));
           }
           row ++;

       }
   }

}

//登记成绩界面的提交
void function::on_sure_clicked()
{
    if(model3->submitAll()){
        QMessageBox::information(nullptr,"提示","修改成功",QMessageBox::Ok);
    }
    else{
        QMessageBox::warning(nullptr,"警告","修改失败",QMessageBox::Ok);
    }
}

//修改密码界面的确定按钮
void function::on_pushButton_clicked()
{
    QString pwd_old = ui->lineEdit_1->text();
    QString pwd_new = ui->lineEdit_2->text();
    if(pwd_new == "" || pwd_old == ""){
        QMessageBox::warning(this,"警告","请填写完整！","确定");
    }else {
         QSqlQuery sql_query;
         QString pwd_sql;
         bool ispass = false;
         QString search = QString("select tch_passwd from teacher where tch_id = '%1'").arg(Widget::tid);
         qDebug()<<search;
         sql_query.exec(search);
         while(sql_query.next()){
             pwd_sql = sql_query.value(0).toString();
             qDebug()<<pwd_sql<<"  "<<pwd_old;
             if(pwd_sql == pwd_old){
                 ispass = true;
             }
         }
         if(ispass){
             QString change = QString("update teacher set tch_passwd = '%1' where tch_id = '%2'").arg(pwd_new).arg(Widget::tid);
             if(sql_query.exec(change)){
                 QMessageBox::information(this,"提示","密码修改成功！","确定");
             }else {
                 QMessageBox::warning(this,"警告","修改失败！","确定");
             }
         }else {
              QMessageBox::warning(this,"警告","原密码错误！","确定");
         }


     }
}
