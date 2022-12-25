#include "tchwork_view.h"
#include "ui_tchwork_view.h"

tchWork_view::tchWork_view(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::tchWork_view)
{
    ui->setupUi(this);

    //设置选中tabWidget的一行时，tabWidget_2显示具体课程信息
    connect(ui->tableWidget,&QTableWidget::cellClicked,[=](int row){
        ui->tableWidget_2->setRowCount(0);

        ui->tableWidget_2->setColumnCount(6);
        QStringList list;
        list<<"课程代号"<<"课程名"<<"课时数"<<"课程类型"<<"学分"<<"开课时间";
        ui->tableWidget_2->setHorizontalHeaderLabels(list);
        ui->tableWidget_2->setSelectionBehavior(QAbstractItemView::SelectRows);

        QString tch_id = ui->tableWidget->item(row,0)->text();
        QSqlQuery query(db);
        QString stat = QString("select C.course_id,C.course_name,C.course_hour,C.course_type,C.course_credit,T.since "
                               "from teaching T,course C "
                               "where T.tch_id='%1' AND T.course_id=C.course_id").arg(tch_id);
        query.exec(stat);
        for (int i=0;query.next();i++) {
            int row_count = ui->tableWidget_2->rowCount();
            ui->tableWidget_2->setRowCount(row_count+1);

            for (int j=0;j<6;j++)
                ui->tableWidget_2->setItem(i,j,new QTableWidgetItem(query.value(j).toString()));
        }
    });

    db = QSqlDatabase::database();

    //设置tabWidget信息
    ui->tableWidget->setColumnCount(3);
    QStringList list;
    list<<"工号"<<"姓名"<<"总工作量";
    ui->tableWidget->setHorizontalHeaderLabels(list);
    ui->tableWidget->setSelectionBehavior(QAbstractItemView::SelectRows);//设置选中模式为选中行

    QSqlQuery query1(db);
    QSqlQuery query2(db);

    //获取信息，填入tabWidget
    QString stat = QString("select tch_id, tch_name from teacher");
    query1.exec(stat);
    for (int i=0;query1.next();i++){
        int row_count = ui->tableWidget->rowCount();
        ui->tableWidget->setRowCount(row_count+1);//添加一行

        for (int j=0;j<2;j++)
            ui->tableWidget->setItem(i,j,new QTableWidgetItem(query1.value(j).toString()));
        stat = QString("select C.course_hour "
                       "from teaching T,course C "
                       "where T.tch_id='%1' AND T.course_id=C.course_id").arg(query1.value(0).toString());
        query2.exec(stat);

        int work = 0;
        for (int k=0;query2.next();k++)
            work += query2.value(0).toInt();
        ui->tableWidget->setItem(i,2,new QTableWidgetItem(QString("%1").arg(work)));
    }

    ui->tableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);//设置禁止编辑
}

tchWork_view::~tchWork_view()
{
    delete ui;
}

void tchWork_view::on_btn_clicked()
{
    int rc = ui->tableWidget->rowCount();//获得行数
    QString text = ui->lineEdit->text();
    if (text == "")
        for (int i = 0; i < rc; i++)
            ui->tableWidget->setRowHidden(i, false);//显示所有行
    else {
        QList <QTableWidgetItem *> item = ui->tableWidget->findItems(text, Qt::MatchContains);

        for (int i = 0; i < rc; i++)
            ui->tableWidget->setRowHidden(i, true);//隐藏所有行

        if (!item.isEmpty())//不为空
            for (int i=0;i<item.count(); i++)
                ui->tableWidget->setRowHidden(item.at(i)->row(),false);
        }
}
