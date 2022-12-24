#ifndef DIALOGLIST_H
#define DIALOGLIST_H

#include <QWidget>

namespace Ui {
class DialogList;
}

class DialogList : public QWidget
{
    Q_OBJECT

public:
    explicit DialogList(QWidget *parent = nullptr);
    ~DialogList();

private:
    Ui::DialogList *ui;
    QVector<bool>isShow;//代表是否打开窗口的标识，false为未打开，ture为打开
};

#endif // DIALOGLIST_H
