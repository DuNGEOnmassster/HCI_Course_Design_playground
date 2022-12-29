#ifndef LOGIN_H
#define LOGIN_H

#include <QWidget>

namespace Ui {
class login;
}

class login : public QWidget
{
    Q_OBJECT

public:
    explicit login(QWidget *parent = nullptr);
    ~login();

private slots:
    void on_btnMin_clicked();

    void on_btnClose_clicked();

    void on_btnLogin_released();

private:
    Ui::login *ui;
    QVector<bool>isShow;
};

#endif // LOGIN_H
