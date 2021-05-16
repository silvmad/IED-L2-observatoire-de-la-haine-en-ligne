import dash_core_components as dcc
import dash_html_components as html

from datetime import datetime as dt

from data import *



card_titre =dbc.Card(
    [
        dbc.CardBody(
            [
                dbc.NavbarSimple(
                    children=[
                        dbc.NavItem(dbc.NavLink("À propos", href="https://plot.ly")),

                        dbc.DropdownMenu(
                            children=[
                                dbc.DropdownMenuItem("code source", href="https://github.com/silvmad/IED-L2-observatoire-de-la-haine-en-ligne"),
                                dbc.DropdownMenuItem("nous contacter", href="#"),
                            ],
                            nav=True,
                            in_navbar=True,
                            label="More",
                        ),
                        #dbc.Button("reload_page", id="bouton",color="primary",n_clicks=0, className="mr-2"),

                    ],
                    brand="Dashboard detection de haine sur twitter",
                    brand_href="mydashapp.py",
                    brand_style={"font-size":"300%", "color": "red"},
                    color="dark",
                    dark=True,
                ),
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
                             options=[{'label': x, 'value': x}
                                      for x in sorted(df['nom_type'].unique())],placeholder="Selectionnez un type",
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
                             options=[{'label': x, 'value': x}
                                      for x in sorted(df['nom_type'].unique())],
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
                             options=[{'label': x, 'value': x}
                                      for x in sorted(df['mot'].unique())],
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

# Layout section: Bootstrap (https://hackerthemes.com/bootstrap-cheatsheet/)
# utilisation de bootstrap
# ************************************************************************

app.layout = dbc.Container([

    dbc.Row([
        dbc.Col([card_titre], xs=12, sm=12, md=12, lg=12, xl=12),
    ]),
    dbc.Row([
        dbc.Col([card_graphique], xs=12, sm=12, md=12, lg=12, xl=12),

         ]),
    dbc.Row([
        dbc.Col([card_pie], xs=12, sm=12, md=12, lg=4, xl=4),
        dbc.Col([card_img], xs=12, sm=12, md=12, lg=12, xl=4),
        dbc.Col([card_hist], xs=12, sm=12, md=12, lg=4, xl=4),


    ], no_gutters= False , justify='around'
    ),
    dcc.Interval(id='interval_pg', interval=900000, n_intervals=0),  # activated once/week or when page refreshed
    # dcc.Store inside the app that stores the intermediate value
    dcc.Store(id='stockmemo')

],fluid=True)



