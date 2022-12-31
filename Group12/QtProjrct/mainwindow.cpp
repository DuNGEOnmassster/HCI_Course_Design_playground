#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QMainWindow>
#include <QAction>
#include <QtGui>
#include <QFileDialog>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    Menu_File();        // 文件菜单
    InitImage();        // 初始化图像QLabel
    Widge_set();        // 设置窗口布局

    setWindowTitle("ImageView created by CXM");                                            // 标题名称
    setWindowIcon(QIcon(":/image.png"));                                                   // 软件图标(图标来源于网络)
    resize(QApplication::desktop()->width()*0.8, QApplication::desktop()->height()*0.8);   // 界面大小
    move(QApplication::desktop()->width()*0.05, QApplication::desktop()->height()*0.01);   // 界面在屏幕的位置
}

/* 窗口布局 */
void MainWindow::Widge_set()
{
    /* 设置图像窗口属性 */
    dock_Image->setFeatures(QDockWidget::DockWidgetMovable | QDockWidget::DockWidgetFloatable);   // 设置为可移动可浮动，但不可关闭
    dock_Image->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);                // 可移动范围：左右
    dock_Image->setMinimumSize(600,600);                      // 设置最小宽高

    /* 为其他窗口开辟堆内存 */
    dock_Tool = new QDockWidget(tr("工具箱"), this);            // 工具箱窗口,若想设置特征或移动范围，方法同上。
    dock_Geom = new QDockWidget(tr("几何变换"), this);          // 几何变换窗口
    dock_Gray = new QDockWidget(tr("灰度变换"), this);          // 灰度变换窗口
    dock_Enhance = new QDockWidget(tr("图像增强"), this);       // 图像增强窗口
    dock_Morp = new QDockWidget(tr("形态学处理"), this);        // 形态学处理窗口
    dock_Color = new QDockWidget(tr("颜色模型"), this);         // 颜色模型窗口
    dock_Prop = new QDockWidget(tr("属性"), this);             // 属性窗口
    dock_Output = new QDockWidget(tr("输出"), this);           // 输出窗口

    /* 添加窗口 */
    setCentralWidget(dock_Image);                             // 指定图像窗口为中心窗口
    addDockWidget(Qt::LeftDockWidgetArea, dock_Tool);         // 默认将工具箱添加至左侧
    addDockWidget(Qt::BottomDockWidgetArea, dock_Output);     // 默认将输出窗口添加至底部
    addDockWidget(Qt::RightDockWidgetArea, dock_Geom);        // 默认将几何变换窗口添加至右侧
    addDockWidget(Qt::RightDockWidgetArea, dock_Gray);        // 默认将灰度变换窗口添加至右侧
    addDockWidget(Qt::RightDockWidgetArea, dock_Enhance);     // 默认将图像增强窗口添加至右侧
    addDockWidget(Qt::RightDockWidgetArea, dock_Morp);        // 默认将形态学窗口添加至右侧
    addDockWidget(Qt::RightDockWidgetArea, dock_Color);       // 默认将颜色模型窗口添加至右侧
    addDockWidget(Qt::RightDockWidgetArea, dock_Prop);        // 默认将属性窗口添加至右侧

    /* 分割窗口 */
    splitDockWidget(dock_Tool, dock_Image, Qt::Horizontal);   // 水平拆分：将图像窗口置于工具箱右侧
    splitDockWidget(dock_Geom, dock_Output, Qt::Vertical);    // 垂直拆分：将合并的输出窗口置于几何窗口下部

    /* 合并窗口 */
    tabifyDockWidget(dock_Geom, dock_Gray);                   // 将几何变换、灰度变换、图像增强、
    tabifyDockWidget(dock_Gray, dock_Enhance);                // 形态学处理、颜色模型五个窗口合并
    tabifyDockWidget(dock_Enhance, dock_Morp);
    tabifyDockWidget(dock_Morp, dock_Color);
    tabifyDockWidget(dock_Output, dock_Prop);                 // 将输出窗口和属性窗口合并

    dock_Geom->raise();                                       // raise()函数可使指定窗口置于最前
}

/* 铺满窗口 */
void MainWindow::fullSize()
{
    QImage* img = new QImage();
    img->load(currentPath);
    imgLabel->setPixmap(QPixmap::fromImage(*img));
    imgLabel->resize(dock_Image->width() - 2,575);
    qDebug() << "铺满窗口成功";
}

