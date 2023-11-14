# Author: Leonardo Rossi Leao
# Description: matplotlib widget to be used on Qt applications
# Created at: 2023-10-27
# Last update: 2023-10-28

import numpy as np
from matplotlib.figure import Figure
from matplotlib.backends.qt_compat import QtWidgets
from matplotlib.backends.backend_qt5agg import FigureCanvas
from matplotlib.backends.backend_qt5agg import FigureCanvasQTAgg as FC
from matplotlib.backends.backend_qt5agg import NavigationToolbar2QT


class MatplotlibWidget(FigureCanvas):


    def __init__(self, len_x: int, interval: int, params: list, get_fnc) -> None:

        """
        Params
        ------
        len_x    (int): number of x-axis points that will be shown
        interval (int): interval between each update (in milliseconds)
        """

        super(MatplotlibWidget, self).__init__(
            Figure(dpi=90, facecolor="#f0f0f0")
        )

        self.len_x    = len_x
        self.interval = interval
        self.params   = params
        self.get_fnc  = get_fnc

        self.x = list(range(0, len_x))
        self.y = [
            np.zeros(len_x).tolist() for _ in self.params
        ]

        # Build the figure
        self.figure.subplots_adjust(left=0.05, top=0.95, right=0.95, bottom=0.1)
        self.ax = self.figure.subplots()
        self.ax.ticklabel_format(style="plain") 
        self.ax.set_facecolor("#f0f0f0")
        self.lines = [
            self.ax.plot(self.x, self.y[i], label=param)[0]
            for i, param in enumerate(self.params)
        ]
        self.draw()
        
        self.setFixedHeight(240)

        # Initialize the timer
        self.timer = self.new_timer(interval, [(self.updateCanvas, (), {})])
        self.timer.start()


    def setGetFunction(self, get_fnc) -> None:

        """
        Sets the function that will be used to get the data.
        """

        self.get_fnc = get_fnc


    def updateCanvas(self) -> None:

        """
        Updates the plot regularly.
        """

        for i in range(len(self.lines)):
            self.y[i].append(round(self.get_fnc(self.params[i]), 2))
            self.y[i] = self.y[i][-self.len_x:]
            self.lines[i].set_ydata(self.y[i])
            self.ax.draw_artist(self.lines[i])

        self.ax.draw_artist(self.ax.patch)
        self.ax.grid(linestyle="--", color="#D3D3D3")
        self.ax.relim()
        self.ax.autoscale_view()
        self.draw()
        
        #fast but dont work autoscale
        #self.update()
        #self.flush_events()