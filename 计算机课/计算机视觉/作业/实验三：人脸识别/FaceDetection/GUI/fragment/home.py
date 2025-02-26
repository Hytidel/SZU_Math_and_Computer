import json
import time
from hashlib import md5
from json import JSONDecodeError
from random import random

import cv2
from PyQt5.QtCore import Qt, QThread, pyqtSignal, QTimer
from PyQt5.QtGui import QPixmap, QImage
from PyQt5.QtWidgets import QFrame, QVBoxLayout, QLabel, QHBoxLayout
from qfluentwidgets import FluentIcon as FIF, TextEdit
from qfluentwidgets import (
    PrimaryPushSettingCard,
    SubtitleLabel,
    setFont,
    PushButton
)

from FaceDetection import FaceDetection

from MainUI import Window

import threading


class MyQLabel(QLabel):
    button_clicked_signal = pyqtSignal()

    def __init__(self, parent=None):
        super(MyQLabel, self).__init__(parent)

    def mouseReleaseEvent(self, QMouseEvent):
        self.button_clicked_signal.emit()

    def connect_customized_slot(self, func):
        self.button_clicked_signal.connect(func)


class HomeFragment(QFrame):
    def __init__(self, parent: Window = None, config=None):
        super().__init__(parent=parent)
        self.once = True
        self.event_map = {}
        self.config = config
        self.log_entries = None
        self.crt_line_index = -1
        self.expandLayout = QVBoxLayout(self)
        self.vBoxLayout = QVBoxLayout(self)

        self.info_box = QFrame(self)
        self.info_box.setFixedHeight(45)
        self.infoLayout = QHBoxLayout(self.info_box)

        title = f'Face Recognition'
        self.label = SubtitleLabel(title, self)
        self.info = SubtitleLabel('No Task', self)
        setFont(self.label, 18)
        setFont(self.info, 24)

        self.infoLayout.addWidget(self.label, 0, Qt.AlignLeft)
        self.infoLayout.addStretch(1)
        self.infoLayout.addWidget(self.info, 0, Qt.AlignRight)

        self.startup_card = PrimaryPushSettingCard(
            self.tr('START'),
            FIF.CARE_RIGHT_SOLID,
            self.tr('Start'),
            'Start Face Recognition',
            self
        )

        self.bottomLayout = QHBoxLayout()
        self.label_update = QLabel(self)
        self.column_2 = QVBoxLayout(self)
        self.label_logger = QLabel(self)
        self.logger_box = TextEdit(self)
        self.logger_box.setReadOnly(True)

        self.addFaceLayout = QHBoxLayout(self)
        self.addFaceButton = PushButton('Add', self)
        self.inputName = TextEdit(self)
        self.inputName.setText('Enter Name Here')
        self.inputName.setFixedWidth(200)
        self.inputName.setFixedHeight(40)
        self.addFaceLayout.addWidget(self.addFaceButton)
        self.addFaceLayout.addWidget(self.inputName)

        self.addFaceButton.setFixedWidth(100)
        self.addFaceButton.setFixedHeight(40)

        self.deletionInput = TextEdit(self)
        self.deletionInput.setPlaceholderText('Name of face to delete')
        self.deletionInput.setFixedHeight(40)
        self.deletionInput.setFixedWidth(200)
        self.deletionButton = PushButton('DELETE', self)
        self.deletionButton.setFixedHeight(40)
        self.deletionButton.setFixedWidth(100)
        self.addFaceLayout.addWidget(self.deletionButton)
        self.addFaceLayout.addWidget(self.deletionInput)
        self.CameraBoxLeft = QLabel(self)
        self.CameraBoxLeft.setFixedWidth(640)
        self.CameraBoxLeft.setFixedHeight(480)
        self.CameraBoxRight = QLabel(self)
        self.CameraBoxRight.setFixedWidth(640)
        self.CameraBoxRight.setFixedHeight(480)
        self.CameraBox = QHBoxLayout(self)

        self.CameraBox.addWidget(self.CameraBoxLeft)
        self.CameraBox.addWidget(self.CameraBoxRight)
        self.logger_box.setFixedHeight(200)
        self.logger_box.setFixedWidth(1280)

        self.column_2.addLayout(self.addFaceLayout)
        self.column_2.addLayout(self.CameraBox)

        self.column_2.addWidget(self.logger_box)
        self.bottomLayout.addLayout(self.column_2)
        self.bottomLayout.setSpacing(10)
        self.__initLayout()

        self._main_thread_attach = MainThread(self.config)
        self._main_thread_attach.button_signal.connect(self.set_button_state)
        self._main_thread_attach.logger_signal.connect(self.logger_box.append)
        self._main_thread_attach.update_signal.connect(self.call_update)

        self.startup_card.clicked.connect(self._start_clicked)
        self.addFaceButton.clicked.connect(self.add_clicked)
        self.deletionButton.clicked.connect(self.deleteClicked)
        # set a hash object name for this widget
        self.object_name = md5(f'{time.time()}%{random()}'.encode('utf-8')).hexdigest()
        self.setObjectName(self.object_name)
        self.leftImageTimer = QTimer(self)
        self.rightImageTimer = QTimer(self)
        self.leftImageTimer.timeout.connect(self.updateLeftFrame)
        self.rightImageTimer.timeout.connect(self.updateRightFrame)

    def call_update(self, text):
        if text == 'l':
            self.updateLeftFrame()
        elif text == 'r':
            self.updateRightFrame()
        elif text == 'lr':
            self.updateLeftFrame()
            self.updateRightFrame()

    def set_button_state(self, state):
        self.startup_card.button.setText(state)
        if state == 'START':
            self._main_thread_attach.running = False

    def __initLayout(self):
        self.expandLayout.setSpacing(28)
        self.expandLayout.addWidget(self.info_box)
        self.expandLayout.addWidget(self.startup_card)
        # self.vBoxLayout.addWidget(self.logger_box)
        # self.logger_box.setLayout(self.logger_box_layout)

        self.expandLayout.addLayout(self.bottomLayout)
        self.setLayout(self.expandLayout)
        # self.startupCard.clicked.connect(self.__init_starter)

    def updateLeftFrame(self):
        if self._main_thread_attach._main_thread is not None:
            frame = self._main_thread_attach._main_thread.getLastFrame()
            if frame is None:
                return
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            image = QImage(frame, frame.shape[1], frame.shape[0], frame.strides[0], QImage.Format_RGB888)
            self.CameraBoxLeft.setPixmap(QPixmap.fromImage(image))

    def updateRightFrame(self):
        if self._main_thread_attach._main_thread is not None:
            frame = self._main_thread_attach._main_thread.getRecognized()
            if frame is None:
                return
            frame = cv2.cvtColor(frame, cv2.COLOR_BGR2RGB)
            image = QImage(frame, frame.shape[1], frame.shape[0], frame.strides[0], QImage.Format_RGB888)
            self.CameraBoxRight.setPixmap(QPixmap.fromImage(image))

    def _start_clicked(self):
        if self._main_thread_attach.running:
            self._main_thread_attach.stop_play()
            self.leftImageTimer.stop()
            self.rightImageTimer.stop()
        else:
            self._main_thread_attach.run_choice = 0b11
            self._main_thread_attach.start()
            self.leftImageTimer.start(20)
            self.rightImageTimer.start(20)

    def deleteClicked(self):
        index = self.deletionInput.toPlainText()
        self.get_main_thread()._main_thread.deleteFace(index)

    def add_clicked(self):
        name = self.inputName.toPlainText()
        main = self._main_thread_attach.getMainThread()
        if main.checkNameExist(name):
            main.logger.error('Name Already Exist : [ ' + name + ' ]')
            return
        if not self._main_thread_attach.running:
            self._main_thread_attach.run_choice = 0b01
            self._main_thread_attach.start()
            self.leftImageTimer.start(20)
            self.rightImageTimer.start(20)
        else:
            self._main_thread_attach.getMainThread().recognizeAlive = False
        threading.Thread(target=main.takeSnapshotAndAdd,
                         args=(name,)).start()

    def get_main_thread(self):
        return self._main_thread_attach


