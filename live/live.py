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
import scipy.fft
import pandas as pd

import electro

electromagnet = electro.electro

external_stylesheets = ['https://codepen.io/chriddyp/pen/bWLwgP.css']

app = dash.Dash(__name__, external_stylesheets=external_stylesheets, update_title="")
app.layout = html.Div(
    html.Div([
        html.H4('Electro-magnet intensity'),
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
    x_y_z = electromagnet.get_last_xyz()
    if not x_y_z:
        return "No data :/"

    style = {'padding': '5px', 'fontSize': '16px'}
    return [
        html.Span(f"X: {x_y_z['x']:+5d}", style=style),
        html.Span(f"Y: {x_y_z['y']:+5d}", style=style),
        html.Span(f"Z: {x_y_z['z']:+5d}", style=style)
    ]

# Multiple components can update everytime interval gets fired.
@app.callback(Output('live-update-graph', 'figure'),
              Input('interval-component', 'n_intervals'))
def update_graph_live(n):
    df = electromagnet.get_xyz_data()

    if df is None or df.empty:
        return "no data :/"

    df = df.tail(60)

    fig_x = px.scatter(df, x="time", y="x")
    fig_x.data[0].update(mode='lines')
    trace_x = fig_x.data[0]

    fig_y = px.scatter(df, x="time", y="y")
    fig_y.data[0].update(mode='lines')
    trace_y = fig_y.data[0]

    fig_z = px.scatter(df, x="time", y="z")
    fig_z.data[0].update(mode='lines')
    trace_z = fig_z.data[0]



    # Create the graph with subplots
    fig = plotly.subplots.make_subplots(rows=3, cols=2, vertical_spacing=0.2)
    fig['layout']['margin'] = {
        'l': 30, 'r': 10, 'b': 30, 't': 10
    }
    fig['layout']['legend'] = {'x': 0, 'y': 1, 'xanchor': 'left'}

    def fft(key):
        FS = electromagnet.delay
        HIST = 10 # s
        data = df[key].tail(int(HIST * FS))
        fft_imag = scipy.fft.fft(data).imag
        fft_real = scipy.fft.fft(data).real
        fft_fre = scipy.fft.fftfreq(n=len(data), d=1/FS)
        points = []
        prev_fre = None
        max_imag = max([abs(min((fft_imag))), max(fft_imag)])
        max_kept_imag = 0
        MAX_FREQ = 40
        MIN_FREQ = 30
        for imag, real, fre in zip(fft_imag, fft_real, fft_fre):
            if prev_fre and prev_fre > 0 and fre < 0:
                points.append(dict(fre=MAX_FREQ, imag=0, real=0))
                points.append(dict(fre=None, imag=None, real=None))
                points.append(dict(fre=-MAX_FREQ, imag=0, real=0))
            if math.isnan(imag): continue
            if abs(fre) > MAX_FREQ: continue
            if abs(fre) < MIN_FREQ: imag = 0
            if abs(imag) < max_imag/2: imag = 0

            points.append(dict(fre=fre, imag=imag, real=real))
            prev_fre = fre
            if abs(imag) > max_kept_imag:
                max_kept_imag = abs(imag)
        points.append(dict(fre=0, imag=0, real=0))

        found = max_kept_imag > 100
        df_points = pd.DataFrame(points)

        return go.Scatter(
            x=df_points["fre"], y=df_points["imag"],
            mode="lines",
            fill='tozeroy',
            line=go.scatter.Line(color="red" if found else "gray"),
            showlegend=False)


    fig.append_trace(trace_x, 1, 1)
    fig.append_trace(trace_y, 2, 1)
    fig.append_trace(trace_z, 3, 1)

    fig.append_trace(fft("x"), 1, 2)
    fig.append_trace(fft("y"), 2, 2)
    fig.append_trace(fft("z"), 3, 2)
    return fig


if __name__ == '__main__':
    electro.start_thread()
    #import pdb;pdb.set_trace()
    app.run_server(debug=False)
