import pandas as pd
from connexion import *



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

