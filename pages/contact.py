import dash_core_components as dcc
import dash_html_components as html





layout = html.Div([
    html.H1("Contacter l'equipe", style={"textAlign": "center"}),

    html.P(
                    "Vous pouvez contacter l'équipe via le lien suivant:",
                    className="text-center",
                ),
    dcc.Link('dashboardhaine@gmail.com', href='"mailto:dashboardhaine@gmail.com"')
    ])