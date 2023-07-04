import math

# La force mécanique d'attraction de l'électroaimant est proportionnelle au carré du produit du courant par le nombre de spires.

import plotly.express as px
from dash import Dash, dcc, html, Output, Input
from plotly import data
import pandas as pd
import numpy as np
import plotly.graph_objects as go

df = data.iris()

options_lst = []

options = [{"label": options_lst.capitalize(), "value": opt} for opt in options_lst]

app = Dash(__name__)

app.layout = html.Div(
    [
        dcc.Checklist(
            options=options,
            inline=True,
            value=options_lst,
            id="checklist",
        ),
        dcc.Graph(id="scatter"),
    ]
)

MIN_SPIRES = 250
MAX_SPIRES = 1250
MAX_INTENSITY = 0.5

target_force = math.pow(MAX_INTENSITY * MIN_SPIRES, 2)
max_intensity = 0
iso_forces = {}
data = []
for spires in range(MIN_SPIRES, MAX_SPIRES + 1, MIN_SPIRES):
    for intensity in np.arange(0, MAX_INTENSITY * 1.1, 0.01):
        force =  math.pow(intensity * spires, 2)
        data.append(dict(spires=str(spires), intensity=intensity, force=force))
        max_intensity = max([max_intensity, intensity])
    iso_forces[spires] = math.sqrt(target_force) / spires

df = pd.DataFrame(data)

@app.callback(
    Output("scatter", "figure"),
    Input("checklist", "value"),
)
def update_figure(values):
    fig = px.scatter(
        df,
        x="intensity",
        y="force",
        color="spires",
    )
    for plot in fig.data:
        plot.update(mode='lines')

    iso_intensity = go.Scatter(
        x = [-1] + list(iso_forces.values()) + [max_intensity*1.5],
        y = [target_force] * (len(iso_forces) + 2),
        mode = 'lines+markers',
        line_dash="dot",
        name = "iso-force",
        line_color="red",
    )

    fig = go.Figure(list(fig.data) + [iso_intensity])

    fig.update_yaxes(range=[0, target_force *1.1])
    fig.update_xaxes(range=[0, max_intensity])
    return fig

if __name__ == "__main__":
    app.run_server(debug=True)
