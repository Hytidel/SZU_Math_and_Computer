import random
import subprocess
import threading
from datetime import datetime

from PyQt5.QtWidgets import QAbstractItemView, QTableWidgetItem, QHBoxLayout
from qfluentwidgets import FluentWindow, TableWidget, FluentIcon as FIF
from qfluentwidgets import TextEdit, PushButton

class HistoryWindow(FluentWindow):
    def __init__(self):
        super().__init__()
        self.layout = QHBoxLayout(self)
        self.setObjectName('HistoryWindow' + random.randint(0, 100000).__str__())
        self.information = None
        self.table_view = TableWidget(self)
        self.table_view.setColumnCount(3)
        self.table_view.setSelectionBehavior(QAbstractItemView.SelectRows)
        self.table_view.setSelectionMode(QAbstractItemView.SingleSelection)
        self.table_view.setColumnWidth(0, 80)
        self.table_view.setColumnWidth(1, 160)
        self.table_view.setColumnWidth(2, 360)
        # hide index column
        self.table_view.verticalHeader().setVisible(False)

        self.table_view.setHorizontalHeaderLabels(['Index', 'Name', 'Image Path'])

        self.table_view.setObjectName('HistoryTable' + random.randint(0, 100000).__str__())
        self.addSubInterface(icon=FIF.CARE_RIGHT_SOLID, interface=self.table_view, text='Face Library')

    def fetch_update_info(self):
        self.table_view.setRowCount(len(self.information))
        for i, entry in enumerate(self.information):
            self.table_view.setItem(i, 0, QTableWidgetItem(str(i+1)))
            self.table_view.setItem(i, 1, QTableWidgetItem(entry['name']))
            self.table_view.setItem(i, 2, QTableWidgetItem(entry['imgPath']))
