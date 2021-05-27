from .function import load_config, connexion_db
from projet.app import app


# charger le fichier cnfig
config = load_config("config.sys")

# exraie les paramètres de connexion du dictionnaire
HOST = config.get("Bdd_host")
USER = config.get("Bdd_login")
PASSWORD = config.get("Bdd_secret")
DATABASE = config.get("Bdd")

# se connecter à la BDD
con = connexion_db(USER, PASSWORD, HOST, DATABASE)

# pour la mise en ligne sur le site distant
server = app.server

# listes des tables de la BDD
table_liste = ['corpus1', 'possede', 'type', 'contient','mot_clé']



