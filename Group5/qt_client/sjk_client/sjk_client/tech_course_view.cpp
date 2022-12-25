#include "tech_course_view.h"
#include "ui_tech_course_view.h"

tech_course_view::tech_course_view(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tech_course_view)
{
    ui->setupUi(this);
    db = QSqlDatabase::database();
}

tech_course_view::~tech_course_view()
{
    delete ui;
}

//清空
void tech_course_view::on_pbn_clear_clicked()
{
    ui->lineEdit_tch_id->clear();
    ui->lineEdit_course_id->clear();
    ui->lineEdit_since->clear();
    ui->lineEdit_q_cid->clear();
    ui->lineEdit_q_tid->clear();
}

void tech_course_view::on_pbn_add_clicked()
{
    QString tch_id = ui->lineEdit_tch_id->text();
    QString c_id = ui->lineEdit_course_id->text();
    //QString c_name = ui->lineEdit_course_name->text();
    QString since = ui->lineEdit_since->text();

    if(tch_id.length()==0){
        QMessageBox::warning(nullptr,"警告","请输入教师职工号",QMessageBox::Ok);
    }
    else if(c_id.length()==0){
        QMessageBox::warning(nullptr,"警告","请输入课程号",QMessageBox::Ok);
    }
    else if(since.length()==0){
        QMessageBox::warning(nullptr,"警告","请输入开课日期",QMessageBox::Ok);
    }
    else{
        QSqlQuery query(db);
        QString str;

        str = QString("select * from teaching where course_id = '%1'").arg(c_id);
        query.exec(str);
        if(query.next()){
            QMessageBox::warning(nullptr,"警告","该课程已有教师授课",QMessageBox::Ok);
        }
        else{
            QString instert = QString("insert into teaching values('%1','%2','%3')").arg(tch_id).arg(c_id).arg(since);
            if(query.exec(instert)){
                QMessageBox::information(nullptr,"提示","添加成功",QMessageBox::Ok);
            }
            else{
                QMessageBox::warning(nullptr,"警告","添加失败",QMessageBox::Ok);
            }
        }
    }
}

void tech_course_view::on_pbn_del_clicked()
{
    QString c_id = ui->lineEdit_course_id->text();
    QString tch_id = ui->lineEdit_tch_id->text();

    if(c_id.length()==0||tch_id.length()==0){
        QMessageBox::warning(nullptr,"警告","请输入课程号和职工号",QMessageBox::Ok);
    }
    else{
        QSqlQuery query(db);
        QString del = QString("delete from teaching where tch_id = '%1' and course_id = '%2'").arg(tch_id).arg(c_id);
        if(query.exec(del)){
            QMessageBox::information(nullptr,"提示","删除成功",QMessageBox::Ok);
        }
        else{
            QMessageBox::warning(nullptr,"警告","删除失败！",QMessageBox::Ok);
        }
    }
}

void tech_course_view::on_pbn_save_clicked()
{
    QString c_id = ui->lineEdit_course_id->text();
    QString tch_id = ui->lineEdit_tch_id->text();
    QString since = ui->lineEdit_since->text();
    QSqlQuery query(db);

    if(tch_id.length()==0){
        QMessageBox::warning(nullptr,"警告","请输入教师职工号",QMessageBox::Ok);
    }
    else if(c_id.length()==0){
        QMessageBox::warning(nullptr,"警告","请输入课程号",QMessageBox::Ok);
    }
    else{
        if(since.length()==0){
            QString select_since = QString("select since from teaching where course_id = '%1' and tch_id = '%2'").arg(c_id).arg(tch_id);
            query.exec(select_since);
            since = query.value(0).toString();
        }
        QString check = QString("select * from teaching where course_id = '%1' and tch_id = '%2'").arg(c_id).arg(tch_id);
        query.exec(check);
        if(!query.next()){
            QMessageBox::warning(nullptr,"警告","该排课记录不存在",QMessageBox::Ok);
        }
        else{
            QString update = QString("update teaching set since = '%3' where course_id = '%1' and tch_id='%2'").arg(c_id).arg(tch_id).arg(since);
            if(query.exec(update)){
                QMessageBox::information(nullptr,"提示","修改成功！",QMessageBox::Ok);
            }
            else{
                QMessageBox::warning(nullptr,"警告","修改失败！",QMessageBox::Ok);
            }
        }
    }
}


void tech_course_view::on_pbn_query_clicked()
{
    QString c_id = ui->lineEdit_q_cid->text();
    QString t_id = ui->lineEdit_q_tid->text();

    if(c_id.length()==0||t_id.length()==0){
        QMessageBox::warning(nullptr,"警告","请输入课程号和职工号",QMessageBox::Ok);
    }
    else{
        QSqlQuery query(db);
        QString select = QString("select * from teaching where course_id = '%1' and tch_id = '%2'").arg(c_id).arg(t_id);
        query.exec(select);
        if(!query.next()){
            QMessageBox::warning(nullptr,"警告","未找到排课记录",QMessageBox::Ok);
        }
        else{
            ui->lineEdit_course_id->setText(query.value(1).toString());
            ui->lineEdit_tch_id->setText(query.value(0).toString());
            ui->lineEdit_since->setText(query.value(2).toString());
        }
    }
}
