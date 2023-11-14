# Author: Leonardo Rossi Leao
# Description: thread that observe variables by serial communication and 
#              executes a callback function when a value changes
# Created at: 2023-10-27
# Last update: 2023-10-28

from time import sleep
from datetime import datetime
from PyQt5.QtCore import QThread, pyqtSignal

class Observer(QThread):

    onValueChange = pyqtSignal(str, float)

    def __init__(self, get_function, delay: int = 0.1):
        super(Observer, self).__init__()

        self.get_function = get_function
        self.save_path = None
        self.delay = delay

        self.variables = {}
        self.is_running = True


    def appendVariable(self, name: str, command: str, callback):

        """
        Appends a variable to be observed.

        Params
        ------
        name          (str): name of the variable.
        command       (str): command to be sent to the device.
        callback (function): function to be executed when the value changes.
        """

        self.variables[name] = {
            "command":  command,
            "value":    None,
            "callback": callback
        }


    def setSavePath(self, path: str):

        """
        Sets the path where the data will be saved.
        
        Params
        ------
        path (str): path where the data will be saved.
        """

        self.save_path = path


    def stop(self):

        """
        Stops the variable observer.
        """

        self.is_running = False


    def get(self, parameter: str):

        """
        Returns the value of the variable.

        Params
        ------
        parameter (str): name of the variable.
        """

        return self.variables[parameter]["value"]


    def run(self):

        """
        Starts the variable observer. This function save the readed
        data in a file and executes a callback function when a value
        changes.
        """

        while self.is_running:

            now = datetime.now().strftime("%H:%M:%S.%f")

            for name, items in self.variables.items():
                value = self.get_function(items["command"])
                if value is not None:
                    if self.save_path is not None:
                        with open(self.save_path + f"/{name}.txt", "a") as file:
                            file.write(f"{now},{value}\n")
                    if value != items["value"]:
                        items["value"] = value
                        items["callback"](name, value)

            sleep(self.delay)