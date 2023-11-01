import pandas as pd

class Table(pd.DataFrame):
    def __init__ (self, *args, **kwargs):
        super().__init__(*args, **kwargs)

    def from_rows (rows):
        return Table(rows)

    def from_columns (names, *args):
        data = dict()
        for i in range(len(names)):
            data[names[i]] = args[i]
        return Table(data)

    def integrate (self, **kwargs):
        old_columns = list(map(lambda c: list(c[1].values), self.items()))[1:]
        keys = self.columns
        xdata = self.iloc[:, 0].values

        new_columns = [[0] for _ in range(len(old_columns))]

        for step in range(len(xdata) - 1):
            deltax = xdata[step + 1] - xdata[step]
            for i in range(len(old_columns)):
                old_column = old_columns[i]
                new_column = new_columns[i]
                new_column.append(new_column[-1] + (old_column[step + 1] + old_column[step]) * deltax / 2)

        return Table.from_columns(keys, xdata, *new_columns, **kwargs)