import dash_core_components as dcc
import dash_html_components as html
import dash_bootstrap_components as dbc

layout = dbc.Card(
    [
        dbc.CardBody(
            [
                html.H4("présentation du projet", className="card-title"),
                html.P(
                    "Notre projet est de créer un outil qui permette d’étudier les discours de haine sur twitter"
                    "Cet outil pourra extraire des messages de twitter en fonction de mots-clé prédéfinis, puis les analyser afin de"
                    "déterminer leur caractère haineux ou non, ainsi que le cas échéant le type de haine concerné. Les données ainsi"
                    "récoltées et analysées seront ensuite affichées dans une interface web où l’utilisateur final aura la capacité d’en"
                    "manipuler la présentation.",
                    className="card-text",
                ),
                #dbc.Button("Go somewhere", color="primary"),
                #dbc.CardFooter("This is the footer"),
            ]
        ),
    ],

    style={"width": "80%",
            "left": "10%",
    "margin-right":"-20%,",

},
)

"""html.Div([
    html.H4("présentation du projet", style={"textAlign": "center"}),

    html.H5(
            "Notre projet est de créer un outil qui permette d’étudier les discours de haine sur twitter"),
    html.H5( "Cet outil pourra extraire des messages de twitter en fonction de mots-clé prédéfinis, puis les analyser afin de"
            "déterminer leur caractère haineux ou non, ainsi que le cas échéant le type de haine concerné. Les données ainsi"
            "récoltées et analysées seront ensuite affichées dans une interface web où l’utilisateur final aura la capacité d’en"
            "manipuler la présentation."

                ),
    ])"""