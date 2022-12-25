#ifndef HOME_H
#define HOME_H

#include <QDialog>
#include <QMessageBox>
#include <QStandardItemModel>

namespace Ui {
class home;
}

class home : public QDialog
{
    Q_OBJECT

public:
    explicit home(QWidget *parent = nullptr);
    ~home();

    int currentRow;

    QString f_stat;

    void init_checkin();

    void init_score();

    void init_selection();

    void display(int row,QStringList lists);

private slots:
    void on_pbtn_subch_clicked();

    void on_pbtn_ok_clicked();

    void on_pushButton_clicked();

private:
    Ui::home *ui;

    QStandardItemModel *model;


};

#endif // HOME_H
