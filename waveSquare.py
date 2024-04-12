import tkinter as tk
from tkinter import ttk
from matplotlib.backends.backend_tkagg import FigureCanvasTkAgg
from matplotlib.figure import Figure
import serial
import matplotlib.animation as animation
from matplotlib import style
import numpy as np

root = tk.Tk()
root.title("Wave")

# Make the main window expandable
root.rowconfigure(0, weight=1)
root.columnconfigure(0, weight=1)

style.use('fivethirtyeight')

# Adjust the figure size here
fig = Figure(figsize=(8, 4))
ax = fig.add_subplot(111)

def animate(i):
    if ser.in_waiting > 0:
        line = ser.readline().decode('utf-8').rstrip()
        try:
            value = float(line)
            with open("sensor_data.txt", "a") as f:
                f.write(f"{value}\n")
        except ValueError:
            print(f"Cannot convert {line} to float")
            return

        data = open('sensor_data.txt','r').read()
        lines = data.split('\n')
        xs = []
        ys = []

        for line in lines[-200:]:
            if len(line) > 1:
                y = float(line)
                xs.append(len(xs))
                ys.append(y)

        ax.clear()
        ax.plot(xs, ys)
        ax.set_ylim(0, 5) 

        try:
            y_div = float(y_div_entry.get())
            x_div = float(x_div_entry.get())
            if y_div > 0 and x_div > 0: 
                ax.set_yticks(np.arange(0, 5, y_div))
                ax.set_xticks(np.arange(0, len(xs), x_div))
        except ValueError:
            pass  

ser = serial.Serial('COM8', 115200)

canvas = FigureCanvasTkAgg(fig, master=root)
canvas_widget = canvas.get_tk_widget()
canvas_widget.grid(row=0, column=0, columnspan=4)


y_div_label = ttk.Label(root, text="Volt_div:")
y_div_label.grid(row=1, column=0)
y_div_entry = ttk.Entry(root)
y_div_entry.grid(row=1, column=1)

x_div_label = ttk.Label(root, text="Time_div:")
x_div_label.grid(row=1, column=2)
x_div_entry = ttk.Entry(root)
x_div_entry.grid(row=1, column=3)

ani = animation.FuncAnimation(fig, animate, interval=50, cache_frame_data=False)


root.mainloop()
