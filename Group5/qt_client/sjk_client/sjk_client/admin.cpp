#include "admin.h"
#include "ui_admin.h"

admin::admin(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::admin)
{
    ui->setupUi(this);
    connectMysql();

    ui->tabWidget->setCurrentIndex(0);//默认显示tableWidget第0个标签页

    //学生信息管理
    ui->stackedWidget_stu->setCurrentIndex(0);//默认显示stackedWidget第0个布局
    connect(ui->btn_stu_menu_0,&QPushButton::clicked,[=](){ui->stackedWidget_stu->setCurrentIndex(0);});//将stackedWidget与按钮进行绑定
    connect(ui->btn_stu_menu_1,&QPushButton::clicked,[=](){ui->stackedWidget_stu->setCurrentIndex(1);});
    connect(ui->btn_stu_menu_2,&QPushButton::clicked,[=](){ui->stackedWidget_stu->setCurrentIndex(2);});

    //更新当前选课状态
    connect(ui->btn_stu_menu_2,&QPushButton::clicked,[=](){
        QSqlQuery query(db);
        //判断是否处于选课阶段
        query.exec("select f_state from func where f_name='course_select'");
        query.next();
        if (query.value(0).toString()=="0") {
            ui->lineEdit_stu_3_s->setText("关闭");
            ui->btn_stu_3_3->setText("开启");
        }
        else {
            ui->lineEdit_stu_3_s->setText("开启");
            ui->btn_stu_3_3->setText("关闭");
        }
    });


    //学生基本信息管理
    model_stu = new QSqlTableModel;
    model_stu->setTable("student");
    ui->tableView_stu->setModel(model_stu);
    connect(ui->btn_stu_menu_0,&QPushButton::clicked,[=](){model_stu->select();});

    model_stu->setHeaderData(0,Qt::Horizontal,"学号");
    model_stu->setHeaderData(1,Qt::Horizontal,"姓名");
    model_stu->setHeaderData(2,Qt::Horizontal,"性别");
    model_stu->setHeaderData(3,Qt::Horizontal,"年龄");
    model_stu->setHeaderData(4,Qt::Horizontal,"院系");
    model_stu->setHeaderData(5,Qt::Horizontal,"密码");
    model_stu->setHeaderData(6,Qt::Horizontal,"登记情况");

    model_stu->setEditStrategy(QSqlTableModel::OnManualSubmit);
    model_stu->select();//初始化学生基本信息表

    //考试成绩管理
    model_stu_score = new QSqlTableModel;
    model_stu_score->setTable("selection");
    ui->tableView_stu_score->setModel(model_stu_score);
    connect(ui->btn_stu_menu_1,&QPushButton::clicked,[=](){model_stu_score->select();});

    model_stu_score->setHeaderData(0,Qt::Horizontal,"学号");
    model_stu_score->setHeaderData(1,Qt::Horizontal,"课程号");
    model_stu_score->setHeaderData(2,Qt::Horizontal,"成绩");
    model_stu_score->setHeaderData(3,Qt::Horizontal,"是否补考");

    model_stu_score->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //选课界面 信号与槽
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

    //教师信息管理
    model_tch = new QSqlTableModel;
    model_tch->setTable("teacher");
    ui->tableView_tch->setModel(model_tch);
    connect(ui->tabWidget,&QTabWidget::tabBarClicked,[=](int a){if (a==1) model_tch->select();});

    model_tch->setHeaderData(0,Qt::Horizontal,"工号");
    model_tch->setHeaderData(1,Qt::Horizontal,"姓名");
    model_tch->setHeaderData(2,Qt::Horizontal,"性别");
    model_tch->setHeaderData(3,Qt::Horizontal,"所属院系");
    model_tch->setHeaderData(4,Qt::Horizontal,"职称");
    model_tch->setHeaderData(5,Qt::Horizontal,"年龄");
    model_tch->setHeaderData(6,Qt::Horizontal,"出生年月");
    model_tch->setHeaderData(7,Qt::Horizontal,"基本工资");
    model_tch->setHeaderData(8,Qt::Horizontal,"密码");

    model_tch->setEditStrategy(QSqlTableModel::OnManualSubmit);

    //课程信息管理
    model_course = new QSqlTableModel;//建立模型
    model_course->setTable("course");//指定使用course表
    ui->tableView_course->setModel(model_course);//将模型放入tableView
    connect(ui->tabWidget,&QTabWidget::tabBarClicked,[=](int a){if (a==2) model_course->select();});

    model_course->setHeaderData(0,Qt::Horizontal,"课程号");
    model_course->setHeaderData(1,Qt::Horizontal,"课程名");
    model_course->setHeaderData(2,Qt::Horizontal,"课时数");
    model_course->setHeaderData(3,Qt::Horizontal,"课程类型");
    model_course->setHeaderData(4,Qt::Horizontal,"学分");

    model_course->setEditStrategy(QSqlTableModel::OnManualSubmit);//设置模型的编辑模式，手动提交修改

}

