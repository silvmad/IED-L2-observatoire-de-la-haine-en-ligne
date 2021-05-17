
import dash_core_components as dcc
import dash_html_components as html
import dash_bootstrap_components as dbc
from dash.dependencies import Output, Input

from pages import contact, mydashapp

from connexion import app, server

app.layout = html.Div([
    dcc.Location(id='url', refresh=False),
    dbc.NavbarSimple(
                    children=[
                        dbc.NavItem(dbc.NavLink("À propos", href="https://plot.ly")),
                        dbc.NavItem(dbc.NavLink("Contact", href="/pages/contact")),
                        dbc.NavItem(dbc.NavLink("dashboard", href="/pages/mydashapp")),

                        dbc.DropdownMenu(
                            children=[
                                dbc.DropdownMenuItem("code source", href="https://github.com/silvmad/IED-L2-observatoire-de-la-haine-en-ligne"),
                                dbc.DropdownMenuItem("nous contacter", href="/contact"),
                            ],
                            nav=True,
                            in_navbar=True,
                            label="More",
                        ),
                        #dbc.Button("reload_page", id="bouton",color="primary",n_clicks=0, className="mr-2"),

                    ],
                    brand="Dashboard detection de haine sur twitter",
                    brand_href="mydashapp.py",
                    brand_style={"font-size":"300%", "color": "red"},
                    color="dark",
                    dark=True,
                ),
    html.Div(id='page-content')
])


@app.callback(Output('page-content', 'children'),
              Input('url', 'pathname'))
def display_page(pathname):
    if pathname == '/pages/contact':
        return contact.layout
    elif pathname == '/pages/mydashapp':
        return mydashapp.layout
    else:
        return mydashapp.layout




if __name__ == '__main__':
    app.run_server(debug=True, port=8050)
