# Author: Leonardo Rossi Leao
# Description: window that leads with device connection
# Created at: 2023-10-27
# Last update: 2023-10-27

import sys
sys.path.append("../gui")

from PyQt5.uic import loadUi
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QMainWindow, QApplication

from modules.common import *
from modules.communication import Communication

RELATIVE_PATH = getRelativePath()
configureLogFile(RELATIVE_PATH + "/log", "connection.log")


class ConnectionWindow(QMainWindow):
    
    signal_connect    = pyqtSignal(str, str)
    signal_disconnect = pyqtSignal()

    def __init__(self, gui):
        super(ConnectionWindow, self).__init__()
        loadUi(os.path.join(RELATIVE_PATH, "../ui/connection.ui"), self)
        self.setWindowTitle("Device Connection")

        self.gui = gui
        self.forms = {
            "commType": None,
            "port":     None,
            "desc":     None,
            "baudrate": None,
        }
        self.available_devices = None

        self.btn_serial.clicked.connect(lambda: self.commTypeButton("serial"))
        self.btn_bluetooth.clicked.connect(lambda: self.commTypeButton("bluetooth"))
        self.cb_devices.currentIndexChanged.connect(self.deviceValueChange)
        self.btn_connect.clicked.connect(self.connect)


    def commTypeButton(self, button: str):

        """
        Handles with the connection type button press
        
        Params
        ------
        button (str): type of the connection (serial or bluetooth).
        """

        self.forms["commType"] = button
        
        if button == "serial":
            self.available_devices = self.gui.comm.serialGetAvailableDevices()
        elif button == "bluetooth":
            self.available_devices = self.gui.comm.bluetoothGetAvailableDevices()

        self.cb_devices.clear()
        self.cb_devices.addItems([device[0] for device in self.available_devices])


    def deviceValueChange(self):

        """
        Change the device description label according
        to the selected device.
        """

        target = self.cb_devices.currentText()
        for port, desc in self.available_devices:
            if port == target:
                self.lb_device_desc.setText(desc)
                self.forms["desc"] = desc


    def changeLedStatus(self, color: str, desc: str):

        """
        Changes the led status.

        Params
        ------
        color (str): color of the led.
        desc  (str): description of the device.
        """
        
        style = "background-color: %s; border: 1px solid black;"
        self.led_connect_status.setStyleSheet(style % color)
        self.lb_connect_status.setText(desc)

    
    def connect(self):

        """
        Establishes a connection with some device.
        
        Params
        ------
        commType (str): type of the connection (serial or bluetooth).
        """

        port = self.cb_devices.currentText()
        baudrate = self.cb_baudrate.currentText()

        if self.forms["commType"] == "serial":
            self.gui.comm.serialDisconnect()
            conn_status = self.gui.comm.serialConnect(port, baudrate)
        elif self.forms["commType"] == "bluetooth":
            #TODO: implement bluetooth connection
            pass

        if conn_status:
            self.changeLedStatus("#378805", f"Connected to {port}.")
            self.signal_connect.emit(self.forms["commType"], self.forms["desc"])
            self.btn_connect.setText("Disconnect")
            self.btn_connect.clicked.disconnect()
            self.btn_connect.clicked.connect(self.disconnect)
        else:
            self.changeLedStatus("#C21807", f"Was not possible connect to {port}.")
            self.signal_disconnect.emit()


    def disconnect(self):

        """
        Disconnects the device.
        """

        if self.forms["commType"] == "serial":
            self.gui.comm.serialDisconnect()
            self.changeLedStatus("#C21807", "No device connected.")
            self.btn_connect.setText("Connect")
            self.btn_connect.clicked.disconnect()
            self.btn_connect.clicked.connect(self.connect)
            self.signal_disconnect.emit()

        elif self.forms["commType"] == "bluetooth":
            # TODO: implement bluetooth disconnection
            pass

if __name__ == "__main__":

    """
    Entry point of the application.
    """

    app = QApplication(sys.argv)
    connection_window = ConnectionWindow()
    connection_window.show()
    sys.exit(app.exec_())