/* 自适应窗口 */
void MainWindow::autoSize()
{
    QImage Img = QImage(595, 575, QImage::Format_RGB32);
    double ImgRatio = 1.0 * imgLabel->pixmap()->toImage().width() / imgLabel->pixmap()->toImage().height();     // 图像宽高比
    double WinRatio = 1.0 * (dock_Image->width() - 2) / (dock_Image->height() - 2); // 窗口宽高比
    if (ImgRatio > WinRatio)        // 图像宽高比大于窗口宽高比
    {
        Img = imgLabel->pixmap()->toImage().scaled((dock_Image->width() - 2), (dock_Image->width() - 2) / ImgRatio, Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    else                            // 图像宽高比小于等于图像宽高比
    {
        Img = imgLabel->pixmap()->toImage().scaled((dock_Image->height() - 2) * ImgRatio, (dock_Image->height() - 25), Qt::IgnoreAspectRatio, Qt::SmoothTransformation);
    }
    imgLabel->setPixmap(QPixmap::fromImage(Img));   // 显示图像
    imgLabel->resize(Img.width(), Img.height());
    qDebug() << "自适应窗口成功";
}

/* 图片左转 */
void MainWindow::img_Left(void)
{
    QMatrix leftmatrix;
    leftmatrix.rotate(-90);                                // 左转为负
    QImage img = imgLabel->pixmap()->toImage();            // 读取图像
    imgLabel->setPixmap(QPixmap::fromImage(img).transformed(leftmatrix,Qt::SmoothTransformation));
    qDebug() << "图像向左旋转90°";
}

/* 图片右转 */
void MainWindow::img_Right(void)
{
    QMatrix leftmatrix;
    leftmatrix.rotate(+90);                                // 右转为正
    QImage img = imgLabel->pixmap()->toImage();            // 读取图像
    imgLabel->setPixmap(QPixmap::fromImage(img).transformed(leftmatrix,Qt::SmoothTransformation));
    qDebug() << "图像向右旋转90°";
}

/* 文件菜单 */
void MainWindow::Menu_File()
{

    QMenu *file = menuBar()->addMenu(tr("文件"));  // 菜单栏

    /* 菜单动作 */
    QAction *Act_file_new = new QAction(QIcon(":/add.png"), tr("新建"), this);
    Act_file_new->setShortcuts(QKeySequence::New);                                        // 快捷键 Ctrl+N
    connect(Act_file_new, &QAction::triggered,this, &MainWindow::File_new);

    QAction *Act_file_open = new QAction(QIcon(":/document-open.png"), tr("打开"), this);
    Act_file_open->setShortcuts(QKeySequence::Open);                                       // 快捷键 Ctrl+O
    connect(Act_file_open, &QAction::triggered, this, &MainWindow::File_open);

    QAction *Act_file_save = new QAction(QIcon(":/document-save.png"), tr("保存"), this);
    Act_file_save->setShortcuts(QKeySequence::Save);                                       // 快捷键 Ctrl+S
    connect(Act_file_save, &QAction::triggered, this, &MainWindow::File_save);

    QAction *Act_file_saveas = new QAction(QIcon(":/document-save-as.png"), tr("另存为"), this);
    Act_file_saveas->setShortcuts(QKeySequence::SaveAs);                                   // 快捷键
    connect(Act_file_saveas, &QAction::triggered, this, &MainWindow::File_saveas);

    QAction *Act_file_close = new QAction(QIcon(":/gtk-cancel.png"), tr("关闭"), this);
    Act_file_close->setShortcuts(QKeySequence::Close);                                     // 快捷键 Ctrl+F4
    connect(Act_file_close, &QAction::triggered, this, &MainWindow::close);

    QPushButton *button_full = new QPushButton(tr("铺满窗口"));                     // 创建按钮
    connect(button_full, &QPushButton::clicked, this, &MainWindow::fullSize);     // 信号与槽

    QPushButton *button_auto = new QPushButton(tr("自适应窗口"));                   // 创建按钮
    connect(button_auto, &QPushButton::clicked, this, &MainWindow::autoSize);     // 信号与槽

    QAction *button_left = new QAction(QIcon(":/back.png"),tr("向左旋转"),this);   // 创建控件
    connect(button_left, &QAction::triggered, this, &MainWindow::img_Left);       // 信号与槽

    QAction *button_right = new QAction(QIcon(":/forward"),tr("向右旋转"),this);   // 创建控件
    connect(button_right, &QAction::triggered, this, &MainWindow::img_Right);     // 信号与槽

    /* 将动作添加到菜单上 */
    file->addAction(Act_file_new);
    file->addAction(Act_file_open);
    file->addAction(Act_file_save);
    file->addAction(Act_file_saveas);
    file->addSeparator();                       // 添加分割线
    file->addAction(Act_file_close);

    /* 工具栏 */
//    ui->mainToolBar->addAction(Act_file_new);
//    ui->MainWindow::mainToolBar->addAction(Act_file_open);
//    ui->MainWindow::mainToolBar->addAction(Act_file_save);
//    ui->MainWindow::mainToolBar->addWidget(button_full);
//    ui->MainWindow::mainToolBar->addWidget(button_auto);
//    ui->MainWindow::mainToolBar->addAction(button_left);
//    ui->MainWindow::mainToolBar->addAction(button_right);

    /* 任务栏 */
    Act_file_new->setStatusTip(tr("新建图像"));
    Act_file_open->setStatusTip(tr("打开图像"));
    Act_file_save->setStatusTip(tr("保存图像"));
    Act_file_saveas->setStatusTip(tr("图像另存为"));
    Act_file_close->setStatusTip(tr("关闭软件"));
}

/* 初始化图像Label */
void MainWindow::InitImage()
{
        dock_Image = new QDockWidget(tr("图像"), this);
        setCentralWidget(dock_Image);                           // 设置中心控件

        /* 初始化QLabel */
        imgLabel = new QLabel(dock_Image);
        imgLabel->setScaledContents(true);                      // 设置QLabel自动适应图像大小

        /* 初始化图像 */
        QImage image = QImage(595, 575, QImage::Format_RGB32);  // 新建图像
        image.fill(qRgb(255, 255, 255));                        // 全白
        imgLabel->setPixmap(QPixmap::fromImage(image));         // 显示图像
        imgLabel->resize(image.width(), image.height());        // 图像与imgLabel同大小

        // 增加滚动条,如果图像比imgLabel大，就会出现滚动条
        QScrollArea* scrollArea = new QScrollArea(this);
        scrollArea->setBackgroundRole(QPalette::Dark);
        scrollArea->setAlignment(Qt::AlignCenter);
        scrollArea->setWidget(imgLabel);
        dock_Image->setWidget(scrollArea);

}

/* 新建图像 */
void MainWindow::File_new()
{
    QImage image = QImage(500, 500, QImage::Format_RGB32);  // 创建长宽各500的RGB图像
    image.fill(qRgb(255, 255, 255));                        // 白色图像
    imgLabel->setPixmap(QPixmap::fromImage(image));         // 转载图像
    imgLabel->resize(image.width(), image.height());        // imgLabel与图像同大小
    currentPath = "";                                       // 当前路径为空
    qDebug() << "新建成功";                                  // 测试语句
}

/* 打开文件 */
void MainWindow::File_open()
{
    /* 文件选择框 */
    QString path = QFileDialog::getOpenFileName
            (this,"打开文件","C:\\Users\\ming\\Desktop");
    if (!path.isEmpty())                                    // 判断当前路径是否正确
    {
        QImage* img = new QImage();
        if (!(img->load(path)))
        {
            QMessageBox::information(this, tr("错误"), tr("打开图像失败！"));
            delete img;
            return;
        }
        qDebug() << "打开图片成功";                           // 仅用于检测是否进入if判断
        imgLabel->setPixmap(QPixmap::fromImage(*img));
        imgLabel->resize(img->size());
        currentPath = path;
    }
}

/* 保存文件 */
void MainWindow::File_save()
{
    if (currentPath.isEmpty())                             // 判断是新建的图像还是打开的图像
    {
        QString path = QFileDialog::getSaveFileName(this, tr("保存图像"), ".", tr("Images(*.jpg *.png *.bmp)"));
        {
            if (!path.isEmpty())
                currentPath = path;
        }
    }
    qDebug() << "保存图片成功";                              // 仅用于检测是否进入if判断
    QImage img = imgLabel->pixmap()->toImage();            // 读取图像
    img.save(currentPath);                                 // 保存图像
}

/* 另存为 */
void MainWindow::File_saveas()
{
    QString filename1 = QFileDialog::getSaveFileName
            (this,tr("Save Image"),"",tr("Images (*.png *.bmp *.jpg)"));    //选择路径
    QScreen *screen = QGuiApplication::primaryScreen();
    screen->grabWindow(imgLabel->winId()).save(filename1);
    qDebug() << "另存图片成功";
}

