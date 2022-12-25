#ifndef TECH_COURSE_VIEW_H
#define TECH_COURSE_VIEW_H

#include <QWidget>
#include <QDebug>
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
class tech_course_view;
}

class tech_course_view : public QWidget
{
    Q_OBJECT

public:
    explicit tech_course_view(QWidget *parent = nullptr);
    ~tech_course_view();

private slots:
    void on_pbn_add_clicked();
    void on_pbn_clear_clicked();
    void on_pbn_del_clicked();
    void on_pbn_save_clicked();
    void on_pbn_query_clicked();

private:
    Ui::tech_course_view *ui;
    QSqlDatabase db;

};

#endif // TECH_COURSE_VIEW_H
