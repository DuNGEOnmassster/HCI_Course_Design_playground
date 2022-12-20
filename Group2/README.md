# 眼动画图

## Abstract

目前对于眼动的研究主要依赖于双目眼动仪，即一个对准瞳孔的红外相机用于计算视线向量，和一个场景相机用于估计视线在场景中的观测点，这对于居家实验环境并不友好。

因此，我们采取了基于facial landmarks和eyes keypoints的单目视线估计策略，借助预训练模型对眼部与瞳孔点位进行跟踪后，将坐标关于屏幕成比例放大，实现单目视线估计。

此外，我们制作了基于PyQT的画图程序，它是通过QT Designer进行设计后人为添加图标等元素的绘图UI程序

![](./Readme_img/PaintUI.png)

我们基于单目视线估计实现了眼动操控鼠标移动和单击选择画图工具；同时为了避免眼动控制功能过于复杂，我们引入了对嘴部keypoints的检测与计算，实现嘴部开闭控制开始绘图/停止绘图。

## Setup

我们推荐您使用Anaconda创建虚拟环境

```shell script
conda create GazePainting python=3.10

conda activate GazePainting
```

如果您使用M1芯片的MacOS运行程序，可以直接通过requirement.txt配置环境

```shell script
pip install -r requirement.txt
```

否则，建议您通过Pypi或者conda下载环境依赖(以PyQT5的安装为例)

```shell script
# 👇️ in a virtual environment or using Python 2
pip install PyQt5

# 👇️ for python 3 (could also be pip3.10 depending on your version)
pip3 install PyQt5

# 👇️ if you get permissions error
sudo pip3 install PyQt5
pip install PyQt5 --user

# 👇️ if you don't have pip in your PATH environment variable
python -m pip install PyQt5

# 👇️ for python 3 (could also be pip3.10 depending on your version)
python3 -m pip install PyQt5

# 👇️ using py alias (Windows)
py -m pip install PyQt5

# 👇️ alternative for Ubuntu/Debian
sudo apt-get install python3-pyqt5

# 👇️ for Anaconda
conda install -c anaconda pyqt

# 👇️ for Jupyter Notebook
!pip install PyQt5

```

## Run

```shell script
python demo.py
```

![](./Readme_img/Tracking_basic.png)

再打开绘图窗口

```shell script
cd Paint
python paint.py
```

通过眨眼控制鼠标点击

![](./Readme_img/Tracking_click.png)

通过张嘴控制画笔落笔/抬笔

![](./Readme_img/Tracking_paint.png)