from connexion import load_config, init_db, app, connexion_db

# charger le fichier cnfig
config = load_config("config.sys")

# exraie les paramètres de connexion du dictionnaire
HOST = config.get("Bdd_host")
USER = config.get("Bdd_login")
PASSWORD = config.get("Bdd_secret")
DATABASE = config.get("Bdd")

# se connecter à la BDD
db = connexion_db(USER, PASSWORD, HOST, DATABASE)

#vérifier la bonne connexion
c = init_db(db)
# pour la mise en ligne sur le site distant
server = app.server

# listes des tables de la BDD
table_liste = ['corpus1', 'possede', 'type', 'contient','mot_clé']

#établir la connexion
con = c.engine

