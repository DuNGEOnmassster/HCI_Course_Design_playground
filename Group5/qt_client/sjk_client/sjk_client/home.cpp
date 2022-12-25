#include "home.h"
#include "ui_home.h"

#include "widget.h"

home::home(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::home)
{
    ui->setupUi(this);

    currentRow = 0;

    f_stat = "";

    init_checkin();

    init_score();

    init_selection();

    ui->tabWidget->setCurrentIndex(2);

    connect(ui->tableWidget_1,&QTableWidget::cellClicked,[=](int row){
        ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);//添加一行
        for (int i=0;i<5;i++)
            ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,i,new QTableWidgetItem(ui->tableWidget_1->item(row,i)->text()));//复制选中行内容
        ui->tableWidget_1->removeRow(row);
    });

    connect(ui->tableWidget_2,&QTableWidget::cellClicked,[=](int row){
        ui->tableWidget_1->setRowCount(ui->tableWidget_1->rowCount()+1);//添加一行
        for (int i=0;i<5;i++)
            ui->tableWidget_1->setItem(ui->tableWidget_1->rowCount()-1,i,new QTableWidgetItem(ui->tableWidget_2->item(row,i)->text()));//复制选中行内容
        ui->tableWidget_2->removeRow(row);
    });
}

home::~home()
{
    delete ui;
}

//显示数据
void home::display(int row, QStringList lists){
    for (int i = 0;i < lists.length();i++) {
        this->model->setItem(row,i,new QStandardItem(lists.at(i)));
    }
}

// 成绩信息界面
void home::init_score(){
    model = new QStandardItemModel;
    model->clear();
    //设置表头名称
    model->setHorizontalHeaderItem(0,new QStandardItem("课程名称"));
    model->setHorizontalHeaderItem(1,new QStandardItem("课程类型"));
    model->setHorizontalHeaderItem(2,new QStandardItem("学分"));
    model->setHorizontalHeaderItem(3,new QStandardItem("得分"));
    //model->setHorizontalHeaderItem(4,new QStandardItem("得分性质"));

    //显示表头
    ui->tableView->setModel(model);

    //设置表格宽度
    ui->tableView->setColumnWidth(0,100);
    ui->tableView->setColumnWidth(1,100);
    ui->tableView->setColumnWidth(2,100);
    ui->tableView->setColumnWidth(3,100);
    //ui->tableView->setColumnWidth(4,100);

    QSqlQuery query(Widget::database);
    QString course_name,course_type,course_credit,score;
    QString  search = QString("select C.course_name,C.course_type,C.course_credit,S.score "
                              " from course C, selection S "
                              "where C.course_id = S.course_id and S.stu_id = '%1'").arg(Widget::sid);
    query.exec(search);
    while (query.next()) {
        course_name = query.value(0).toString();
        course_type = query.value(1).toString();
        course_credit = query.value(2).toString();
        score = query.value(3).toString();
        //again = query.value(4).toString();
        QString tmp = QString("%0*%1*%2*%3").arg(course_name).arg(course_type).arg(course_credit).arg(score);
        QStringList cnts = tmp.split('*');//按*分割
        display(currentRow++,cnts);
    }

    if(currentRow < 10){
        for (int i = currentRow;i < 10; i++) {
            for (int j = 0;j < 4;j++) {
                this->model->setItem(i,j,new QStandardItem(""));
            }
        }
    }
    currentRow = 0;
    ui->tableView->setEditTriggers(QAbstractItemView::NoEditTriggers);
}