admin::~admin()
{
    delete ui;
    db.close();
}

void admin::connectMysql()//连接mysql数据库
{
    db = QSqlDatabase::database();
}

void admin::on_btn_stu_add_clicked()
{
    QSqlRecord record = model_stu->record();
    int row = model_stu->rowCount();
    model_stu->insertRecord(row,record);
}

void admin::on_btn_stu_del_clicked()
{
    QItemSelectionModel *sModel = ui->tableView_stu->selectionModel();
    QModelIndexList list = sModel->selectedRows();

    for (int i=0;i<list.size();i++)
        model_stu->removeRow(list.at(i).row());
}

void admin::on_btn_stu_ok_clicked()
{
    if(model_stu->submitAll()){
        QMessageBox::information(nullptr,"提示","提交成功",QMessageBox::Ok);
    }
    else{
        QMessageBox::warning(nullptr,"警告","提交失败",QMessageBox::Ok);
    }
}

void admin::on_btn_stu_cancel_clicked()
{
    model_stu->revertAll();
    model_stu->submitAll();
}

void admin::on_btn_stu_search_clicked()
{
    QString text = ui->lineEdit_stu_1->text();
    if (text=="")
        model_stu->setFilter(nullptr);
    else {
        QString search_text = QString("stu_id = '%1' OR stu_name = '%2'").arg(text).arg(text);
        model_stu->setFilter(search_text);
    }
}

void admin::on_btn_stu_score_add_clicked()
{
    QSqlRecord record = model_stu_score->record();
    int row = model_stu_score->rowCount();
    model_stu_score->insertRecord(row,record);
}

void admin::on_btn_stu_score_del_clicked()
{
    QItemSelectionModel *sModel = ui->tableView_stu_score->selectionModel();
    QModelIndexList list = sModel->selectedRows();

    for (int i=0;i<list.size();i++)
        model_stu_score->removeRow(list.at(i).row());
}

void admin::on_btn_stu_score_ok_clicked()
{
    if(model_stu_score->submitAll()){
        QMessageBox::information(nullptr,"提示","提交成功",QMessageBox::Ok);
    }
    else{
        QMessageBox::warning(nullptr,"警告","提交失败",QMessageBox::Ok);
    }
}

void admin::on_btn_stu_score_cancel_clicked()
{
    model_stu_score->revertAll();
    model_stu_score->submitAll();
}

void admin::on_btn_stu_score_search_clicked()
{
    QString text = ui->lineEdit_stu_2->text();
    if (text=="")
        model_stu_score->setFilter(nullptr);
    else {
        QString search_text = QString("stu_id = '%1' OR course_id = '%2'").arg(text).arg(text);
        model_stu_score->setFilter(search_text);
    }
}

