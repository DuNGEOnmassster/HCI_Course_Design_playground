import os
os.environ['HUB_HOME'] = "./modules"
from PyQt5 import QtCore, QtGui, QtWidgets
from PyQt5.QtCore import Qt
from PIL import Image
import io
import sys
import numpy as np
import paddlehub as hub


class Snipper(QtWidgets.QWidget):
    def __init__(self, parent=None, flags=Qt.WindowFlags()):
        super().__init__(parent=parent, flags=flags)

        self.setWindowTitle("TextShot")
        self.setWindowFlags(
            Qt.FramelessWindowHint | Qt.WindowStaysOnTopHint | Qt.Dialog
        )

        self.setWindowState(self.windowState() | Qt.WindowFullScreen)
        self.screen = QtGui.QScreen.grabWindow(
            QtWidgets.QApplication.primaryScreen(),
            QtWidgets.QApplication.desktop().winId(),
        )
        palette = QtGui.QPalette()
        palette.setBrush(self.backgroundRole(), QtGui.QBrush(self.screen))
        self.setPalette(palette)

        QtWidgets.QApplication.setOverrideCursor(QtGui.QCursor(QtCore.Qt.CrossCursor))

        self.start, self.end = QtCore.QPoint(), QtCore.QPoint()

    def keyPressEvent(self, event):
        if event.key() == Qt.Key_Escape:
            QtWidgets.QApplication.quit()

        return super().keyPressEvent(event)

    def paintEvent(self, event):
        painter = QtGui.QPainter(self)
        painter.setPen(Qt.NoPen)
        painter.setBrush(QtGui.QColor(0, 0, 0, 100))
        painter.drawRect(0, 0, self.width(), self.height())

        if self.start == self.end:
            return super().paintEvent(event)

        painter.setPen(QtGui.QPen(QtGui.QColor(255, 255, 255), 3))
        painter.setBrush(painter.background())
        painter.drawRect(QtCore.QRect(self.start, self.end))
        return super().paintEvent(event)

    def mousePressEvent(self, event):
        self.start = self.end = QtGui.QCursor.pos()
        self.update()
        return super().mousePressEvent(event)

    def mouseMoveEvent(self, event):
        self.end = QtGui.QCursor.pos()
        self.update()
        return super().mousePressEvent(event)

    def mouseReleaseEvent(self, event):
        if self.start == self.end:
            return super().mouseReleaseEvent(event)

        self.hide()
        QtWidgets.QApplication.processEvents()
        shot = self.screen.copy(QtCore.QRect(self.start, self.end))
        processImage(shot)
        QtWidgets.QApplication.quit()


def processImage(img):

    buffer = QtCore.QBuffer()
    buffer.open(QtCore.QBuffer.ReadWrite)
    img.save(buffer, "PNG")
    pil_img = Image.open(io.BytesIO(buffer.data()))
    buffer.close()

    np_images = [np.array(pil_img)]

    results = ocr.recognize_text(
        images=np_images,  # ???????????????ndarray.shape ??? [H, W, C]???BGR?????????
        use_gpu=False,  # ???????????? GPU????????????GPU???????????????CUDA_VISIBLE_DEVICES????????????
        output_dir='ocr_result',  # ???????????????????????????????????? ocr_result???
        visualization=True,  # ?????????????????????????????????????????????
        box_thresh=0.5,  # ????????????????????????????????????
        text_thresh=0.5)  # ???????????????????????????????????????

    text = []

    for result in results:
        data = result['data']
        save_path = result['save_path']
        for infomation in data:
            print('text: ', infomation['text'], '\nconfidence: ', infomation['confidence'], '\ntext_box_position: ',
                  infomation['text_box_position'])
            text.append(str(infomation['text']) + '\n')

    print(text)

    with open('data.txt', 'w') as f:
        for i in text:
            f.write(str(i))

    os.system(r'data.txt')


if __name__ == '__main__':
    # ??????????????????????????????
    # ocr = hub.Module(name="chinese_ocr_db_crnn_mobile")
    # ?????????????????????????????????????????????
    ocr = hub.Module(name="chinese_ocr_db_crnn_server")

    QtCore.QCoreApplication.setAttribute(Qt.AA_DisableHighDpiScaling)
    app = QtWidgets.QApplication(sys.argv)
    window = QtWidgets.QMainWindow()
    snipper = Snipper(window)
    snipper.show()
    sys.exit(app.exec_())

