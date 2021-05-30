import pandas as pd
from flask_sqlalchemy import SQLAlchemy
from app import app


# fonction de chargement depuis le fichier config


def load_config(file):
    dic = {}
    with open(file, 'r') as flux:
        load = flux.readlines()
        for ligne in load:
            conf = ligne.replace("\n", "")
            conf = conf.split("\t")
            try:
                dic[conf[0]] = conf[1]
            except IndexError:
                return "problème de lecture"
    if dic:
        return dic
    else:
        return "le fichier est vide"


# connecter à la bdd
def connexion_db(user,mdp, host, bdd):
    app.server.config["SQLALCHEMY_DATABASE_URI"] = "postgresql://%s:%s@%s/%s" % (user, mdp, host, bdd)
    db = SQLAlchemy(app.server)
    try:
        db.engine.connect()
        return db.engine
    except :
        return "la connection a echoue"


# préparation de la dataframe en récupérant et manipulant le contenu de la BDD avec pandas


def read_table(liste,con):
    dic ={}
    # lire les tables de la bdd
    for table in liste:
        dic[table] = pd.read_sql_table(table, con)
    # fusionner les tables et préparer les données
    df = pd.merge(dic[liste[1]], dic[liste[0]][['id', 'date', 'haineux']], on="id")
    df = pd.merge(df, dic[liste[2]], on="id_type")
    df = pd.merge(df, dic[liste[3]], on="id")
    df = pd.merge(df, dic[liste[4]], on="id_mot_clé")
    # convertir la colonne date écrite en string dans la bdd en date
    df['date'] = pd.to_datetime(df['date'].map(lambda t: t[:-6]), errors='coerce' )
    df.set_index('date', drop=False, inplace=True)
    return df







