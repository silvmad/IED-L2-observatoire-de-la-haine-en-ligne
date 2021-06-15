import dash_core_components as dcc
import dash_html_components as html
import dash_bootstrap_components as dbc
from dash.dependencies import Output, Input

from pages import mydashapp, description

from connexion.init_conn import app

# page principale
app.layout = html.Div([
    # represents the URL bar, doesn't render anything
    dcc.Location(id='url', refresh=False),
    dbc.NavbarSimple(id='navdbc', children=[
                        dbc.NavItem(dbc.NavLink("À propos", id="desc",href="/pages/description")),
                        dbc.NavItem(dbc.NavLink("Contact", id='contact',href="/pages/contact")),
                        dbc.NavItem(dbc.NavLink("dashboard",id='dashb', href="/pages/mydashapp")),
                        dbc.NavItem(dbc.NavLink("code source",id='source', href="https://github.com/silvmad/IED-L2-observatoire-de-la-haine-en-ligne")),


                    ],
                    brand="Dashboard detection de haine sur twitter",
                    brand_href="mydashapp.py",
                    brand_style={"font-size":"300%", "color": "white"},
                    color="dark",
                    dark=True,
                ),
    html.Div(id='page-content')
])

# display the pages according to the pathname


@app.callback(Output('page-content', 'children'),
              Input('url', 'pathname'))
def display_page(pathname):
    if pathname == '/pages/mydashapp':
        return mydashapp.layout
    elif pathname == '/pages/description':
        return description.layout
    else:
        return mydashapp.layout


if __name__ == '__main__':
    app.run_server(debug=True, port=2300)
