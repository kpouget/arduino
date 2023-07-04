#! /usr/bin/env python3
import datetime
import logging
logging.getLogger('werkzeug').setLevel(logging.ERROR)
import math

import dash
from dash import dcc, html
import plotly
import plotly.subplots
from dash.dependencies import Input, Output
import plotly.express as px
import plotly.graph_objects as go
import pandas as pd

import sound

drum = sound.drum

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

app = dash.Dash(__name__, external_stylesheets=external_stylesheets, update_title="")
app.layout = html.Div(
    html.Div([
        html.H4('Drum notes'),
        html.Div(id='live-update-text'),
        dcc.Graph(id='live-update-graph'),
        dcc.Interval(
            id='interval-component',
            interval=1*1000, # in milliseconds
            n_intervals=0
        )
    ])
)


@app.callback(Output('live-update-text', 'children'),
              Input('interval-component', 'n_intervals'))
def update_metrics(n):
    data = drum.get_last_data()
    if not data:
        return "No data :/"

    style = {'padding': '5px', 'fontSize': '16px'}
    return [
        html.Span(f"{data}", style=style),
    ]

# Multiple components can update everytime interval gets fired.
@app.callback(Output('live-update-graph', 'figure'),
              Input('interval-component', 'n_intervals'))
def update_graph_live(n):
    df = drum.get_data()

    if df is None or df.empty:
        return "no data :/"

    df = df.tail(60)

    fig = px.scatter(df, x="time", y="level", color="note")
    for trace in fig.data:
        trace.update(mode='lines')
    return fig

if __name__ == '__main__':
    sound.start_thread()
    #import pdb;pdb.set_trace()
    app.run_server(debug=False)
