#ifndef ADMIN_H
#define ADMIN_H

#include "department_view.h"
#include "tchwork_view.h"
#include "tech_course_view.h"

#include <QDebug>
#include <QWidget>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlTableModel>
#include <QSqlRecord>
#include <QItemSelectionModel>
#include <QPushButton>
#include <QButtonGroup>
#include <QMessageBox>

namespace Ui {
class admin;
}

class admin : public QWidget
{
    Q_OBJECT

public:
    explicit admin(QWidget *parent = nullptr);
    ~admin();

private slots:
    void on_btn_course_add_clicked();
    void on_btn_course_ok_clicked();
    void on_btn_course_cancel_clicked();
    void on_btn_course_del_clicked();
    void on_btn_course_search_clicked();

    void on_btn_dept_query_clicked();
    void on_btn_dept_add_clicked();
    void on_btn_dept_del_clicked();
    void on_btn_dept_clear_clicked();
    void on_btn_dept_save_clicked();

    void on_btn_tch_add_clicked();
    void on_btn_tch_del_clicked();
    void on_btn_tch_ok_clicked();
    void on_btn_tch_cancel_clicked();
    void on_btn_tch_search_clicked();

    void on_btn_stu_add_clicked();
    void on_btn_stu_del_clicked();
    void on_btn_stu_ok_clicked();
    void on_btn_stu_cancel_clicked();
    void on_btn_stu_search_clicked();

    void on_btn_dept_view_clicked();

    void on_btn_tch_work_clicked();

    void on_btn_stu_3_1_clicked();

    void on_btn_stu_3_2_clicked();

    void on_btn_stu_score_add_clicked();

    void on_btn_stu_score_del_clicked();

    void on_btn_stu_score_ok_clicked();

    void on_btn_stu_score_cancel_clicked();

    void on_btn_stu_score_search_clicked();

    void on_btn_stu_3_3_clicked();

    void on_btn_tch_course_clicked();

private:
    Ui::admin *ui;

    tchWork_view *tch_view;
    department_view *dept_view;
    tech_course_view *tch_course_view;
    void connectMysql();

    QSqlDatabase db;
    QSqlTableModel *model_stu;
    QSqlTableModel *model_stu_score;
    QSqlTableModel *model_tch;
    QSqlTableModel *model_course;
};

#endif // ADMIN_H
