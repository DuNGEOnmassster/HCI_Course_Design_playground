#include "Picture.h"
#include <QMouseEvent>
#include <QDebug>
#include <QPixmap>
QString filepath;

Picture::Picture(QWidget* p) : QWidget(p)
{
    Init();
}

double zoom;      // 缩放比例
double zoom_init; // 初始缩放比例
int trans;        // 旋转角度
QPoint pressPoint, corePoint, currentPoint;
int oldAngle;
QString transText = QString("%1").arg(trans);
QString zoomText  = QString("%1").arg(zoom*100);

void Picture::slot_uploadBtnClicked()
{
    filepath = QFileDialog::getOpenFileName(nullptr, QStringLiteral("选择图片"), ".", "*.png;*.jpg");
    if (filepath.trimmed().isEmpty() == false)
    {
        QPixmap picture;
        picture.load(filepath);

        // 按照比例缩放
        double ratio = 1.0;
        double w, h;
        w = 1.0 * picLable->width()/picture.width();
        h = 1.0 * picLable->height()/picture.height();
        ratio = w < h ? w : h;
        ratio = ((int)(ratio*100))/100.0;

        image = new QPixmap(picture);

        picture = picture.scaled(picture.width()*ratio, picture.height()*ratio);
        picLable->setPixmap(picture);
        picLable->setAlignment(Qt::AlignCenter);

        corePoint = picLable->rect().center();
        zoom = ratio;
        zoom_init = ratio;
        trans = 0;
        refresh();
    }
}

void Picture::slot_clearBtnClicked()
{
    QPixmap picture;
    picLable->setPixmap(picture);
    image = &picture;
    zoom = 1.0;
    trans = 0;
    transBlk->setText("");
    zoomBlk->setText("");
//    refresh();
}


int Picture::setAngle(){
    QLineF lineBegin(corePoint, pressPoint);
    QLineF lineEnd(corePoint, currentPoint);
    int mouseAngle = 360 - lineBegin.angleTo(lineEnd);
    return mouseAngle;
}
void Picture::mousePressEvent(QMouseEvent *event){
    pressPoint = event->pos();
    oldAngle = trans;
}
void Picture::mouseMoveEvent(QMouseEvent *event){
    currentPoint = event->pos();
    if(oldAngle >= 360){
        oldAngle = oldAngle % 360;
    }
    trans = oldAngle + setAngle(); //setAngle()返回mouseAngle
    if(trans >= 360){
        trans = trans % 360;
    }
    // 图像旋转
    QMatrix rotatematrix;
    rotatematrix.rotate(trans);
    QPixmap img = *image;
    img = img.scaled(img.width() * zoom, img.height() * zoom);
    picLable->setPixmap(img.transformed(rotatematrix, Qt::SmoothTransformation));

    refresh();
}


/* 图片左转 */
void Picture::image_Left()
{
    trans -= 90;
    if(trans <= -360)
        trans += 360;
    QMatrix leftmatrix;
    leftmatrix.rotate(trans);
    QPixmap img = *image;
    img = img.scaled(img.width() * zoom, img.height() * zoom);
    picLable->setPixmap(img.transformed(leftmatrix,Qt::SmoothTransformation));
    qDebug() << "图像向左旋转90°";

    refresh();
}

/* 图片右转 */
void Picture::image_Right()
{
    trans += 90;
    if(trans >= 360)
        trans -= 360;
    QMatrix leftmatrix;
    leftmatrix.rotate(trans);
    QPixmap img = *image;
    img = img.scaled(img.width() * zoom, img.height() * zoom);
    picLable->setPixmap(img.transformed(leftmatrix,Qt::SmoothTransformation));
    qDebug() << "图像向右旋转90°";

    refresh();
}

void Picture::wheelEvent(QWheelEvent *event)
{
    QPoint numDegrees;                             // numDegrees用于获取滚轮转角
    numDegrees = event->angleDelta();              // 获取滚轮转角
    int step = 0;
    if (!numDegrees.isNull() && image != NULL)     // 判断滚轮是否转动
        step = numDegrees.y();                     // 将滚轮转动数值传给step
    else
        return ;

    event->accept();                               // 获取事件
    QPixmap picture = *image;
    int currentWidth = picture.width();
    int currentHeight = picture.height();
    step = step > 0? 3: -3;
    zoom += step * 0.01;
    currentWidth = (int)(zoom * currentWidth);
    currentHeight = (int)(zoom * currentHeight);
    if(zoom <= 0)
    {
        zoom = 0.01;
        return ;
    }

//    QImage img = picLable->pixmap()->toImage();
//    img = img.scaled(currentWidth, currentHeight);
//    picture = QPixmap::fromImage(img);
//    picLable->setPixmap(picture);

    picture = picture.scaled(currentWidth, currentHeight);
    QMatrix matrix;
    matrix.rotate(trans);
    picLable->setPixmap(picture.transformed(matrix, Qt::SmoothTransformation));
    picLable->setAlignment(Qt::AlignCenter);

    refresh();
}