//入学登记界面，显示学生基本信息
void  home::init_checkin(){
    QString stu_name,stu_gen,stu_age,stu_deptname,is_check;
    QSqlQuery sql_query(Widget::database);
    QString search = QString("select * from student where stu_id = '%1'").arg(Widget::sid);
    sql_query.exec(search);
    while(sql_query.next()){
        stu_name = sql_query.value(1).toString();
        stu_gen = sql_query.value(2).toString();
        stu_age = sql_query.value(3).toString();
        stu_deptname = sql_query.value(4).toString();
        is_check = sql_query.value(6).toString();
    }
    if(is_check == 'Y'){
        ui->pbtn_ok->setEnabled(false);
        //已登记的学生不能编辑基本信息
        ui->le_age->setReadOnly(true);
        ui->le_gender->setReadOnly(true);
        ui->le_dept->setReadOnly(true);
        ui->le_id->setReadOnly(true);
        ui->le_name->setReadOnly(true);
    }
        /*
        QString stu_dept;
        QString search1 = QString("select dept_name from department where dept_name = '%1'").arg(stu_deptname);
        sql_query.exec(search1);
        while(sql_query.next()){
            stu_dept = sql_query.value(0).toString();
        }
        */
        ui->le_id->setText(Widget::sid);
        ui->le_age->setText(stu_age);
        ui->le_dept->setText(stu_deptname);
        ui->le_gender->setText(stu_gen);
        ui->le_name->setText(stu_name);

}

//显示选课界面
void home::init_selection(){
    //清空tableweidget
    ui->tableWidget_1->setRowCount(0);
    ui->tableWidget_2->setRowCount(0);

    //设置tablewidget列数
    ui->tableWidget_1->setColumnCount(5);
    ui->tableWidget_2->setColumnCount(5);

    QStringList list;
    list<<"课程代号"<<"课程名"<<"课时数"<<"课程类型"<<"学分";
    ui->tableWidget_1->setHorizontalHeaderLabels(list);//设置tableWidget_1列标签
    ui->tableWidget_2->setHorizontalHeaderLabels(list);

    ui->tableWidget_1->setSelectionBehavior(QAbstractItemView::SelectRows);//设置tableWidget_1选择模式为选中行
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);

    //查询已选课程
    QSqlQuery query(Widget::database);
    QString stat = QString("select C.course_id,C.course_name,C.course_hour,C.course_type,C.course_credit "
                            "from course C,selection S "
                            "where S.stu_id='%1' AND S.course_id=C.course_id").arg(Widget::sid);
    query.exec(stat);
    for (int i=0;query.next();i++) {
        //添加1行
        int row_count = ui->tableWidget_1->rowCount();
        ui->tableWidget_1->setRowCount(row_count+1);

        for (int j=0;j<5;j++)//添加内容
            ui->tableWidget_1->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
    }

    stat = QString("select f_state from func where f_name ='course_select'");
    query.exec(stat);
    while(query.next()){
        f_stat = query.value(0).toString();
    }
    if (f_stat == "1") {
        //查询未选课程
        stat = QString("select C.course_id,C.course_name,C.course_hour,C.course_type,C.course_credit "
                       "from course C "
                       "where C.course_id not in"
                       "(select C1.course_id "
                       "from course C1,selection S "
                       "where S.stu_id='%1' AND S.course_id=C1.course_id)").arg(Widget::sid);
        query.exec(stat);
        for (int i=0;query.next();i++) {
            int row_count = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->setRowCount(row_count+1);

            for (int j=0;j<5;j++)
                ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
        }
    }
}