class MainThread(QThread):
    button_signal = pyqtSignal(str)
    logger_signal = pyqtSignal(str)
    update_signal = pyqtSignal(str)

    def __init__(self, config):
        super(MainThread, self).__init__()
        self.leftImageTimer = None
        self.config = config
        self.hash_name = md5(f'{time.time()}%{random()}'.encode('utf-8')).hexdigest()
        self._main_thread = None
        self.Main = None
        self.running = False
        self.init_thread = threading.Thread(target=self.initScriptThread)
        self.init_thread.start()
        self.run_choice = 0

    def initScriptThread(self):
        self._main_thread = FaceDetection()
        self._main_thread.loadFaceInfo()
        self._main_thread.setUISignals(self)
        self._main_thread.initCamera()
        self._main_thread.load_pretrained_model()

    def run(self):
        self.running = True
        self._main_thread.logger.info("Starting Face Detection...")
        self.display('STOP')
        self._main_thread.alive = True
        print(self.run_choice)
        if self.run_choice & 1:
            threading.Thread(target=self._main_thread.run).start()
        if self.run_choice & 1 << 1:
            threading.Thread(target=self._main_thread.recognizeThread).start()

    def stop_play(self):
        self.running = False
        self.display('START')
        self._main_thread.stop()

    def display(self, text):
        self.button_signal.emit(text)

    def setMainThread(self):
        self._main_thread = FaceDetection()
        self._main_thread.setUISignals(self)

    def getMainThread(self) -> FaceDetection:
        if self._main_thread is None:
            self.setMainThread()
        return self._main_thread

