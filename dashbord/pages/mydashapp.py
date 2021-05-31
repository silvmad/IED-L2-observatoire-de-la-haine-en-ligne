import dash_html_components as html
import dash_core_components as dcc
import dash_bootstrap_components as dbc
from datetime import datetime as dt
from dash.dependencies import Output, Input
from dash.exceptions import PreventUpdate
from wordcloud import WordCloud
import plotly.express as px

import base64
from io import BytesIO

from connexion.function import *
from connexion.init_conn import app,con, table_liste



# layout de la page principale (dashboard)
# ************************************************************************
layout = dbc.Container([

    dbc.Row([
        dbc.Col([dbc.Card(
            [
                dbc.CardBody(
                    [
                        html.H5(
                            "Choisissez une date de début et une date de fin:",
                            className="card-text",
                        ),
                        html.Div([dcc.DatePickerRange(
                            id='my-calender',
                            end_date_placeholder_text="date de fin",  # text that appears when no end date chosen
                            start_date_placeholder_text="date de début",  # text that appears when no start date chosen
                            display_format='D-M-Y',
                            first_day_of_week=1,  # Display of calendar when open (0 = dimanche)
                            min_date_allowed=dt(2017, 8, 5),
                            max_date_allowed=dt(2022, 7, 25),
                            initial_visible_month=dt(2021, 4, 1),
                        ),
                            html.Div(id='calender'),
                        ]),
                    ]),
            ],
            color="light",   # https://bootswatch.com/default/ for more card colors
            inverse=True,   # change color of text (black or white)
            outline=False,  # True = remove the block colors from the background and header
            )
        ], xs=12, sm=12, md=12, lg=12, xl=12),
    ]),
    dbc.Row([
        dbc.Col([dbc.Card(
            [
                dbc.CardBody(
                    [
                        html.H5("Évolution des types de haines dans le temps", className="card-title"),
                        dcc.Dropdown(id='menu_line', multi=True, value=['homophobie', 'racisme'],
                                     options=[],placeholder="Selectionnez un type",
                                     style={"color": "#000000"}
                                     ),
                        dcc.Graph(id='line', figure={})
                    ]
                ),
            ],
            color="light",   # https://bootswatch.com/default/ for more card colors
            inverse=True,   # change color of text (black or white)
            outline=False,  # True = remove the block colors from the background and header
            )
        ], xs=12, sm=12, md=12, lg=12, xl=12),
    ]),
    dbc.Row([
        dbc.Col([dbc.Card(
            [
                dbc.CardBody(
                    [
                        html.H5(
                            "Répartition par type de haine", className="card-title"),
                        dcc.Dropdown(id='menu_pie', multi=True, value=['homophobie', 'racisme'],options=[],
                                     style={"color": "#000000"}
                                     ),
                        dcc.Graph(id='mypie', figure={})
                    ]
                ),
            ],
            color="light",   # https://bootswatch.com/default/ for more card colors
            inverse=True,   # change color of text (black or white)
            outline=False,  # True = remove the block colors from the background and header
        )
        ], xs=12, sm=12, md=12, lg=4, xl=4),
        dbc.Col([dbc.Card([
            dbc.CardBody(
                [
                    html.H5("Nuage de mots haineux", className="card-title"),
                ]
            ),
            dbc.Spinner(children=dbc.CardImg(id="wordcloud", title= "Nuage de mots haineux",
                                              style=
                                              {
                                                'width': '100%',
                                                'height': '470px',
                                                'textAlign': 'center',
                                              }
                                              ),
                        size="lg", color="primary", type="border", fullscreen=True,),
        ],
            color="light",  # https://bootswatch.com/default/ for more card colors
            inverse=True,   # change color of text (black or white)
            outline=False,  # True = remove the block colors from the background and header
            )
        ], xs=12, sm=12, md=12, lg=4, xl=4),
        dbc.Col([dbc.Card(
            [
                dbc.CardBody(
                    [
                        html.H5(
                            "Répartition des mots haineux par nombre", className="card-title"),
                        dcc.Dropdown(id='menu_hist', multi=True, value=['bougnoule', 'pd'],options=[],
                                     style={"color": "#000000"}
                                     ),
                        dcc.Graph(id='myhist', figure={}),
                    ]
                ),
            ],
            color="light",   # https://bootswatch.com/default/ for more card colors
            inverse=True,   # change color of text (black or white)
            outline=False,  # True = remove the block colors from the background and header
        )], xs=12, sm=12, md=12, lg=4, xl=4),
    ], no_gutters=False, justify='around'
    ),
    dcc.Interval(id='interval_pg', interval=900000, n_intervals=0),  # activé toutes les 15minutes où quand la page est rechargé
    # dcc.Store inside the app that stores the intermediate value
    dcc.Store(id='stockmemo'),
], fluid=True)