//修改密码按钮
void home::on_pbtn_subch_clicked()
{
   QString pwd_old = ui->le_old->text();
   QString pwd_new = ui->le_new->text();
   if(pwd_new == "" || pwd_old == ""){
       QMessageBox::warning(this,"警告","密码不能为空！","确定");
   }else {
       //判断原密码是否正确
        QSqlQuery sql_query(Widget::database);
        QString pwd_sql;
        bool ispass = false;
        QString search = QString("select stu_passwd from student where stu_id = '%1'").arg(Widget::sid);
        sql_query.exec(search);
        while(sql_query.next()){
            pwd_sql = sql_query.value(0).toString();
            if(pwd_sql == pwd_old){
                ispass = true;
            }
        }
        //正确则允许更改密码
        if(ispass){
            QString change = QString("update student set stu_passwd = '%1' where stu_id = '%2'").arg(pwd_new).arg(Widget::sid);
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

//确认登记按钮
void home::on_pbtn_ok_clicked()
{
    ui->le_id->setEnabled(true);
    QString id = ui->le_id->text();
    QString stu_age = ui->le_age->text();
    QString stu_gen = ui->le_gender->text();
    QString stu_name = ui->le_name->text();

    if(stu_gen != 'M' && stu_gen != 'F'){
        QMessageBox::warning(this,"警告","性别请选择F或M",QMessageBox::Ok);
    }

    QSqlQuery sql_query(Widget::database);
    QString stu_deptid;

    QString search = QString("select stu_dept from student where stu_id = '%1'").arg(id);
    bool issearched = true;
    while(issearched){
        if(!sql_query.exec(search)){
            QMessageBox::warning(this,"警告","不存在该学院",QMessageBox::Ok);
        }else {
            while(sql_query.next()){
                stu_deptid = sql_query.value(0).toString();
                issearched = false;
            }
        }

    }

    QString change = QString("update student set stu_age = %1,stu_gen = '%2',stu_name = '%3',is_check = 'Y', "
                             "stu_dept = '%4' where stu_id = '%5'").arg(stu_age).arg(stu_gen).arg(stu_name).arg(stu_deptid).arg(id);
    if(sql_query.exec(change)){
        QMessageBox::information(this,"提示","提交成功","确定");
        ui->pbtn_ok->setEnabled(false);
        //提交后设置单元格不可编辑
        ui->le_age->setReadOnly(true);
        ui->le_gender->setReadOnly(true);
        ui->le_dept->setReadOnly(true);
        ui->le_id->setReadOnly(true);
        ui->le_name->setReadOnly(true);
    }else{
        QMessageBox::warning(this,"警告","入学登记只需提交一次！","确定");
    }
}

//保存选课按钮
void home::on_pushButton_clicked()
{
    if(f_stat == "0"){
        QMessageBox::information(this,"提示","选课暂未开启，请在规定时间内选课",QMessageBox::Ok);
        return;
    }
    QString text = Widget::sid;
    QSqlQuery query(Widget::database);
    QSqlQuery query1(Widget::database);
    for(int i = 0;i < ui->tableWidget_1->rowCount();i++){
        QString stat = QString("select * from selection where course_id = '%1'").arg(ui->tableWidget_1->item(i,0)->text());
        qDebug()<<stat;
        query1.exec(stat);
        int tmp_num=0;
        while(query1.next()) {tmp_num++;}
        qDebug() << tmp_num;
        if(tmp_num >= 50){
            QString mess = QString("课程 %1 选课人数已满50").arg(ui->tableWidget_1->item(i,1)->text());
            qDebug()<<ui->tableWidget_1->item(i,1)->text();
            QMessageBox::information(this,"确定",mess,QMessageBox::Ok);
            ui->tableWidget_2->setRowCount(ui->tableWidget_2->rowCount()+1);//添加一行
            for (int j=0;j<5;j++)
                ui->tableWidget_2->setItem(ui->tableWidget_2->rowCount()-1,j,new QTableWidgetItem(ui->tableWidget_1->item(i,j)->text()));//复制选中行内容
            ui->tableWidget_1->removeRow(i);

        }
    }

    QString stat = QString("delete from selection where stu_id='%1'").arg(text);    //删除该学生所有选课
    query.exec(stat);

    //从tableWidget_1读取数据，更新选课结果
    bool result = true;
    int count = ui->tableWidget_1->rowCount();
    if (!count);
    else{
        //判断总学分是否大于120
        float total_credit = 0;
        for (int i=0; i<count;i++) {
            total_credit += ui->tableWidget_1->item(i,4)->text().toFloat(nullptr);
        }
        qDebug() << total_credit;
        if(total_credit>120){
            QMessageBox::warning(nullptr,"警告","总学分不能高于120","确定");
        }else{
            for (int i=0; i<count;i++) {
                stat = QString("insert into selection values('%1','%2',0)").arg(text).arg(ui->tableWidget_1->item(i,0)->text());
                qDebug()<<stat;
                if (!query.exec(stat)) {
                            QMessageBox::warning(nullptr,"警告","！",QMessageBox::Ok);
                    result = false;
                    break;
                }
            }

            if(result){
                init_score();
                QMessageBox::information(nullptr,"提示","保存成功！",QMessageBox::Ok);
            }
        }

    }

}
