import unittest
from unittest import TestCase
from connexion.function import load_config, connexion_db


class Test(TestCase):
    def test_load_config(self):
        dic = load_config("fixtures/configtest.sys")
        res = {'T_api_key': 'x',
                'T_api_secret': 'x',
                'T_api_token': 'x',
                'T_api_token_secret': 'x',
                'Bdd_host': 'localhost', 'Bdd_login': 'postgres', 'Bdd_secret': 'Rahma2011',
                'Bdd': 'haine_base', 'Bdd_table': 'message', 'langue': 'fr',
                'nbre_tweet': '100', 'max_tweet': '500000', 'rate_limite': '450', 'interval': '900'}

        self.assertDictEqual(dic, res)

    def test_empty_dic(self):
        dic = load_config("fixtures/emptyfile")
        res = "le fichier est vide"
        self.assertEqual(dic, res)

    def test_conformity_dic(self):
        dic = load_config("fixtures/pbfile")
        res = "problème de lecture"
        self.assertEqual(dic, res)

    # à refaire avec une vraie bdd test créé avec le script
    def test_connexion_db(self):
        HOST = "localhost"
        USER = "postgres"
        PASSWORD = "Rahma2011"
        DATABASE = "haine_test"
        connex = connexion_db(USER, PASSWORD, HOST, DATABASE)
        self.assertEqual(str(connex), "Engine(postgresql://postgres:***@localhost/haine_test)")

    def test_faillure_connexion_db(self):
        HOST = "localhost"
        USER = "postgres"
        PASSWORD = "mdp"
        DATABASE = "haine_test"
        connex = connexion_db(USER, PASSWORD, HOST, DATABASE)
        self.assertEqual(str(connex), "la connection a echoue")


if __name__ == '__main__':
    unittest.main()







