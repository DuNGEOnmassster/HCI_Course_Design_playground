#pragma once
#include <QWidget>
#include <QPushButton>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QtWidgets>
#include <QLabel>
#include <QLineEdit>

#ifndef PICTURETOOLWND_H
#define PICTURETOOLWND_H

class Picture : public QWidget
{
    Q_OBJECT
public:
    Picture(QWidget* p = nullptr);
    void Init();
    int setAngle();
    void refresh();
public slots:
    void slot_uploadBtnClicked();
    void slot_clearBtnClicked();
    void image_Right();
    void image_Left();
    void reset();
    void set_zoom();
    void set_trans();
    void confirm();
protected:
    void wheelEvent(QWheelEvent *event) override;        // 鼠标滚轮缩放图像
    void mouseMoveEvent(QMouseEvent *event) override;    // 鼠标移动
    void mousePressEvent(QMouseEvent *event) override;   // 鼠标按下
public:
    QLabel* picLable;

    QPixmap* image;
    QVBoxLayout* vLayout;
    QHBoxLayout* hLayout;
    QHBoxLayout* hLayout_;

    QPushButton* clearBtn;
    QPushButton* uploadBtn;
    QPushButton* LeftBtn;
    QPushButton* RightBtn;
    QPushButton* resetBtn;
    QPushButton* confirmBtn;

    QLineEdit* transBlk;
    QLineEdit* zoomBlk;
    QLineEdit* zoomTxt;
    QLineEdit* transTxt;
};

#endif // PICTURETOOLWND_H
