import plotly.express as px
from dash.dependencies import Output, Input

from wordcloud import WordCloud
import base64
from io import BytesIO
import time

from dashcard import *


# Callback section: connecting the components
# connecter les composantes html (menu , calendrier) aux graphiques
# ************************************************************************
#calculer, stocker et updater les données à un interval régulier
@app.callback(Output('stockmemo', 'data'),
              [
                  Input('my-date-picker-range', 'start_date'),
                  Input('my-date-picker-range', 'end_date'),
              ])
def update_data(start_date,end_date):
    #dff = pd.read_csv("donnees.csv")
    dff = read_table(table_liste)
    if ((not start_date) and (not end_date))  :
        # Return all the rows on initial load/no country selected.
        return dff.to_dict('records')
    dff = dff.sort_index().loc[start_date:end_date]
    return dff.to_dict('records')

#partager les données et les lier à chaque graphique
#ce qui amènera à updater chaque graphique à chaque update des données de données
# pie chart
@app.callback(
    Output('mypie', 'figure'),
    [
        Input('stockmemo', 'data'),
        Input('menu1', 'value'),
     ]
)
def update_graph(data,nametype):
    if data is None:
        raise PreventUpdate
    #récupérer les données
    dff = pd.DataFrame.from_dict(data)
    #les filtrer par rapport au menu déroulant
    dff = dff[dff['nom_type'].isin(nametype)]
    # utilisation de plotly express pour la création de graphique
    pifig = px.pie(dff, names='nom_type', hole=.5,
                    labels={'nom_type': 'type de haine ',
                            },
                    #color_discrete_sequence=["red", "blue", "orange","green"],
                    template='plotly_dark'

                    )
    return pifig




# Histogram
@app.callback(
    Output('myhist', 'figure'),
    [
        Input('stockmemo', 'data'),
        Input('menu2', 'value'),
     ]

)
def update_graph(data,mots):
    if data is None:
        raise PreventUpdate
    #récupérer la data
    dff = pd.DataFrame.from_dict(data)
    #filtrer par rapport au choix e l'utilisateur
    dff = dff[dff['mot'].isin(mots)]
    # utilisation de plotly express pour la création de graphique
    fighist = px.histogram(dff, x='mot',
                           labels={'mot': 'mot haineux '
                                   },
                           template = 'plotly_dark'
                           )
    return fighist

# line chart
@app.callback(
    Output('line', 'figure'),
    [
        Input('stockmemo', 'data'),
        Input('menu', 'value'),
     ]

)
def update_graph(data,nametype):
    dff = pd.DataFrame.from_dict(data)
    dff = dff[dff['nom_type'].isin(nametype)]
    dfm = dff.groupby(['nom_type', 'date']).size().reset_index(name='count')
    # utilisation de plotly express pour la création de graphique
    figln = px.line(dfm, x='date', y='count', color='nom_type',
                     labels={'date': '',
                             'count': 'nombre de mots haineux par type',
                             },
                    template='plotly_dark'
                     )
    return figln


# calendrier
@app.callback(
    Output('calender', 'children'),
    [Input('my-date-picker-range', 'start_date'),
     Input('my-date-picker-range', 'end_date')])
def update_output(start_date, end_date):
    string_prefix = 'vous avez choisi: '
    if start_date is not None:
        start_date_object = dt.fromisoformat(start_date)
        start_date_string = start_date_object.strftime('%B %d, %Y')
        string_prefix = string_prefix + 'début de période: ' + start_date_string + ' | '
    if end_date is not None:
        end_date_object = dt.fromisoformat(end_date)
        end_date_string = end_date_object.strftime('%B %d, %Y')
        string_prefix = string_prefix + 'fin de période: ' + end_date_string
    if len(string_prefix) == len('vous avez choisi: '):
        return 'choisissez une date'
    else:
        return string_prefix

# créer et connecter le wordcloud
# ************************************************************************
# fonction de création de l'image du nuage de mots
def plot_wordcloud(data):
    d = {a: x for a, x in data.values}
    wc = WordCloud(background_color='black', width=380, height=380).generate_from_frequencies(frequencies=d)
    return wc.to_image()


# callback liant le div à l'image du wordcloud
@app.callback(
    Output('wordcloud', 'src'),
    [
        Input('wordcloud', 'id'),
        Input('stockmemo', 'data'),
     ])

def make_image(b,data):
    dff = pd.DataFrame.from_dict(data)
    dfm = dff.groupby('mot').size().reset_index(name='count')
    dfm = dfm[['mot', 'count']]
    img = BytesIO()
    plot_wordcloud(data=dfm).save(img, format='PNG')
    return 'data:image/png;base64,{}'.format(base64.b64encode(img.getvalue()).decode())


if __name__ == '__main__':
    app.run_server(debug=True, port=8050)
