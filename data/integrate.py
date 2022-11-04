import pandas as pd

class Table:
    def __init__ (self, xdata: list, ydata: list):
        self.df = pd.DataFrame(data = {
            "x": xdata,
            "y": ydata,
        })

    def __repr__ (self):
        return repr(self.df)

    def __str__ (self):
        return repr(self)

    def integrate (self):
        new_ydata = [0]
        running_sum = 0
        xdata = self.df.x.values
        ydata = self.df.y.values

        for step in range(len(xdata) - 1):
            deltax = xdata[step + 1] - xdata[step]
            running_sum += (ydata[step + 1] + ydata[step]) * deltax / 2
            new_ydata.append(running_sum)
        
        return Table(self.df.x.values, new_ydata)