# Author: Leonardo Rossi Leao
# Description: main application
# Created at: 2023-10-27
# Last update: 2023-10-28

import sys
sys.path.append("../gui")

from PyQt5.uic import loadUi
from PyQt5.QtCore import pyqtSignal
from PyQt5.QtWidgets import QMainWindow, QApplication, QCheckBox

class ParametersWindow(QMainWindow):


    signal_add = pyqtSignal(list)


    def __init__(self) -> None:
        super(ParametersWindow, self).__init__()
        loadUi("ui/parameters.ui", self)
        self.setWindowTitle("Parameters")

        self.selected = []
        self.num_options = 17

        self.btn_add.clicked.connect(self.checkSelected)


    def checkSelected(self):

        """
        Checks which parameters are selected and sends
        them to callback function.
        """
        
        self.selected = []
        for i in range(self.num_options):
            checkbox = self.findChild(QCheckBox, "cb_%.2d" % i)
            if checkbox.isChecked():
                self.selected.append("%.2d" % i)
        self.signal_add.emit(self.selected)


if __name__ == "__main__":

    """
    Entry point of the application.
    """

    app = QApplication(sys.argv)
    connection_window = ParametersWindow()
    connection_window.show()
    sys.exit(app.exec_())
