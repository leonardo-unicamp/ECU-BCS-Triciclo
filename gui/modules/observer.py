# Author: Leonardo Rossi Leao
# Description: thread that observe variables by serial communication and 
#              executes a callback function when a value changes
# Created at: 2023-10-27
# Last update: 2023-10-27

from time import sleep
from PyQt5.QtCore import QThread, pyqtSignal

class Observer():

    onValueChange = pyqtSignal(str, float)


    def __init__(self, get_function, delay: int = 0.1):
        super(Observer, self).__init__()
        self.get_function = get_function
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


    def stop(self):

        """
        Stops the variable observer.
        """

        self.is_running = False


    def run(self):

        for name, items in self.variables.items():
            value = self.get_function(items["command"])
            if value != items["value"]:
                items["value"] = value
                items["callback"](name, value)
        #sleep(self.delay)


if __name__ == "__main__":


    def get(command: str):
        print(command)
        return 10
    
    def callback(name: str, value: float):
        print("Callback:", name, value)


    obsv = Observer(get)
    obsv.appendVariable("v1", "#g00;", callback)
    obsv.appendVariable("v2", "#g01;", callback)
    obsv.appendVariable("v3", "#g02;", callback)
    obsv.appendVariable("v4", "#g03;", callback)
    obsv.appendVariable("v5", "#g04;", callback)
    obsv.appendVariable("v6", "#g05;", callback)

    obsv.run()