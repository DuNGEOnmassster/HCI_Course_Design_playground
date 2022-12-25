#ifndef FUNCTION_H
#define FUNCTION_H

#include <QDialog>
#include <QTabWidget>
#include <QMessageBox>
#include <QDebug>
#include <QStringList>
#include <QStandardItemModel>
#include <QStandardItem>
#include <QSqlTableModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QSqlDatabase>
#include <QSqlError>
#include <QSqlQueryModel>

namespace Ui {
class function;
}

class function : public QDialog
{
    Q_OBJECT

public:
    explicit function(QWidget *parent = nullptr);
    ~function();

private slots:

    void on_inquiry_clicked();

    void on_show_all_clicked();

    void on_tongji_clicked();

    void on_sure_clicked();

    void on_pushButton_clicked();

private:
    //QSqlDatabase db;
    QString t_id;
    Ui::function *ui;
    QSqlTableModel *model3;
    QString course_id,course_name,course_hours,since;
    QString stu_name,stu_id,stu_course,score;
};

#endif // FUNCTION_H
