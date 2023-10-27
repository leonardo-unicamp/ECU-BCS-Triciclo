# Author: Leonardo Rossi Leao
# Description: contains some common functions
# Created at: 2023-10-27
# Last update: 2023-10-27

import os
import sys
import logging

def getRelativePath() -> None:

    """
    Returns the relative path of the file that calls this function.
    """

    if getattr(sys, 'frozen', False):
        return os.path.dirname(sys.executable)
    return os.path.dirname(__file__)


def configureLogFile(path: str, filename: str) -> None:

    """
    Configures the log file and create the directory if
    not exists.
    
    Params
    ------
    path     (str): path of the log file.
    filename (str): name of the log file.
    """

    if not os.path.exists(path):
        os.makedirs(path)

    logging.basicConfig(
        level=logging.INFO,
        format='%(asctime)s %(levelname)s %(message)s',
        handlers=[
            logging.FileHandler(os.path.join(path, filename)),
            logging.StreamHandler()
        ]
    )