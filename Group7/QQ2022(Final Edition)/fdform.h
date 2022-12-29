#ifndef FDFORM_H
#define FDFORM_H

#include <QWidget>

namespace Ui {
class fdform;
}

class fdform : public QWidget
{
    Q_OBJECT

public:
    explicit fdform(QWidget *parent, QString name);
    ~fdform();

signals:
   //关闭窗口发送关闭信息
   void closeFdform();
protected:
   //关闭事件
  void closeEvent(QCloseEvent *);

private:
    Ui::fdform *ui;
    QVector<bool>isShow;
    QString myName;
};

#endif // FDFORM_H
