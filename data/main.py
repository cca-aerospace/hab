import plotly.graph_objects as go
import dash
import dash_core_components as dcc
import dash_html_components as html
import pandas as pd
df = pd.read_csv('') #put csv file link here
fig = go.Figure([go.Scatter(x=df['foo'], y=df['foo'])])
app = dash.Dash(__name__)
app.layout = html.Div([
    dcc.Graph(id="line-chart", figure=fig),
])
app.run_server(host="0.0.0.0", debug=True)
