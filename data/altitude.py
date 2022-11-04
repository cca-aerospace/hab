import pandas as pd
from integrate import Table

csv = pd.read_csv("Data.csv")
seconds = list([t / 1000 for t in csv["ms"]])
acceleration = list(csv["accely"])

acceleration = Table(xdata=seconds, ydata=acceleration)
velocity = acceleration.integrate()
altitude = velocity.integrate()

print(acceleration)
print(velocity)
print(altitude)

# TODO: generate altitude csv