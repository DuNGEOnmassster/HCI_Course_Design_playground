#ifndef WIDGET_H
#define WIDGET_H

#include <QWidget>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QDebug>
#include <QMessageBox>

#include "home.h"
#include "function.h"
#include "admin.h"


namespace Ui {
class Widget;
}

class Widget : public QWidget
{
    Q_OBJECT

public:
    explicit Widget(QWidget *parent = nullptr);
    ~Widget();

    static QSqlDatabase database;
    static QStringList list;
    static QString sid;
    static QString tid;
    static QString aid;

private slots:
    void on_pbtn_login_clicked();

private:
    Ui::Widget *ui;
    home *hm;
    function *ft;
    admin *adm;
};

#endif // WIDGET_H