void Picture::reset()
{
    zoom = zoom_init;
    trans = 0;
    QPixmap img = *image;
    QMatrix matrix;
    matrix.rotate(trans);
    img = img.scaled(img.width()*zoom, img.height()*zoom);
    picLable->setPixmap(img.transformed(matrix, Qt::SmoothTransformation));
    picLable->setAlignment(Qt::AlignCenter);

    refresh();
}

void Picture::set_trans()
{
    QString s = transBlk->text();
    int tmp = s.toInt();
    qDebug() << " trans: " << tmp;
    trans = tmp % 360;
}

void Picture::set_zoom()
{
    QString s = zoomBlk->text();
    double tmp = s.toInt()/100.0;
    qDebug() << " zoom: " << tmp;
    if(tmp > 0)
        zoom = tmp;
}

void Picture::confirm()
{
    set_zoom();
    set_trans();
    refresh();
    // 显示图像
    QMatrix matrix;
    matrix.rotate(trans);
    QPixmap img = *image;
    img = img.scaled(img.width() * zoom, img.height() * zoom);
    picLable->setPixmap(img.transformed(matrix, Qt::SmoothTransformation));
}

void Picture::refresh()
{
    transText = QString("%1").arg(trans);
    zoomText  = QString("%1").arg(zoom*100);
    transBlk->setText(transText);
    zoomBlk->setText(zoomText);
}

void Picture::Init()
{
    // 初始化界面
    vLayout = new QVBoxLayout();
    setLayout(vLayout);
    picLable = new QLabel();
    picLable->setFixedSize(1500, 1000);

    // 图片状态显示
    transBlk = new QLineEdit();
    zoomBlk  = new QLineEdit();
    confirmBtn = new QPushButton("确认");
    zoomTxt = new QLineEdit();
    transTxt = new QLineEdit();

//    transBlk->setText(transText);
//    zoomBlk->setText(zoomText);
    zoomTxt->setText("缩放（%）");
    transTxt->setText("旋转角度（°）");
    zoomTxt->setReadOnly(true);
    transTxt->setReadOnly(true);
    zoomTxt->setFixedSize(150,40);
    transTxt->setFixedSize(150,40);
    zoomBlk->setFixedSize(150,40);
    transBlk->setFixedSize(150,40);
    confirmBtn->setFixedSize(100,40);
    zoomBlk->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));
    transBlk->setValidator(new QRegExpValidator(QRegExp("[0-9]+$")));

    hLayout_ = new QHBoxLayout();
    hLayout_->addStretch();
    hLayout_->addWidget(transTxt);
    hLayout_->addWidget(transBlk);
    hLayout_->addWidget(zoomTxt);
    hLayout_->addWidget(zoomBlk);
    hLayout_->addWidget(confirmBtn);


    // 绑定按键
    clearBtn = new QPushButton("清空图片");
    uploadBtn = new QPushButton("加载图片");
    LeftBtn = new QPushButton("左旋90°");
    RightBtn = new QPushButton("右旋90°");
    resetBtn = new QPushButton("图片重置");

    hLayout = new QHBoxLayout();
    hLayout->addWidget(uploadBtn);
    hLayout->addWidget(clearBtn);
    hLayout->addWidget(LeftBtn);
    hLayout->addWidget(RightBtn);
    hLayout->addWidget(resetBtn);

    vLayout->addLayout(hLayout_);
    vLayout->addWidget(picLable);
    vLayout->addLayout(hLayout);
    // 按键功能
    connect(uploadBtn, SIGNAL(clicked()), this, SLOT(slot_uploadBtnClicked()));
    connect(clearBtn , SIGNAL(clicked()), this, SLOT(slot_clearBtnClicked()));
    connect(LeftBtn  , SIGNAL(clicked()), this, SLOT(image_Left()));
    connect(RightBtn , SIGNAL(clicked()), this, SLOT(image_Right()));
    connect(resetBtn , SIGNAL(clicked()), this, SLOT(reset()));
    connect(confirmBtn, SIGNAL(clicked()), this, SLOT(confirm()));
}
