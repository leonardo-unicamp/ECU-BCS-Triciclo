import matplotlib.pyplot as plt
from datetime import datetime

# FILEPATH: /c:/Users/ASUS/Desktop/ECU_BCS_TRICICLO_PID4/gui/plotter.py

# Read the text file
filename = "./experimento_04_12/18.txt"

def getData(filename):
    dates = []
    values = []

    with open(filename, 'r') as file:
        for line in file:
            line = line.strip()
            if line.count(',') != 1:
                continue
            try:
                date_str, value_str = line.split(',')
                date = datetime.strptime(date_str, "%Y-%m-%d %H:%M:%S")
                value = float(value_str)
                if value > 100:
                    dates.append(date)
                    values.append(value)
            except ValueError:
                continue
    
    return dates, values

# Plot the data
plt.plot(getData("./experimento_04_12/18.txt")[1], getData("./experimento_04_12/16.txt")[1])
plt.xlabel('Date')
plt.ylabel('Value')
plt.title('Data Plot')
plt.show()
