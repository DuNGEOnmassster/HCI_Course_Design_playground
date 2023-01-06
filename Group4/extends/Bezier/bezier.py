# -*- coding: utf-8 -*-
import numpy as np
from scipy.special import comb, perm
import matplotlib.pyplot as plt

plt.rcParams['font.sans-serif'] = ['SimHei']
# plt.rcParams['font.sans-serif'] = ['STXIHEI']
plt.rcParams['axes.unicode_minus'] = False

class Handwriting:
    def __init__(self, line):
        self.line = line
        self.index_02 = None  # 保存拖动的这个点的索引
        self.press = None  # 状态标识，1为按下，None为没按下
        self.pick = None  # 状态标识，1为选中点并按下,None为没选中
        self.motion = None  # 状态标识，1为进入拖动,None为不拖动
        self.xs = list()  # 保存点的x坐标
        self.ys = list()  # 保存点的y坐标
        self.cidpress = line.figure.canvas.mpl_connect('button_press_event', self.on_press)  # 鼠标按下事件
        self.cidrelease = line.figure.canvas.mpl_connect('button_release_event', self.on_release)  # 鼠标放开事件
        self.cidmotion = line.figure.canvas.mpl_connect('motion_notify_event', self.on_motion)  # 鼠标拖动事件
        self.cidpick = line.figure.canvas.mpl_connect('pick_event', self.on_picker)  # 鼠标选中事件
        self.ctl_point_1 = None

    def on_press(self, event):  # 鼠标按下调用
        if event.inaxes != self.line.axes: return
        self.press = 1

    def on_motion(self, event):  # 鼠标拖动调用
        if event.inaxes != self.line.axes: return
        if self.press is None: return
        if self.pick is None: return
        if self.motion is None:  # 整个if获取鼠标选中的点是哪个点
            self.motion = 1
            x = self.xs
            xdata = event.xdata
            ydata = event.ydata
            index_01 = 0
            for i in x:
                if abs(i - xdata) < 0.02:  # 0.02 为点的半径
                    if abs(self.ys[index_01] - ydata) < 0.02: break
                index_01 = index_01 + 1
            self.index_02 = index_01
        if self.index_02 is None: return
        self.xs[self.index_02] = event.xdata  # 鼠标的坐标覆盖选中的点的坐标
        self.ys[self.index_02] = event.ydata
        self.draw_01()

    def on_release(self, event):  # 鼠标按下调用
        if event.inaxes != self.line.axes: return
        if self.pick is None:  # 如果不是选中点，那就添加点
            self.xs.append(event.xdata)
            self.ys.append(event.ydata)
        if self.pick == 1 and self.motion != 1:  # 如果是选中点，但不是拖动点，那就降阶
            x = self.xs
            xdata = event.xdata
            ydata = event.ydata
            index_01 = 0
            for i in x:
                if abs(i - xdata) < 0.02:
                    if abs(self.ys[index_01] - ydata) < 0.02: break
                index_01 = index_01 + 1
            self.xs.pop(index_01)
            self.ys.pop(index_01)
        self.draw_01()
        self.pick = None  # 所有状态恢复，鼠标按下到稀放为一个周期
        self.motion = None
        self.press = None
        self.index_02 = None

    def on_picker(self, event):  # 选中调用
        self.pick = 1

    def draw_01(self):  # 绘图
        self.line.clear()  # 不清除的话会保留原有的图
        self.line.set_title('Bezier曲线拟合手写笔迹')
        self.line.axis([0, 1, 0, 1])  # x和y范围0到1
        # self.bezier(self.xs, self.ys)  # Bezier曲线
        self.all_curve(self.xs, self.ys)
        self.line.scatter(self.xs, self.ys, color='b', s=20, marker="o", picker=5)  # 画点
        # self.line.plot(self.xs, self.ys, color='black', lw=0.5)  # 画线
        self.line.figure.canvas.draw()  # 重构子图

    # def list_minus(self, a, b):
    #     list(map(lambda x, y: x - y, middle, begin))

    def controls(self, k, begin, end):
        if k <= 0 or k >= 1: return
        first_middle = begin + k * (end - begin)
        second_middle = begin + (1 - k) * (end - begin)
        return first_middle, second_middle


    def all_curve(self, xs, ys):
        le = len(xs)
        if le < 2: return
        self.ctl_point_1 = None

        begin = [xs[0], ys[0]]
        end = [xs[1], ys[1]]
        self.one_curve(begin, end)

        for i in range(2, le):
            begin = end
            end = [xs[i], ys[i]]
            self.one_curve(begin, end)

        end = [xs[le - 1], ys[le - 1]]
        x = [self.ctl_point_1[0], end[0]]
        y = [self.ctl_point_1[1], end[1]]

        #linestyle='dashed',
        self.line.plot(x, y,  color='yellowgreen', marker='o', lw=3)

    def one_curve(self, begin, end):
        ctl_point1 = self.ctl_point_1

        begin = np.array(begin)
        end = np.array(end)

        ctl_point2, self.ctl_point_1 = self.controls(0.4, begin, end)
        color = 'red';
        if ctl_point1 is None :
            xs = [begin[0], self.ctl_point_1[0]]
            ys = [begin[1], self.ctl_point_1[1]]
            self.line.plot(xs, ys, color=color, marker='o', linewidth='3')
        else :
            xs = [ctl_point1[0], begin[0], ctl_point2[0]]
            ys = [ctl_point1[1], begin[1], ctl_point2[1]]
            self.bezier(xs, ys)
            xs = [ctl_point2[0], self.ctl_point_1[0]]
            ys = [ctl_point2[1], self.ctl_point_1[1]]
            self.line.plot(xs, ys, color=color, marker='o', linewidth='3')

    def bezier(self, *args):  # Bezier曲线公式转换，获取x和y
        t = np.linspace(0, 1)  # t 范围0到1
        le = len(args[0]) - 1

        self.line.plot(args[0], args[1], marker='o', linestyle='dashed', color='limegreen', lw=1)
        le_1 = 0
        b_x, b_y = 0, 0
        for x in args[0]:
            b_x = b_x + x * (t ** le_1) * ((1 - t) ** le) * comb(len(args[0]) - 1, le_1)  # comb 组合，perm 排列
            le = le - 1
            le_1 = le_1 + 1

        le = len(args[0]) - 1
        le_1 = 0
        for y in args[1]:
            b_y = b_y + y * (t ** le_1) * ((1 - t) ** le) * comb(len(args[0]) - 1, le_1)
            le = le - 1
            le_1 = le_1 + 1

        color = "mediumseagreen"
        if len(args) > 2: color = args[2]
        self.line.plot(b_x, b_y, color=color, linewidth='3')

fig = plt.figure(2, figsize=(12, 6))
ax = fig.add_subplot(111)  # 一行一列第一个子图
ax.set_title('手写笔迹贝赛尔曲线, 计算控制点图解')

handwriting = Handwriting(ax)
plt.xlabel('X')
plt.ylabel('Y')

begin = np.array([20, 6])
middle = np.array([30, 40])
end = np.array([35, 4])
handwriting.one_curve(begin, middle, end)
myBezier.controls(0.2, begin, middle, end)
plt.show()