# Callback section: connecting the components
# connecter les composantes html (menu , calendrier) aux graphiques
# ************************************************************************
# calculer, stocker et updater les données à un interval régulier
@app.callback(Output('stockmemo', 'mydata'),
              [
                  Input('my-calender', 'start_date'),
                  Input('my-calender', 'end_date'),
              ])
def update_data(start_date, end_date):
    dff = read_table(table_liste,con)
    if (not start_date) and (not end_date):
        # Return all the rows on initial load.
        return dff.to_dict('records')
    dfm = dff.sort_index().loc[start_date:end_date]
    if dfm.empty:
        return dff.to_dict('records')
    return dfm.to_dict('records')


# partager les données et les lier à chaque graphique
# ce qui amènera à updater chaque graphique à chaque update des données


@app.callback(
    [

        Output('menu_line', 'options'),
        Output('menu_pie', 'options'),
        Output('menu_hist', 'options'),

    ],
    Input('stockmemo', 'mydata'),
)
def update_date_dropdown(data):
    if data is None:
        raise PreventUpdate
    dff = pd.DataFrame.from_dict(data)
    option1 = [{'label': x, 'value': x} for x in sorted(dff['nom_type'].unique())]
    option2 = [{'label': x, 'value': x} for x in sorted(dff['nom_type'].unique())]
    option3 = [{'label': x, 'value': x} for x in sorted(dff['mot'].unique())]
    return option1, option2, option3


# pie chart
@app.callback(
    Output('mypie', 'figure'),
    [
        Input('stockmemo', 'mydata'),
        Input('menu_pie', 'value'),
    ]
)
def update_graph(data, nametype):
    if data is None:
        raise PreventUpdate
    # récupérer les données
    dff = pd.DataFrame.from_dict(data)
    # les filtrer par rapport au menu déroulant
    dff = dff[dff['nom_type'].isin(nametype)]
    # utilisation de plotly express pour la création de graphique
    pifig = px.pie(dff, names='nom_type', hole=.5, labels={'nom_type': 'type de haine '},
                   template='plotly_dark'
                   )
    return pifig


# Histogram
@app.callback(
    Output('myhist', 'figure'),
    [
        Input('stockmemo', 'mydata'),
        Input('menu_hist', 'value'),
    ]

)
def update_graph(data, mots):
    if data is None:
        raise PreventUpdate
    # récupérer mydata
    dff = pd.DataFrame.from_dict(data)
    # filtrer par rapport au choix de l'utilisateur
    dff = dff[dff['mot'].isin(mots)]
    # utilisation de plotly express pour la création de graphique
    fighist = px.histogram(dff, x='mot',
                           labels={'mot': 'mot haineux '
                                   },
                           template='plotly_dark'
                           )
    return fighist


# line chart
@app.callback(
    Output('line', 'figure'),
    [
        Input('stockmemo', 'mydata'),
        Input('menu_line', 'value'),
    ]

)
def update_graph(data, nametype):
    dff = pd.DataFrame.from_dict(data)
    dff = dff[dff['nom_type'].isin(nametype)]
    dfm = dff.groupby(['nom_type', 'date']).size().reset_index(name='count')
    # utilisation de plotly express pour la création de graphique
    figln = px.line(dfm, x='date', y='count', color='nom_type',
                    labels={'date': '', 'count': 'nombre de mots haineux par type'},
                    template='plotly_dark'
                    )
    return figln


# calendrier
@app.callback(
    Output('calender', 'children'),
    [Input('my-calender', 'start_date'),
     Input('my-calender', 'end_date')])
def update_output(start_date, end_date):
    string_prefix = 'vous avez choisi: '
    if start_date is not None:
        start_date_object = dt.fromisoformat(start_date)
        start_date_string = start_date_object.strftime('%d %B, %Y')
        string_prefix = string_prefix + 'début de période: ' + start_date_string + ' | '
    if end_date is not None:
        end_date_object = dt.fromisoformat(end_date)
        end_date_string = end_date_object.strftime('%d %B, %Y')
        string_prefix = string_prefix + 'fin de période: ' + end_date_string
    if len(string_prefix) == len('vous avez choisi: '):
        return 'choisissez une date'
    else:
        return string_prefix


# créer et connecter le wordcloud
# ************************************************************************
# fonction de création de l'image du nuage de mots
def plot_wordcloud(data):
    d = {a: x for a, x in data.values}
    wc = WordCloud(background_color='black', width=380, height=380).generate_from_frequencies(frequencies=d)
    return wc.to_image()


# callback liant le div à l'image du wordcloud
@app.callback(
    Output('wordcloud', 'src'),
    [
        Input('wordcloud', 'id'),
        Input('stockmemo', 'mydata'),
    ])
def make_image(b, data):
    dff = pd.DataFrame.from_dict(data)
    dfm = dff.groupby('mot').size().reset_index(name='count')
    dfm = dfm[['mot', 'count']]
    img = BytesIO()
    plot_wordcloud(data=dfm).save(img, format='PNG')
    return 'data:image/png;base64,{}'.format(base64.b64encode(img.getvalue()).decode())

