#include "department_view.h"
#include "ui_department_view.h"

department_view::department_view(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::department_view)
{
    ui->setupUi(this);

    db = QSqlDatabase::database();

    ui->tableWidget->setColumnCount(3);
    QStringList list;
    list<<"编号"<<"院系名"<<"负责人";
    ui->tableWidget->setHorizontalHeaderLabels(list);

    QSqlQuery query(db);
    QString stat = QString("select * from department");
    query.exec(stat);
    for (int i=0;query.next();i++){
        int row_count = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(row_count+1);//添加一行

        for (int j=0;j<2;j++)
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));

    }
    QSqlQuery query1(db);
    query.exec(stat);
    for(int i=0;query.next();i++){
        QString select = QString("select tch_name from teacher where tch_id = '%1'").arg(query.value(2).toString());
        if(query.value(2).toString().size()==0){
            continue;
        }
        else{
            query1.exec(select);
            query1.next();
            qDebug()<<select;
            ui->tableWidget->setItem(i,2,new QTableWidgetItem(query1.value(0).toString()));
        }
    }

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置禁止编辑
}

department_view::~department_view()
{
    delete ui;
}
