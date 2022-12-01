import numpy as np
import matplotlib.pyplot as plt
import pandas as pd
df = pd.read_csv('https://raw.githubusercontent.com/cca-aerospace/hab/main/data/Pressure%20Over%20Altitude.csv')

ax = plt.figure().add_subplot(projection='3d')

x = df['Pressure (psia)']
y = df['Altitude (m)']
ax.scatter(x, y, zs=0, zdir='y', label='points in (x, z)')

# Make legend, set axes limits and labels
ax.legend()
ax.set_xlim(0, 20)
ax.set_ylim(0, 1)
ax.set_zlim(0, 32000)
ax.set_xlabel('X')
ax.set_ylabel('Y')
ax.set_zlabel('Z')

# Customize the view angle so it's easier to see that the scatter points lie
# on the plane y=0
ax.view_init(elev=20., azim=-35, roll=0)

plt.show()
