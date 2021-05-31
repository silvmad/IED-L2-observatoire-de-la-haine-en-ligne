from dash.testing.application_runners import import_app
from selenium.webdriver.common.keys import Keys

import time


# 2. give each testcase a tcid, and pass the fixture
# as a function argument, less boilerplate
def test_ppii001(dash_duo):

    # 3. define your app inside the test function
    app = import_app("dashbord.index")

    # 4. host the app locally in a thread, all dash server configs could be
    # passed after the first app argument
    dash_duo.start_server(app)

    # 5. use wait_for_* if your target element is the result of a callback,
    # keep in mind even the initial rendering can trigger callbacks
    dash_duo.wait_for_element_by_id('page-content', timeout=10)
    # tester la page de description
    dash_duo.wait_for_element_by_id("desc").click()
    # tester ses contenants
    dash_duo.wait_for_text_to_equal("H4", "présentation du projet")
    dash_duo.wait_for_text_to_equal("P", "Notre projet est de créer un outil qui permette d’étudier les discours de haine sur twitter"
                    "Cet outil pourra extraire des messages de twitter en fonction de mots-clé prédéfinis, puis les analyser afin de"
                    "déterminer leur caractère haineux ou non, ainsi que le cas échéant le type de haine concerné. Les données ainsi"
                    "récoltées et analysées seront ensuite affichées dans une interface web où l’utilisateur final aura la capacité d’en"
                    "manipuler la présentation.")
    #verifier son lien
    location = dash_duo.driver.execute_script(
        """
            return window.location.href
        """
    )
    assert location == "http://localhost:8050/pages/description"
    # tester la page contact
    dash_duo.wait_for_element_by_id("contact").click()
    # tester ses composants
    dash_duo.wait_for_text_to_equal("H4", "Contacter l'equipe")
    dash_duo.wait_for_text_to_equal("P", "Vous pouvez contacter l'équipe via le mail suivant:")
    # verifier son lien
    location = dash_duo.driver.execute_script(
        """
            return window.location.href
        """
    )
    assert location == "http://localhost:8050/pages/contact"
    # tester la page principale(dashboard
    dash_duo.wait_for_element_by_id("dashb").click()
    #vérifier son lien
    location = dash_duo.driver.execute_script(
        """
            return window.location.href
        """
    )
    assert location == "http://localhost:8050/pages/mydashapp"
    # tester ses composants
    dash_duo.wait_for_text_to_equal("H5", "Choisissez une date de début et une date de fin:")
    dash_duo.wait_for_element_by_id("calender").click()
    #time.sleep(5)
    # tester les menus
    dash_duo.find_element("#menu_line").click()
    time.sleep(5)
    dropdown = dash_duo.find_element("#menu_line input")
    dropdown.send_keys(Keys.ENTER)
    time.sleep(5)
    dash_duo.percy_snapshot("dcc.Dropdown dropdown overlaps line")
    # tester les graphiques
    dash_duo.wait_for_element("#line .main-svg")
    dash_duo.find_element("#menu_pie").click()
    time.sleep(5)
    dropdown = dash_duo.find_element("#menu_pie input")
    dropdown.send_keys(Keys.ENTER)
    time.sleep(5)
    dash_duo.wait_for_element("#mypie .main-svg")
    dash_duo.find_element("#menu_hist").click()
    time.sleep(5)
    dropdown = dash_duo.find_element("#menu_hist input")
    dropdown.send_keys(Keys.ENTER)
    time.sleep(5)
    dash_duo.wait_for_element("#myhist .main-svg")
    dash_duo.wait_for_element_by_id("wordcloud")
    # tester le bouton menant au code source
    dash_duo.wait_for_element_by_id("source").click()
    location = dash_duo.driver.execute_script(
        """
            return window.location.href
        """
    )
    assert location == "https://github.com/silvmad/IED-L2-observatoire-de-la-haine-en-ligne"

# 7. to make the checkpoint more readable, you can describe the
    # acceptance criterion as an assert message after the comma.
    assert dash_duo.get_logs() == [], "browser console should contain no error"

    # 8. visual testing with percy snapshot
    dash_duo.percy_snapshot("ppii001-layout",wait_for_callbacks=True)



