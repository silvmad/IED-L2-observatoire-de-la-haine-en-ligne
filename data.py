import dash
import dash_bootstrap_components as dbc
import pandas as pd

from flask import Flask
from flask_sqlalchemy import SQLAlchemy


# utilisation du serveur flask pour la bdd
server = Flask(__name__)
# lancer l'application
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP],
                meta_tags=[{'name': 'viewport',
                            'content': 'width=device-width, initial-scale=1.0'}])

# app = dash.Dash(__name__, server=server, suppress_callback_exceptions=True)

app.server.config['SQLALCHEMY_TRACK_MODIFICATIONS'] = False


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

# préparation de la dataframe en récupérant et manipulant le contenu de la BDD avec pandas

table_liste =['corpus1','possede','type', 'mot_clé', 'contient']
dic = {}
def read_table(liste):
    for table in liste:
            dic[table] = pd.read_sql_table(table, con=db.engine)


read_table(table_liste)

df = pd.merge(dic['possede'], dic['corpus1'][['id', 'date','haineux']], on="id")
df = pd.merge(df, dic['type'], on="id_type")
df = pd.merge(df, dic['contient'], on="id")
df = pd.merge(df, dic['mot_clé'], on="id_mot_clé")




# convertir la colonne date écrite en string dans la bdd en date
df['date'] = pd.to_datetime(df['date'].map(lambda t: t[:-6]))
df.set_index('date', inplace=True)

