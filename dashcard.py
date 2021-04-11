import dash_core_components as dcc
import dash_html_components as html

from datetime import datetime as dt

from data import *


card_titre =dbc.Card(
    [
        #dbc.h1.card-header("Dashboard detection de haine sur twitter", className='text-center text-primary mb-4'),
        dbc.CardBody(
            [
                html.H1("Dashboard detection de haine sur twitter", className='text-center text-success mb-4'),
                html.H5(
                    "Choisissez une date de début et une date de fin:",
                    className="card-text",
                ),
                html.Div([dcc.DatePickerRange(
                    id='my-date-picker-range',
                    end_date_placeholder_text="date de fin",  # text that appears when no end date chosen
                    start_date_placeholder_text="date de début",  # text that appears when no start date chosen
                    first_day_of_week=1,  # Display of calendar when open (0 = Sunday)
                    min_date_allowed=dt(2017, 8, 5),
                    max_date_allowed=dt(2021, 5, 25),
                    initial_visible_month=dt(2021, 3, 1),
                ),
                html.Div(id='output-container-date-picker-range'),

        ]),
                ]),

    ],
    color="dark",   # https://bootswatch.com/default/ for more card colors
    inverse=True,   # change color of text (black or white)
    outline=False,  # True = remove the block colors from the background and header
)

card_graphique = dbc.Card(
    [
        dbc.CardBody(
            [
                html.H4("Graphique représentant le nombre de tweets haineux par type par date", className="card-title"),
                html.P(
                    "Vous pouvez choisir dans le menu déroulant le type de haine que vous voulez voir sur le graphique:",
                    className="card-text",
                ),
                dcc.Dropdown(id='menu', multi=True, value=['homophobie', 'racisme'],
                             options=[{'label': x, 'value': x}
                                      for x in sorted(df['nom_type'].unique())],
                             ),
                dcc.Graph(id='line', figure={})
            ]
        ),
    ],
    color="dark",   # https://bootswatch.com/default/ for more card colors
    inverse=True,   # change color of text (black or white)
    outline=False,  # True = remove the block colors from the background and header
)

card_pie = dbc.Card(
    [
        dbc.CardBody(
            [
                html.H4("diagramme circulaire représentant la part de chaque type de haine dans le discours global", className="card-title"),
                html.P(
                    "Vous pouvez choisir dans le menu déroulant le type de haine que vous voulez voir sur le graphique:",
                    className="card-text",
                ),
                dcc.Dropdown(id='menu1', multi=True, value=['homophobie', 'racisme'],
                             options=[{'label': x, 'value': x}
                                      for x in sorted(df['nom_type'].unique())],
                             ),
                dcc.Graph(id='pie-fig', figure={})
            ]
        ),
    ],
    color="dark",   # https://bootswatch.com/default/ for more card colors
    inverse=True,   # change color of text (black or white)
    outline=False,  # True = remove the block colors from the background and header
)

card_hist = dbc.Card(
    [
        dbc.CardBody(
            [
                html.H4("Histogramme représentant le nombre de chaque mot haineux dans le discours global", className="card-title"),
                html.P(
                    "Vous pouvez choisir dans le menu déroulant le mot de haineux que vous voulez voir sur le graphique:",
                    className="card-text",
                ),
                dcc.Dropdown(id='menu2', multi=True, value=['bougnoule', 'pd'],
                             options=[{'label': x, 'value': x}
                                      for x in sorted(df['mot'].unique())],
                             ),
                dcc.Graph(id='my-hist', figure={}),
            ]
        ),
    ],
    color="dark",   # https://bootswatch.com/default/ for more card colors
    inverse=True,   # change color of text (black or white)
    outline=False,  # True = remove the block colors from the background and header
)
card_img = dbc.Card([

    html.H3("Nuage de mots haineux",
                    style={"textDecoration": "underline", 'textAlign': 'center'}),

    html.Div(html.Img(id="image_wc"),
             style={'width': '75%', 'margin': 25, 'textAlign': 'center'})
],
    color="dark",   # https://bootswatch.com/default/ for more card colors
    inverse=True,   # change color of text (black or white)
    outline=False,  # True = remove the block colors from the background and header
)
