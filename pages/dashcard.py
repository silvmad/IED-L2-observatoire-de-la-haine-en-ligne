import dash_core_components as dcc
import dash_html_components as html
import dash_bootstrap_components as dbc

from datetime import datetime as dt




card_titre =dbc.Card(
    [
        dbc.CardBody(
            [
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

card_pie = dbc.Card(
    [
        dbc.CardBody(
            [
                html.H4("diagramme circulaire représentant la part de chaque type de haine dans le discours global", className="card-title"),
                html.P(
                    "Vous pouvez choisir dans le menu déroulant le type de haine que vous voulez voir sur le graphique:",
                    className="text-center",
                ),
                dcc.Dropdown(id='menu1', multi=True, value=['homophobie', 'racisme'],
                             options=[],
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

card_hist = dbc.Card(
    [
        dbc.CardBody(
            [
                html.H4("Histogramme représentant le nombre de chaque mot haineux dans le discours global", className="card-title"),
                html.P(
                    "Vous pouvez choisir dans le menu déroulant le mot de haineux que vous voulez voir sur le graphique:",
                    className="text-center",
                ),
                dcc.Dropdown(id='menu2', multi=True, value=['bougnoule', 'pd'],
                             options=[],
                             style={"color": "#000000"}
                             ),
                dcc.Graph(id='myhist', figure={}),
            ]
        ),
    ],
    color="light",   # https://bootswatch.com/default/ for more card colors
    inverse=True,   # change color of text (black or white)
    outline=False,  # True = remove the block colors from the background and header
)

card_img = dbc.Card([
    dbc.CardBody(
            [
                html.H2("Nuage de mots haineux", className="card-title"),
                html.H4(
                    "nuage de mot représentant les mots clés utilisés en fonction de leur fréquence.",
                    className="card-text",
                ),]
    ),
    dbc.Spinner(children= dbc.CardImg(id="wordcloud", title= "Nuage de mots haineux",
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






