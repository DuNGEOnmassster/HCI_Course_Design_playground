#ifndef DEPARTMENT_VIEW_H
#define DEPARTMENT_VIEW_H

#include <QWidget>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class department_view;
}

class department_view : public QWidget
{
    Q_OBJECT

public:
    explicit department_view(QWidget *parent = nullptr);
    ~department_view();

private:
    Ui::department_view *ui;

    QSqlDatabase db;
};

#endif // DEPARTMENT_VIEW_H
