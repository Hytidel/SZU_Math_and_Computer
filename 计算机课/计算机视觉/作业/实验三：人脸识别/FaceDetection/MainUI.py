import json
import sys
import threading
from functools import partial

from PyQt5.QtCore import Qt, QSize, QPoint, pyqtSignal
from PyQt5.QtGui import QIcon, QColor
from PyQt5.QtWidgets import QApplication, QHBoxLayout
from qfluentwidgets import FluentIcon as FIF, SplashScreen, MSFluentWindow, TabBar, MSFluentTitleBar, MessageBox, \
    TransparentToolButton
from qfluentwidgets import (SubtitleLabel, setFont, setThemeColor)

# from gui.fragments.process import ProcessFragment
from GUI.Utils.Config import Config
from GUI.fragment.readme import ReadMeWindow

sys.path.append('./')

ICON_DIR = 'GUI/resource/images/logo.png'


class Widget(MSFluentWindow):

    def __init__(self, text: str, parent=None):
        super().__init__(parent=parent)

        self.label = SubtitleLabel(text, self)
        self.hBoxLayout = QHBoxLayout(self)

        setFont(self.label, 24)
        self.label.setAlignment(Qt.AlignCenter)
        self.hBoxLayout.addWidget(self.label, 1, Qt.AlignCenter)
        self.setObjectName(text.replace(' ', '-'))


class TitleBar(MSFluentTitleBar):
    """ Title bar with icon and title """
    onHelpButtonClicked = pyqtSignal()
    onHistoryButtonClicked = pyqtSignal()

    def __init__(self, parent):
        super().__init__(parent)

        self.toolButtonLayout = QHBoxLayout()
        self.hBoxLayout.insertLayout(4, self.toolButtonLayout)

        self.searchButton = TransparentToolButton(FIF.HELP.icon(), self)
        self.searchButton.setToolTip('帮助')
        self.searchButton.clicked.connect(self.onHelpButtonClicked)

        self.historyButton = TransparentToolButton(FIF.HISTORY.icon(), self)
        self.historyButton.setToolTip('更新日志')
        self.historyButton.clicked.connect(self.onHistoryButtonClicked)
        self.hBoxLayout.setStretch(6, 0)
        self.hBoxLayout.insertWidget(6, self.historyButton, 0, alignment=Qt.AlignRight)
        self.hBoxLayout.insertWidget(7, self.searchButton, 0, alignment=Qt.AlignRight)

class Window(MSFluentWindow):
    def __init__(self):
        super().__init__()
        self.initWindow()
        self.splashScreen = SplashScreen(self.windowIcon(), self)
        self.splashScreen.setIconSize(QSize(102, 102))
        self.setTitleBar(TitleBar(self))
        self.navi_btn_list = []
        self.show()
        setThemeColor('#0078d4')
        self.__switchStatus = True
        self.config_dir = "Config/config.json"
        self.config = Config(self.config_dir)
        from GUI.fragment.home import HomeFragment
        self.homeInterface = HomeFragment(parent=self, config=self.config)

        self.initNavigation()
        self.dispatchWindow()
        self.splashScreen.finish()

    def initNavigation(self):
        self.navi_btn_list = [
            self.addSubInterface(self.homeInterface, FIF.HOME, 'Home'),
        ]

        for ind, btn in enumerate(self.navi_btn_list):
            btn.disconnect()

        self.titleBar.onHelpButtonClicked.connect(self.showHelpModal)
        self.titleBar.onHistoryButtonClicked.connect(self.showHistoryModel)

    def initWindow(self):
        self.resize(900, 700)
        desktop = QApplication.desktop().availableGeometry()
        _w, _h = desktop.width(), desktop.height()
        self.move(_w // 2 - self.width() // 2, _h // 2 - self.height() // 2)

    def closeEvent(self, event):
        super().closeEvent(event)

    @staticmethod
    def showHelpModal():
        helpModal = ReadMeWindow()
        helpModal.setWindowTitle('帮助')
        helpModal.setWindowIcon(QIcon(ICON_DIR))
        helpModal.resize(800, 600)
        helpModal.setFocus()
        helpModal.show()

    def showHistoryModel(self):
        from GUI.fragment.history import HistoryWindow
        historyModal = HistoryWindow()
        historyModal.information = self.homeInterface.get_main_thread()._main_thread.getFaceInfo()
        historyModal.fetch_update_info()
        historyModal.setWindowTitle('Face Library')
        historyModal.setWindowIcon(QIcon(ICON_DIR))
        historyModal.resize(800, 600)
        historyModal.setFocus()
        historyModal.show()


    def onTabChanged(self, _: int):
        self.__switchStatus = False
        objectName = self.tabBar.currentTab().routeKey()
        currentName = self.stackedWidget.currentWidget().objectName()
        row = -1
        for i0, sub in enumerate(self._sub_list):
            for i1, tab in enumerate(sub):
                if tab.object_name == currentName:
                    row = i0
        assert row != -1
        col = [x.object_name for x in self._sub_list[0]].index(objectName)
        self.dispatchSubView(row, col)

    def dispatchSubView(self, i0: int, i1: int):
        self.stackedWidget.setCurrentWidget(self._sub_list[i0][i1], popOut=False)

    def dispatchWindow(self):
        self.setWindowIcon(QIcon(ICON_DIR))
        self.setWindowTitle('Face Recognition')


def start():
    QApplication.setHighDpiScaleFactorRoundingPolicy(Qt.HighDpiScaleFactorRoundingPolicy.PassThrough)
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)

    app = QApplication(sys.argv)
    w = Window()
    w.show()
    app.exec_()


if __name__ == '__main__':
    QApplication.setHighDpiScaleFactorRoundingPolicy(Qt.HighDpiScaleFactorRoundingPolicy.PassThrough)
    QApplication.setAttribute(Qt.AA_EnableHighDpiScaling)
    QApplication.setAttribute(Qt.AA_UseHighDpiPixmaps)

    app = QApplication(sys.argv)
    w = Window()
    w.setFocus(True)
    w.show()
    app.exec_()
