import plotly.graph_objects as go
import dash
import dash_core_components as dcc
import dash_html_components as html
import pandas as pd
df = pd.read_csv('https://raw.githubusercontent.com/cca-aerospace/hab/main/data/sample%20data%20-%20Sheet1.csv')

fig = go.Figure([go.Scatter(x=df['Time'], y=df['O2 levels'])])
fig.update_xaxes(
  title_text='Time',
  ticktext=["Start","Finish"],
  tickvals=["16:24:16", df['Time'].index.max()],
)
fig.update_yaxes(
  title_text='O2 percentage',
  range=[0,100],
  tickvals = [0,10,20,30,40,50,60,70,80,90,100],
  ticktext = ['0%', '10%', '20%', '30%', '40%', '50%','60%','70%','80%','90%','100%'],
)
app = dash.Dash(__name__)
app.layout = html.Div([
    dcc.Graph(id="line-chart", figure=fig),
])
app.run_server(host="0.0.0.0", debug=True)