void admin::on_btn_stu_3_1_clicked()
{
    ui->tableWidget_1->setRowCount(0);//清空tableWidget_1
    ui->tableWidget_2->setRowCount(0);

    ui->tableWidget_1->setColumnCount(5);//设置tableWidget_1列数为5
    ui->tableWidget_2->setColumnCount(5);
    QStringList list;
    list<<"课程代号"<<"课程名"<<"课时数"<<"课程类型"<<"学分";
    ui->tableWidget_1->setHorizontalHeaderLabels(list);//设置tableWidget_1列标签
    ui->tableWidget_2->setHorizontalHeaderLabels(list);

    ui->tableWidget_1->setSelectionBehavior(QAbstractItemView::SelectRows);//设置tableWidget_1选择模式为选中行
    ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);

    //查询已选课程
    QString text = ui->lineEdit_stu_3->text();
    QSqlQuery query(db);
    QString stat = QString("select C.course_id,C.course_name,C.course_hour,C.course_type,C.course_credit "
                           "from course C,selection S "
                           "where S.stu_id='%1' AND S.course_id=C.course_id").arg(text);
    query.exec(stat);
    for (int i=0;query.next();i++) {
        //添加1行
        int row_count = ui->tableWidget_1->rowCount();
        ui->tableWidget_1->setRowCount(row_count+1);

        for (int j=0;j<5;j++)//添加内容
            ui->tableWidget_1->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
    }

    //查询未选课程
    stat = QString("select C.course_id,C.course_name,C.course_hour,C.course_type,C.course_credit "
                   "from course C "
                   "where C.course_id not in"
                   "(select C1.course_id "
                   "from course C1,selection S "
                   "where S.stu_id='%1' AND S.course_id=C1.course_id)").arg(text);
    query.exec(stat);
    for (int i=0;query.next();i++) {
        int row_count = ui->tableWidget_2->rowCount();
        ui->tableWidget_2->setRowCount(row_count+1);

        for (int j=0;j<5;j++)
            ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
    }
}

void admin::on_btn_stu_3_2_clicked()
{
    QString text = ui->lineEdit_stu_3->text();
    if (text.length()!=10)
        QMessageBox::warning(nullptr,"警告","保存失败！",QMessageBox::Ok);
    else {
        QSqlQuery query(db);
        QString stat = QString("delete from selection where stu_id='%1'").arg(text);    //删除该学生所有选课
        query.exec(stat);

        //从tableWidget_1读取数据，更新选课结果
        bool result = true;
        int count = ui->tableWidget_1->rowCount();
        for (int i=0; i<count;i++) {
            stat = QString("insert into selection values('%1','%2',0)").arg(text).arg(ui->tableWidget_1->item(i,0)->text());
            if (!query.exec(stat)) {
                QMessageBox::warning(nullptr,"警告","该学号不存在！",QMessageBox::Ok);
                result = false;
                break;
            }
        }
        if(result) QMessageBox::information(nullptr,"提示","保存成功！",QMessageBox::Ok);
    }

}

void admin::on_btn_stu_3_3_clicked()
{
    QString line_text = ui->lineEdit_stu_3_s->text();
    QString btn_text = ui->btn_stu_3_3->text();

    QSqlQuery query(db);
    QSqlQuery query2(db);
    QSqlQuery query3(db);
    if (line_text=="关闭")
        query.exec("update func set f_state='1' where 1");
    else
        query.exec("update func set f_state='0' where 1");
    ui->lineEdit_stu_3_s->setText(btn_text);
    ui->btn_stu_3_3->setText(line_text);
    //选课系统关闭后，检测课程人数，若小于15人，则删除该课程的所有选课记录
    if (line_text == "关闭"){
        query.exec(QString("select course_id from course"));
        while(query.next()){
            query2.exec(QString("select count(*) from selection where course_id = '%1'").arg(query.value(0).toString()));
            query2.next();
            if (query2.value(0).toInt()<15)
                query3.exec(QString("delete from selection where course_id = '%1'").arg(query.value(0).toString()));
        }
    }

}

void admin::on_btn_tch_add_clicked()
{
    QSqlRecord record = model_tch->record();
    int row = model_tch->rowCount();
    model_tch->insertRecord(row,record);
}

