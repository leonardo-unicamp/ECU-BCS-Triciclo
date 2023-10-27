# Author: Leonardo Rossi Leao
# Description: main application
# Created at: 2023-10-27
# Last update: 2023-10-27

import sys
from PyQt5 import QtCore
from PyQt5.uic import loadUi
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QMainWindow, QApplication, QWidget

from windows.connection import ConnectionWindow

class App(QMainWindow):


    def __init__(self) -> None:
        super(App, self).__init__()
        loadUi("ui/app.ui", self)

        self.conn_window = ConnectionWindow()

        self.conn_window.signal_connect.connect(self.deviceConnected)
        self.conn_window.signal_disconnect.connect(self.deviceDisconnected)

        self.btn_connect_change.clicked.connect(self.conn_window.show)


    def deviceConnected(self, comm_type: str, device: str):

        """
        Handles with the device connection.
        """

        style  = "background-color: %s; border: 1px solid black;"
        status = f"{comm_type.capitalize()} - {device}"

        self.led_connect_status.setStyleSheet(style % "#378805")
        self.lb_connect_status.setText(status)


    def deviceDisconnected(self):

        """
        Handles with the device disconnection.
        """

        style  = "background-color: %s; border: 1px solid black;"
        status = "No device connected"

        self.led_connect_status.setStyleSheet(style % "#C21807")
        self.lb_connect_status.setText(status)




if __name__ == "__main__":

    """
    Entry point of the application.
    """
    
    app = QApplication(sys.argv)
    app_window = App()
    app_window.show()
    sys.exit(app.exec_())
