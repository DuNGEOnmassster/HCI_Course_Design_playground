#include "dialoglist.h"
#include "widget.h"

#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

//    Widget w;
//    w.show();

    DialogList list;
    list.show();

    return a.exec();
}
