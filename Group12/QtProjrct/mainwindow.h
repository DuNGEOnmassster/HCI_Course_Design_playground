//.h
#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#pragma once
#include <QtWidgets/QMainWindow>
#include <QtGui>
#include <QtWidgets>
#include <QMainWindow>

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);

    ~MainWindow();

private:
    Ui::MainWindow *ui;

    QDockWidget *dock_Image;                    // 图像窗口
    QString currentPath;                        // 当前图像的路径
    QLabel *imgLabel;                           // 图像显示框

    void InitImage();                           // 初始化图像
    void Menu_File();                           // 文件菜单
    void Widge_set();                           // 设置界面
    void fullSize();                            // 图像铺满屏幕
    void autoSize();                            // 图像自适应窗口
    void img_Left(void);                        // 图片向左旋转
    void img_Right(void);                       // 图片向右旋转

    /* 文件菜单中包含的选项 */
    void File_new();                            // 新建
    void File_open();                           // 打开
    void File_save();                           // 保存
    void File_saveas();                         // 另存为
                                                // 关闭不需要，直接使用close()
    /* 界面包含的窗口 */
    QHBoxLayout* mainToolBar;
    QDockWidget* dock_Tool;                     // 工具箱窗口
    QDockWidget* dock_Geom;                     // 几何变换窗口
    QDockWidget* dock_Gray;                     // 灰度变换窗口
    QDockWidget* dock_Enhance;                  // 图像增强窗口
    QDockWidget* dock_Morp;                     // 形态学处理窗口
    QDockWidget* dock_Color;                    // 颜色模型窗口
    QDockWidget* dock_Prop;                     // 属性窗口
    QDockWidget* dock_Output;                   // 输出窗口
};

#endif // MAINWINDOW_H
