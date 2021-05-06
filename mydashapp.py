import plotly.express as px
from dash.dependencies import Output, Input

from wordcloud import WordCloud
import base64
from io import BytesIO

from dashcard import *


# Layout section: Bootstrap (https://hackerthemes.com/bootstrap-cheatsheet/)
# utilisation de bootstrap
# ************************************************************************

app.layout = dbc.Container([

    dbc.Row([
        dbc.Col([card_titre], xs=12, sm=12, md=12, lg=12, xl=12),
        dbc.Col([card_graphique], xs=12, sm=12, md=12, lg=12, xl=12),
        dbc.Col([card_pie], xs=12, sm=12, md=12, lg=6, xl=6),
        dbc.Col([card_hist], xs=12, sm=12, md=12, lg=6, xl=6),
        dbc.Col([card_img], xs=12, sm=12, md=12, lg=12, xl=12),
    ], align='center'),
], fluid=True)


# Callback section: connecting the components
# connecter les composantes html (menu , calendrier) aux graphiques
# ************************************************************************
# pie chart
@app.callback(
    Output('pie-fig', 'figure'),
    [Input('menu1', 'value'),
     Input('my-date-picker-range', 'start_date'),
     Input('my-date-picker-range', 'end_date')
     ]
)
def update_graph(nametype, start_date, end_date):
    dff = df[df['nom_type'].isin(nametype)]
    dff = dff.loc[start_date:end_date]
    # utilisation de plotly express pour la création de graphique
    pifig = px.pie(dff, names='nom_type', hole=.5,
                    labels={'nom_type': 'type de haine ',
                            }
                    )
    return pifig


# Histogram
@app.callback(
    Output('my-hist', 'figure'),
    [Input('menu2', 'value'),
     Input('my-date-picker-range', 'start_date'),
     Input('my-date-picker-range', 'end_date')
     ]

)
def update_graph(mots, start_date, end_date):
    dff = df[df['mot'].isin(mots)]
    dff = dff.sort_index().loc[start_date:end_date]
    # utilisation de plotly express pour la création de graphique
    fighist = px.histogram(dff, x='mot',
                           labels={'mot': 'mot haineux '
                                   }
                           )
    return fighist

# line chart
@app.callback(
    Output('line', 'figure'),
    [Input('menu', 'value'),
     Input('my-date-picker-range', 'start_date'),
     Input('my-date-picker-range', 'end_date')
     ]

)
def update_graph(nametype, start_date, end_date):
    dff = df[df['nom_type'].isin(nametype)]
    dff = dff.sort_index().loc[start_date:end_date]
    dfm = dff.groupby(['nom_type', 'date']).size().reset_index(name='count')
    # utilisation de plotly express pour la création de graphique
    figln = px.line(dfm, x='date', y='count', color='nom_type',
                     labels={'date': '',
                             'count': 'nombre de mots haineux par type',
                             }
                     )
    return figln


# line chart
@app.callback(
    Output('output-container-date-picker-range', 'children'),
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


# fonction de création de l'image du nuage de mots
def plot_wordcloud(data):
    d = {a: x for a, x in data.values}
    #wc = WordCloud(background_color='black', width=800, height=380)
    wc = WordCloud(background_color='black', width=800, height=380).generate_from_frequencies(frequencies=d)
    print(sorted(d.items(), key=lambda t: t[1]))
    #wc.fit_words(d)
    return wc.to_image()


# callback liant le div à l'image du wordcloud
@app.callback(
    Output('image_wc', 'src'),
    Input('image_wc', 'id'))
def make_image(b):
    dfm = df.groupby('mot').size().reset_index(name='count')
    dfm = dfm[['mot', 'count']]
    img = BytesIO()
    plot_wordcloud(data=dfm).save(img, format='PNG')
    return 'data:image/png;base64,{}'.format(base64.b64encode(img.getvalue()).decode())





if __name__ == '__main__':
    app.run_server(debug=True, port=3200)
