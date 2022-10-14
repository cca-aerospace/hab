import plotly.graph_objects as go
import dash
import dash_core_components as dcc
import dash_html_components as html
import pandas as pd
from plotly.subplots import make_subplots
df = pd.read_csv('https://raw.githubusercontent.com/cca-aerospace/hab/main/data/Altitude%20over%20time%20with%20temp.csv')
fig = make_subplots(rows=2, cols=1, subplot_titles=("Temperature Over Time", "Altitude Over Time"))
fig.add_trace(go.Scatter(x=df['Time'], y=df['Temperature(HAB)'], name = 'Temperature(HAB)'), row=1,col=1)
fig.add_trace(go.Scatter(x=df['Time'], y=df['Temperature(Ground)'], name = 'Temperature(Ground)'),row=1,col=1)
fig.add_trace(go.Scatter(x=df['Time'], y=df['Altitude(HAB)'], name = 'Altitude(HAB)'), row=2,col=1)
fig.add_trace(go.Scatter(x=df['Time'], y=df['Altitude(Ground)'], name = 'Altitude(Ground)'),row=2,col=1)
fig.update_xaxes(
  title_text='Time',
  ticktext=["8am","8:30am","9:00am","9:30am","10am","10:30am", "11am", "End"],
  tickvals=["8:00:00", "8:30:00", "9:00:00", "9:30:00","10:00:00","10:30:00","11:00:00", df['Time'].index.max()],row=1,col=1)
fig.update_xaxes(
  title_text='Time',
  ticktext=["8am","8:30am","9:00am","9:30am","10am","10:30am", "11am", "End"],
  tickvals=["8:00:00", "8:30:00", "9:00:00", "9:30:00","10:00:00","10:30:00","11:00:00", df['Time'].index.max()],row=2,col=1)
fig.update_yaxes(
  title_text='Altitude',
  range=[0,32000],
  tickvals = [0,8000,16000,24000,32000],
  ticktext = ['0 meters','8000 meters','16000 meters','24000 meters','32000 meters'],row=2,col=1
)
fig.update_yaxes(
  title_text='Temperature(Celcius)',
  tickvals = [-70, -60, -50, -40, -30, -20, -10, 0, 10, 20, 30],
  ticktext = ["-70","-60","-50", "-40", "-30", "-20", "-10", "0", "10", "20", "30"],row=1,col=1
)
app = dash.Dash(__name__)
app.layout = html.Div([
    dcc.Graph(id="line-chart", figure=fig),
])
app.run_server(host="0.0.0.0", debug=True)
