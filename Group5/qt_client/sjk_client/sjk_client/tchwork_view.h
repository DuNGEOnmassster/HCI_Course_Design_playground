#ifndef TCHWORK_VIEW_H
#define TCHWORK_VIEW_H

#include <QWidget>
#include <QDebug>
#include <QSqlDatabase>
#include <QSqlQuery>

namespace Ui {
class tchWork_view;
}

class tchWork_view : public QWidget
{
    Q_OBJECT

public:
    explicit tchWork_view(QWidget *parent = nullptr);
    ~tchWork_view();

private slots:
    void on_btn_clicked();

private:
    Ui::tchWork_view *ui;

    QSqlDatabase db;
};

#endif // TCHWORK_VIEW_H
