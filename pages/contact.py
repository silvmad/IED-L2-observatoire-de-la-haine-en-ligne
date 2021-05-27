import dash_core_components as dcc
import dash_html_components as html
import dash_bootstrap_components as dbc


layout = dbc.Card(
    [
        dbc.CardBody(
            [
                html.H4("Contacter l'equipe", className="card-title"),
                html.P(
                    "Vous pouvez contacter l'équipe via le mail suivant:",
                    className="card-text",
                ),
                dbc.CardLink("dashboardhaine@gmail.com", href="mailto:dashboardhaine@gmail.com"),
            ]
        ),
    ],

    style={"width": "80%",
            "left": "10%",
    "margin-right":"-20%,",
           },
)
