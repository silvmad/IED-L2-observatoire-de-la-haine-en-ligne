from datetime import datetime as dt

import dash
import dash_bootstrap_components as dbc
import dash_core_components as dcc
import dash_html_components as html
import pandas as pd
import plotly.express as px
from dash.dependencies import Output, Input

from flask import Flask
from flask_sqlalchemy import SQLAlchemy


#utilisation du serveur flask
server = Flask(__name__)
#lancer l'application
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP],
                     meta_tags=[{'name': 'viewport',
                            'content': 'width=device-width, initial-scale=1.0'}])

# app = dash.Dash(__name__, server=server, suppress_callback_exceptions=True)

app.server.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False

# function for loading config setting
def load_config(dictionnaire):
     with open("config.sys", 'r') as flux:
          load = flux.readlines()
          for ligne in load:
                conf = ligne.replace("\n", "")
                conf = conf.split("\t")
                dictionnaire[conf[0]] = conf[1]
config = {}
load_config(config)

HOST = config.get("Bdd_host")
USER = config.get("Bdd_login")
PASSWORD = config.get("Bdd_secret")
DATABASE = config.get("Bdd")


# connecter à la bdd
try:
    app.server.config["SQLALCHEMY_DATABASE_URI"] = "postgresql://%s:%s@%s/%s" % (USER,PASSWORD,HOST, DATABASE)
except:
    print("\tError during databse connexion")

db = SQLAlchemy(app.server)

# préparation de la dataframe en récupérant et manipulant le contenu de la BDD avec pandas
df1 = pd.read_sql_table('corpus1', con=db.engine)
df2 = pd.read_sql_table('possede', con=db.engine)
df3 = pd.read_sql_table('type', con=db.engine)
df5 = pd.read_sql_table('mot_clé', con=db.engine)
df6 = pd.read_sql_table('contient', con=db.engine)

df4 = pd.merge(df2, df1[['id', 'date','haineux']], on="id")
df7 = pd.merge(df4, df3, on="id_type")
df8 = pd.merge(df7, df6, on="id")
df = pd.merge(df8, df5, on="id_mot_clé")

#convertir la colonne date écrite en string dans la bdd en date
df['date'] = pd.to_datetime(df['date'])
df.set_index('date', inplace=True)


# Layout section: Bootstrap (https://hackerthemes.com/bootstrap-cheatsheet/)
#utilisation de bootstrap
# ************************************************************************

# titre
app.layout = dbc.Container([
    dbc.Row(
        dbc.Col(html.H1("Dashboard detection de haine sur twitter",
                        className='text-center text-primary mb-4'),
                width=12)
    ),
    # calendrier
    dbc.Row([
        dbc.Col(html.Div([
            dcc.DatePickerRange(
                id='my-date-picker-range',
                end_date_placeholder_text="date de fin",  # text that appears when no end date chosen
                start_date_placeholder_text="date de début",  # text that appears when no start date chosen
                first_day_of_week=1,  # Display of calendar when open (0 = Sunday)
                min_date_allowed=dt(2017, 8, 5),
                max_date_allowed=dt(2021, 5, 25),
                initial_visible_month=dt(2021, 3, 1),
            ),
        ]),
        ),
        dbc.Col(html.Div(id='output-container-date-picker-range')
                ),
    ]),
    # menu déroulant pour le pie_chart
    dbc.Row([
        dbc.Col([
            dcc.Dropdown(id='menu', multi=True, value=['homophobie', 'racisme'],
                         options=[{'label': x, 'value': x}
                                  for x in sorted(df['nom_type'].unique())],
                         ),
            dcc.Graph(id='line', figure={})
        ],
            xs=12, sm=12, md=12, lg=12, xl=12
        ),
    ], align="center"),  # Vertical: start, center, end

    # menu déroulant pour l'histogramme
    dbc.Row([
        dbc.Col([
            dcc.Dropdown(id='my-dpdn2', multi=True, value=['homophobie', 'racisme'],
                         options=[{'label': x, 'value': x}
                                  for x in sorted(df['nom_type'].unique())],
                         ),
            dcc.Graph(id='line-fig2', figure={})
        ],
            xs=12, sm=12, md=12, lg=5, xl=5
        ),
        dbc.Col([
            html.H3("choisissez un mot-clé:",
                    style={"textDecoration": "underline"}),
            dcc.Dropdown(id='my-checklist', multi=True, value=['bougnoule', 'pd'],
                         options=[{'label': x, 'value': x}
                                  for x in sorted(df['mot'].unique())],
                         ),
            dcc.Graph(id='my-hist', figure={}),
        ],
            xs=12, sm=12, md=12, lg=5, xl=5
        ),
    ], align="center")

], fluid=True)


# Callback section: connecting the components
# connecter les composantes html (menu , calendrier) aux graphiques
# ************************************************************************
# Line chart - Single
@app.callback(
    Output('line-fig2', 'figure'),
    [Input('my-dpdn2', 'value'),
     Input('my-date-picker-range', 'start_date'),
     Input('my-date-picker-range', 'end_date')
     ]
)
def update_graph(stock_slctd,start_date, end_date):
    dff = df[df['nom_type'].isin(stock_slctd)]
    dff = dff.loc[start_date:end_date]
    #utilisation de plotly express pour la création de graphique
    figln2 = px.pie(dff, names='nom_type', hole=.5)
    return figln2



# Histogram
@app.callback(
    Output('my-hist', 'figure'),
    [Input('my-checklist', 'value'),
     Input('my-date-picker-range', 'start_date'),
     Input('my-date-picker-range', 'end_date')
     ]

)
def update_graph(stock_slctd,start_date, end_date):
    dff = df[df['mot'].isin(stock_slctd)]
    dff = dff.loc[start_date:end_date]
    dfm = dff.groupby('mot').count().reset_index()
    # utilisation de plotly express pour la création de graphique
    fighist = px.histogram(dfm, x='mot', y='haineux')
    return fighist


@app.callback(
    Output('line', 'figure'),
    [Input('menu', 'value'),
     Input('my-date-picker-range', 'start_date'),
     Input('my-date-picker-range', 'end_date')
     ]

)
def update_graph(stock_slctd,start_date, end_date):
    dff = df[df['nom_type'].isin(stock_slctd)]
    dff = dff.loc[start_date:end_date]
    dfm = dff.groupby(['nom_type', 'date']).size().reset_index(name='count')
    # utilisation de plotly express pour la création de graphique
    figln2 = px.line(dfm, x='date', y='count', color='nom_type')
    return figln2


@app.callback(
    dash.dependencies.Output('output-container-date-picker-range', 'children'),
    [dash.dependencies.Input('my-date-picker-range', 'start_date'),
     dash.dependencies.Input('my-date-picker-range', 'end_date')])
def update_output(start_date, end_date):
    string_prefix = 'vous avez choisi: '
    if start_date is not None:
        start_date_object = dt.fromisoformat(start_date)
        start_date_string = start_date_object.strftime('%B %d, %Y')
        string_prefix = string_prefix + 'début de période: ' + start_date_string + ' | '
    if end_date is not None:
        end_date_object = dt.fromisoformat(end_date)
        end_date_string = end_date_object.strftime('%B %d, %Y')
        string_prefix = string_prefix + 'fin de période: ' + end_date_string
    if len(string_prefix) == len('vous avez choisi: '):
        return 'choisissez une date'
    else:
        return string_prefix


if __name__ == '__main__':
    app.run_server(debug=True, port=3200)
