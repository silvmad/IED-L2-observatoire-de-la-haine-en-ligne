import dash
import dash_bootstrap_components as dbc

from flask import Flask

# utilisation du serveur flask pour la bdd
server = Flask(__name__)


# lancer l'application
app = dash.Dash(__name__,
                external_stylesheets=[dbc.themes.CYBORG],
                meta_tags=[{'name': 'viewport',
                            'content': 'width=device-width, initial-scale=1.0'}],
                suppress_callback_exceptions=True)
app.title = "twitter dash_hate"


app.server.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.server.config['SQLALCHEMY_ECHO'] = False




