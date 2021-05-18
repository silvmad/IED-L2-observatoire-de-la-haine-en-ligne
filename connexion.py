import dash
import dash_bootstrap_components as dbc

from flask import Flask
from flask_sqlalchemy import SQLAlchemy




# utilisation du serveur flask pour la bdd
server = Flask(__name__)

# lancer l'application
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.CYBORG],
                meta_tags=[{'name': 'viewport',
                            'content': 'width=device-width, initial-scale=1.0'}])
# app = dash.Dash(__name__, server=server, suppress_callback_exceptions=True)

app.server.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False
app.config.suppress_callback_exceptions = True


# fonction de chargement depuis le fichier config
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
    app.server.config["SQLALCHEMY_DATABASE_URI"] = "postgresql://%s:%s@%s/%s" % (USER, PASSWORD, HOST, DATABASE)
except:
    print("\tError during databse connexion")

db = SQLAlchemy(app.server)



