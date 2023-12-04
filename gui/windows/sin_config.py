# Author: Leonardo Rossi Leao
# Description: window that leads with device connection
# Created at: 2023-10-27
# Last update: 2023-10-27

import sys
sys.path.append("../gui")

from PyQt5.uic import loadUi
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QMainWindow, QApplication, QLineEdit

from modules.common import *
from modules.communication import Communication

RELATIVE_PATH = getRelativePath()
configureLogFile(RELATIVE_PATH + "/log", "connection.log")


class SinWindow(QMainWindow):
    
    signal_set = pyqtSignal(int, float)

    def __init__(self):
        super(SinWindow, self).__init__()
        loadUi(os.path.join(RELATIVE_PATH, "../ui/sin.ui"), self)
        self.setWindowTitle("Sin Parameters")

        for id in range(21, 25):
            self.findChild(QLineEdit, "le_" + str(id)).returnPressed.connect(lambda id=id: self.setValue(id))
    
    def setValue(self, id):
        value = float(self.findChild(QLineEdit, "le_" + str(id)).text())
        self.signal_set.emit(id, value)

if __name__ == "__main__":

    """
    Entry point of the application.
    """

    app = QApplication(sys.argv)
    connection_window = SinWindow()
    connection_window.show()
    sys.exit(app.exec_())