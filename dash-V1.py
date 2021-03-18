import dash
import dash_bootstrap_components as dbc
import dash_core_components as dcc
import dash_html_components as html
import pandas as pd
import plotly.express as px
from dash.dependencies import Output, Input
from datetime import date

df = pd.read_csv("donnees.csv")
#dm= df.sort_values(by='type')
#dm= df.groupby('type','Date').count().reset_index()
#print(dm)


# https://www.bootstrapcdn.com/bootswatch/
app = dash.Dash(__name__, external_stylesheets=[dbc.themes.BOOTSTRAP],
                meta_tags=[{'name': 'viewport',
                            'content': 'width=device-width, initial-scale=1.0'}]
                )


# Layout section: Bootstrap (https://hackerthemes.com/bootstrap-cheatsheet/)
# ************************************************************************
app.layout = dbc.Container([
    dbc.Row(
        dbc.Col(html.H1("Dashboard detection de haine sur twitter",
                        className='text-center text-primary mb-4'),
                width=12)
    ),

    dbc.Row([
        dbc.Col(html.Div([
            dcc.DatePickerRange(
                id='my-date-picker-range',
                min_date_allowed=date(2017, 8, 5),
                max_date_allowed=date(2021, 2, 25),
                initial_visible_month=date(2020, 12, 1),
                end_date=date(2021, 2, 25),
    ),
        ]),
        ),
        dbc.Col(html.Div(id='output-container-date-picker-range')
),
    ]),

    dbc.Row([

        dbc.Col([
            dcc.Dropdown(id='menu', multi=True, value=['homophobie', 'misogynie'],
                         options=[{'label': x, 'value': x}
                                  for x in sorted(df['type'].unique())],
                         ),
            dcc.Graph(id='line', figure={})
        ],  # width={'size':5, 'offset':0, 'order':2},
            xs=12, sm=12, md=12, lg=12, xl=12
        ),
    ], align="center"),  # Vertical: start, center, end

    dbc.Row([

        dbc.Col([
            dcc.Dropdown(id='my-dpdn2', multi=True, value=['homophobie', 'mysogynie'],
                         options=[{'label':x, 'value':x}
                                  for x in sorted(df['type'].unique())],
                         ),
            dcc.Graph(id='line-fig2', figure={})
        ], #width={'size':5, 'offset':0, 'order':2},
           xs=12, sm=12, md=12, lg=5, xl=5
        ),
        dbc.Col([
            html.H3("choisissez un mot-clé:",
                   style={"textDecoration": "underline"}),
            dcc.Dropdown(id='my-checklist', multi=True, value=['pute', 'pd'],
                         options=[{'label':x, 'value':x}
                                  for x in sorted(df['mot'].unique())],
                         ),
            dcc.Graph(id='my-hist', figure={}),
        ], #width={'size':5, 'offset':1},
           xs=12, sm=12, md=12, lg=5, xl=5
        ),
    ], align="center")

], fluid=True)


# Callback section: connecting the components
# ************************************************************************
# Line chart - Single


# pie chart - multiple
@app.callback(
    Output('line-fig2', 'figure'),
    Input('my-dpdn2', 'value')
)
def update_graph(stock_slctd):
    dff = df[df['type'].isin(stock_slctd)]
    figln2 = px.pie(dff, names='type',hole=.5)
    return figln2


# Histogram
@app.callback(
    Output('my-hist', 'figure'),
    Input('my-checklist', 'value')
)
def update_graph(stock_slctd):
    dff = df[df['mot'].isin(stock_slctd)]
    dfm = dff.groupby('mot').count().reset_index()
    #print(dff)
    #dff= df['type'].value_counts().aggregate({'decompte': pd.Series.nunique})
    #dff = dff[dff['Date']=='2020-12-03']
    fighist = px.histogram(dfm, x='mot', y='Hate')
    return fighist

@app.callback(
    Output('line', 'figure'),
    Input('menu', 'value')
)
def update_graph(stock_slctd):
    dff = df[df['type'].isin(stock_slctd)]
    dfm = dff.groupby(['type', 'Date']).size().reset_index(name='count')
    figln2 = px.line(dfm, x='Date', y='count', color='type')
    return figln2

@app.callback(
    dash.dependencies.Output('output-container-date-picker-range', 'children'),
    [dash.dependencies.Input('my-date-picker-range', 'start_date'),
     dash.dependencies.Input('my-date-picker-range', 'end_date')])
def update_output(start_date, end_date):
    string_prefix = 'vous avez choisi: '
    if start_date is not None:
        start_date_object = date.fromisoformat(start_date)
        start_date_string = start_date_object.strftime('%B %d, %Y')
        string_prefix = string_prefix + 'début de période: ' + start_date_string + ' | '
    if end_date is not None:
        end_date_object = date.fromisoformat(end_date)
        end_date_string = end_date_object.strftime('%B %d, %Y')
        string_prefix = string_prefix + 'fin de période: ' + end_date_string
    if len(string_prefix) == len('vous avez choisi: '):
        return 'choisissez une date'
    else:
        return string_prefix

if __name__=='__main__':
    app.run_server(debug=True, port=3500)