void admin::on_btn_tch_del_clicked()
{
    QItemSelectionModel *sModel = ui->tableView_tch->selectionModel();
    QModelIndexList list = sModel->selectedRows();

    for (int i=0;i<list.size();i++)
        model_tch->removeRow(list.at(i).row());
}

void admin::on_btn_tch_ok_clicked()
{
    if(model_tch->submitAll()){
        QMessageBox::information(nullptr,"提示","提交成功",QMessageBox::Ok);
    }
    else{
        QMessageBox::warning(nullptr,"警告","提交失败",QMessageBox::Ok);
    }
}

void admin::on_btn_tch_cancel_clicked()
{
    model_tch->revertAll();
    model_tch->submitAll();
}

void admin::on_btn_tch_search_clicked()
{
    QString text = ui->lineEdit_tch->text();
    if (text=="")
        model_tch->setFilter(nullptr);
    else {
        QString search_text = QString("tch_id = '%1' OR tch_name = '%2'").arg(text).arg(text);
        model_tch->setFilter(search_text);
    }
}

void admin::on_btn_course_add_clicked()
{
    QSqlRecord record = model_course->record();//添加空记录
    int row = model_course->rowCount();//获取行号
    model_course->insertRecord(row,record);//插入行
}

void admin::on_btn_course_ok_clicked()
{
    if(model_course->submitAll()){
        QMessageBox::information(nullptr,"提示","提交成功",QMessageBox::Ok);
    }
    else{
        QMessageBox::warning(nullptr,"警告","提交失败",QMessageBox::Ok);
    }//提交
}

void admin::on_btn_course_cancel_clicked()
{
    model_course->revertAll();//取消所有动作
    model_course->submitAll();//提交
}

void admin::on_btn_course_del_clicked()
{
    QItemSelectionModel *sModel = ui->tableView_course->selectionModel();//获取选中模型
    QModelIndexList list = sModel->selectedRows();//取出模型索引

    //删除所有选中行
    for (int i=0;i<list.size();i++)
        model_course->removeRow(list.at(i).row());
}

void admin::on_btn_course_search_clicked()
{
    QString text = ui->lineEdit_course->text();
    if (text=="")
        model_course->setFilter(nullptr);
    else {
        QString search_text = QString("course_id = '%1' OR course_name = '%2'").arg(text).arg(text);
        model_course->setFilter(search_text);
    }
}

void admin::on_btn_tch_work_clicked()
{
    tch_view = new tchWork_view;
    tch_view->show();
}

void admin::on_btn_dept_query_clicked()
{
    QString text = ui->lineEdit_dept->text();
    QSqlQuery query(db);
    QString str;
    if (text.length()!=0 && text[0]>='0' && text[0]<='9')
        str = QString("select dept_id, dept_name, dept_master from department where dept_id='%1'").arg(text);
    else
        str = QString("select dept_id, dept_name, dept_master from department where dept_name='%1'").arg(text);
    qDebug()<<str;
    query.exec(str);
    qDebug()<<query.exec(str);
    query.next();
    if(query.value(2).toString()==NULL){
        ui->lineEdit_dept_1->setText(query.value(0).toString());
        ui->lineEdit_dept_2->setText(query.value(1).toString());
        ui->lineEdit_dept_3->clear();
    }
    else{
        ui->lineEdit_dept_1->setText(query.value(0).toString());
        ui->lineEdit_dept_2->setText(query.value(1).toString());
        str = QString("select tch_name from teacher where tch_id = '%1'").arg(query.value(2).toString());
        query.exec(str);
        qDebug()<<query.exec(str);
        query.next();
        ui->lineEdit_dept_3->setText(query.value(0).toString());
    }
}

