# Author: Leonardo Rossi Leao
# Description: handles with communication between devices (bluetooth and serial)
# Created at: 2023-10-27
# Last update: 2023-10-27

import sys
sys.path.append("../gui")

import serial
from serial.tools.list_ports import comports
from modules.common import *

RELATIVE_PATH = getRelativePath()
configureLogFile(RELATIVE_PATH + "/log", "communication.log")

class Communication:

    def __init__(self):

        self.serial = {
            "port":        None,
            "baudrate":    None,
            "connection":  None,
            "isConnected": False
        }

        self.bluetooth = {
            "macAddress":  None,
            "connection":  None,
            "isConnected": False,
        }


    def serialGetAvailableDevices(self):

        """
        Returns a list with tuples containing the serial port and
        its description.
        """

        return [(port, desc) for port, desc, _ in comports()]
    

    def bluetoothGetAvailableDevices(self):

        """
        Returns a list with tuples containing the bluetooth mac address
        and its description.
        """

        return []


    def serialConnect(self, port: str, baudrate: int):

        """
        Establishes a serial connection with some device.

        Params
        ------
        port     (str): serial port to connect.
        baudrate (int): serial baudrate.
        """
        
        self.serial["port"]       = port
        self.serial["baudrate"]   = baudrate

        try:
            self.serial["connection"]  = serial.Serial(port, baudrate, timeout=1)
            self.serial["isConnected"] = True
            logging.info(f"Connected to serial port {port}.")
            return True
        except:
            self.serial["isConnected"] = False
            logging.error(f"Could not connect to serial port {port}.")
            return False
        

    def serialDisconnect(self):

        """
        Disconnects the serial connection.
        """

        if self.serial["isConnected"]:
            self.serial["connection"].close()
            logging.info(f"Disconnected from serial port {self.serial['port']}.")



if __name__ == "__main__":
    com = Communication()
    ports = com.serialGetAvailableDevices()
    com.serialConnect(ports[0][0], 9600)