# -*- coding: utf-8 -*-

# Form implementation generated from reading ui file 'addContactUi.ui'
#
# Created by: PyQt5 UI code generator 5.15.7
#
# WARNING: Any manual changes made to this file will be lost when pyuic5 is
# run again.  Do not edit this file unless you know what you are doing.


from PyQt5 import QtCore, QtGui, QtWidgets


class Ui_Dialog(object):
    def setupUi(self, Dialog):
        Dialog.setObjectName("Dialog")
        Dialog.resize(400, 300)
        self.label_3 = QtWidgets.QLabel(Dialog)
        self.label_3.setGeometry(QtCore.QRect(120, 0, 221, 51))
        font = QtGui.QFont()
        font.setFamily("一纸情书")
        font.setPointSize(20)
        self.label_3.setFont(font)
        self.label_3.setObjectName("label_3")
        self.error = QtWidgets.QLabel(Dialog)
        self.error.setGeometry(QtCore.QRect(300, 70, 101, 16))
        self.error.setAutoFillBackground(False)
        self.error.setStyleSheet("color:red")
        self.error.setText("")
        self.error.setObjectName("error")
        self.time = QtWidgets.QLCDNumber(Dialog)
        self.time.setGeometry(QtCore.QRect(230, 230, 51, 61))
        self.time.setLineWidth(5)
        self.time.setDigitCount(1)
        self.time.setProperty("value", 8.0)
        self.time.setObjectName("time")
        self.tips = QtWidgets.QLabel(Dialog)
        self.tips.setGeometry(QtCore.QRect(140, 250, 61, 16))
        self.tips.setObjectName("tips")
        self.toolButton = QtWidgets.QToolButton(Dialog)
        self.toolButton.setGeometry(QtCore.QRect(360, 0, 47, 21))
        self.toolButton.setObjectName("toolButton")
        self.back = QtWidgets.QPushButton(Dialog)
        self.back.setGeometry(QtCore.QRect(10, 10, 41, 28))
        self.back.setObjectName("back")
        self.avatar_img = QtWidgets.QLabel(Dialog)
        self.avatar_img.setGeometry(QtCore.QRect(310, 130, 80, 80))
        self.avatar_img.setObjectName("avatar_img")
        self.layoutWidget = QtWidgets.QWidget(Dialog)
        self.layoutWidget.setGeometry(QtCore.QRect(71, 63, 227, 155))
        self.layoutWidget.setObjectName("layoutWidget")
        self.verticalLayout = QtWidgets.QVBoxLayout(self.layoutWidget)
        self.verticalLayout.setContentsMargins(0, 0, 0, 0)
        self.verticalLayout.setObjectName("verticalLayout")
        self.horizontalLayout_2 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_2.setObjectName("horizontalLayout_2")
        self.label_2 = QtWidgets.QLabel(self.layoutWidget)
        self.label_2.setObjectName("label_2")
        self.horizontalLayout_2.addWidget(self.label_2)
        self.username = QtWidgets.QLineEdit(self.layoutWidget)
        self.username.setObjectName("username")
        self.horizontalLayout_2.addWidget(self.username)
        self.verticalLayout.addLayout(self.horizontalLayout_2)
        self.horizontalLayout_4 = QtWidgets.QHBoxLayout()
        self.horizontalLayout_4.setObjectName("horizontalLayout_4")
        self.label_4 = QtWidgets.QLabel(self.layoutWidget)
        self.label_4.setMinimumSize(QtCore.QSize(38, 21))
        self.label_4.setMaximumSize(QtCore.QSize(38, 21))
        self.label_4.setObjectName("label_4")
        self.horizontalLayout_4.addWidget(self.label_4)
        self.nickname = QtWidgets.QLineEdit(self.layoutWidget)
        self.nickname.setMinimumSize(QtCore.QSize(0, 0))
        self.nickname.setMaximumSize(QtCore.QSize(100000, 10000))
        self.nickname.setObjectName("nickname")
        self.horizontalLayout_4.addWidget(self.nickname)
        self.verticalLayout.addLayout(self.horizontalLayout_4)
        self.search = QtWidgets.QPushButton(self.layoutWidget)
        self.search.setObjectName("search")
        self.verticalLayout.addWidget(self.search)
        self.add_contact = QtWidgets.QPushButton(self.layoutWidget)
        self.add_contact.setObjectName("add_contact")
        self.verticalLayout.addWidget(self.add_contact)

        self.retranslateUi(Dialog)
        QtCore.QMetaObject.connectSlotsByName(Dialog)

    def retranslateUi(self, Dialog):
        _translate = QtCore.QCoreApplication.translate
        Dialog.setWindowTitle(_translate("Dialog", "Dialog"))
        self.label_3.setText(_translate("Dialog", "添加好友"))
        self.tips.setText(_translate("Dialog", "即将返回"))
        self.toolButton.setText(_translate("Dialog", "..."))
        self.back.setText(_translate("Dialog", "返回"))
        self.avatar_img.setText(_translate("Dialog", "+"))
        self.label_2.setText(_translate("Dialog", "账号:"))
        self.label_4.setText(_translate("Dialog", "备注:"))
        self.search.setText(_translate("Dialog", "查找"))
        self.add_contact.setText(_translate("Dialog", "添加联系人"))