void admin::on_btn_dept_add_clicked()
{
    QString id = ui->lineEdit_dept_1->text();
    QString name = ui->lineEdit_dept_2->text();
    QString master = ui->lineEdit_dept_3->text();

    if (id.length()==0)
        QMessageBox::warning(nullptr,"警告","请输入编号！",QMessageBox::Ok);
    else if (name.length()==0)
        QMessageBox::warning(nullptr,"警告","请输入院系名！",QMessageBox::Ok);
    else if (master.length()==0){
        master = "";
        QSqlQuery query(db);
        QString str;
        str = QString("insert into department(dept_id,dept_name,dept_master) values('%1','%2',null)").arg(id).arg(name);
        qDebug()<<str;
        query.exec(str);
        qDebug()<<query.exec(str);
        str = QString("select * from department where dept_id = '%1'").arg(id);
        query.exec(str);
        if(query.next()){
            QMessageBox::information(nullptr,"提示","添加成功",QMessageBox::Ok);
        }
        else{
            QMessageBox::warning(nullptr,"警告","添加失败！",QMessageBox::Ok);
        }
    }
    else {
        QSqlQuery query(db);
        QString str;

        str = QString("select tch_id from teacher where tch_name='%1'").arg(master);
        query.exec(str);
        if (!query.next())
            QMessageBox::warning(nullptr,"警告","负责人不存在！",QMessageBox::Ok);
        else {
            QString master = query.value(0).toString();

            str = QString("insert into department values('%1','%2','%3')").arg(id).arg(name).arg(master);
            if (query.exec(str))
                QMessageBox::information(nullptr,"提示","添加成功！",QMessageBox::Ok);
            else
                QMessageBox::warning(nullptr,"警告","添加失败！",QMessageBox::Ok);
        }
    }

}

void admin::on_btn_dept_del_clicked()
{
    QString id = ui->lineEdit_dept_1->text();
    QString name = ui->lineEdit_dept_2->text();

    if (id.length()==0)
        QMessageBox::warning(nullptr,"警告","请输入编号！",QMessageBox::Ok);
    else if (name.length()==0)
        QMessageBox::warning(nullptr,"警告","请输入院系名！",QMessageBox::Ok);
    else {
        QSqlQuery query(db);
        QString str = QString("delete from department where dept_id='%1' AND dept_name='%2'").arg(id).arg(name);
        if (query.exec(str))
            QMessageBox::information(nullptr,"提示","删除成功！",QMessageBox::Ok);
        else
            QMessageBox::warning(nullptr,"警告","删除失败！",QMessageBox::Ok);
    }
}

void admin::on_btn_dept_clear_clicked()
{
    ui->lineEdit_dept_1->clear();
    ui->lineEdit_dept_2->clear();
    ui->lineEdit_dept_3->clear();
}

void admin::on_btn_dept_save_clicked()
{
    QString id = ui->lineEdit_dept_1->text();
    QString name = ui->lineEdit_dept_2->text();
    QString master = ui->lineEdit_dept_3->text();

    if (id.length()==0)
        QMessageBox::warning(nullptr,"警告","请输入编号！",QMessageBox::Ok);
    else if (name.length()==0)
        QMessageBox::warning(nullptr,"警告","请输入院系名！",QMessageBox::Ok);
    else if (master.length()==0)
        QMessageBox::warning(nullptr,"警告","请输入负责人！",QMessageBox::Ok);
    else {
        QSqlQuery query(db);
        QString str;

        str = QString("select tch_id from teacher where tch_name='%1'").arg(master);
        query.exec(str);
        if (!query.next())
            QMessageBox::warning(nullptr,"警告","负责人不存在！",QMessageBox::Ok);
        else {
            QString master = query.value(0).toString();

            str = QString("update department set dept_name='%1', dept_master='%2' where dept_id='%3' ").arg(name).arg(master).arg(id);
            if (query.exec(str))
                QMessageBox::information(nullptr,"提示","保存成功！",QMessageBox::Ok);
            else
                QMessageBox::warning(nullptr,"警告","保存失败！",QMessageBox::Ok);
        }
    }
}

void admin::on_btn_dept_view_clicked()
{
    dept_view = new department_view;
    dept_view->show();
}

void admin::on_btn_tch_course_clicked()
{
    tch_course_view = new tech_course_view;
    tch_course_view->show();
}

