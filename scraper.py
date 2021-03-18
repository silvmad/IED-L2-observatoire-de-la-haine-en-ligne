#**********************************************************************************************************#
#  This program was written by Mickael D. Pernet on 03/10/2021                                             #
#  For the course of second year of computer science license: program realization                          #
#                                                                                                          #
#  This scrape program uses the libraries:                                                                 #
#              - Tweepy for the use of the Twitter API: https://www.tweepy.org/                            #
#              - psycopg2 for the use of the postgres database: https://pypi.org/project/psycopg2/         #
#                                                                                                          #
#    It also requires permission to use the Twitter API:  https://developer.twitter.com/en                 #
#                                                                                                          #
#    The configuration is in a config.sys file                                                             #
#    the model database:                                                                                   #
#                                                                                                          #
#    CREATE TABLE corpus1 (                                                                                #
#        ID           int PRIMARY KEY,                                                                     #
#        Contenu      text NOT NULL,                                                                       #
#        Date         varchar(22) NOT NULL,                                                                #
#        Haineux      boolean default NULL                                                                 #
#    );                                                                                                    #
#  Note on the size of tweets:                                                                             #
#  With tweepy the use of extended and full_text brings a large disparity in message size so we will use   #
#  text for tweets because tweets have a maximum size of 280 characters, but these characters do not       #
#  include @users, retweet specifications and links.                                                       #
#                                                                                                          #
#  Twitter Scraper version 1.12:                                                                           #
#        Functional version for terminal - tested under Ubuntu 20.x                                        #
#            Add   - error catching in logfile                                                             #
#            fixed - empty line in config                                                                  #
#                  - recording error & always log problem due to cursor                                    # 
#                   (psycopg2.errors.InFailedSqlTransaction)                                               #
#                  - size problem for Contenu                                                              #
#                   (psycopg2.errors.StringDataRightTruncation)                                            #
#  Next: No duplictates, preparation for integration into the project  ...                                 #
#                                                                                                          #
#**********************************************************************************************************#                                   
                
import tweepy
import psycopg2
import datetime
import time
import os
import sys

# function for loading config setting
def load_config(dictionnaire):
     with open("config.sys", 'r') as flux:
          load = flux.readlines()
          for ligne in load:
                conf = ligne.replace("\n", "")
                conf = conf.split("\t")
                dictionnaire[conf[0]] = conf[1]

def check_rate(golist, interval):
     elements = 0
     with open("goliste.txt", 'r') as flux:
          for ligne in flux:
               elements = elements + 1
     return (900.0/interval) * elements 

config = {}
load_config(config)

# Twitter auth
CONSUMER_KEY =  config.get("T_api_key")
CONSUMER_SECRET = config.get("T_api_secret")
ACCES_TOKEN = config.get("T_api_token")
ACCES_TOKEN_SECRET = config.get("T_api_token_secret")
T_INTERVAL = int(config.get("interval"))
T_RATE = int(config.get("rate_limite"))

auth = tweepy.OAuthHandler(CONSUMER_KEY, CONSUMER_SECRET)
auth.set_access_token(ACCES_TOKEN, ACCES_TOKEN_SECRET)

# rate watcher
api = tweepy.API(auth, wait_on_rate_limit=True, wait_on_rate_limit_notify=True)

# check Twitter API connexion
try:
    api.verify_credentials()
    print("\tAuthentication OK")
except:
    print("\tError during authentication")

# Postgres connexion
HOST = config.get("Bdd_host")
USER = config.get("Bdd_login")
PASSWORD = config.get("Bdd_secret")
DATABASE = config.get("Bdd")
BDDTABLE = config.get("Bdd_table")
try:
    c = psycopg2.connect("host=%s dbname=%s user=%s password=%s" % (HOST, DATABASE, USER, PASSWORD))
except:
    print("\tError during databse connexion")
print(f"\t{c}")

cur = c.cursor()

#ID init
cur.execute(f"SELECT max(ID) FROM {BDDTABLE}")
MAX_tuple = cur.fetchone()
MAX = MAX_tuple[0]
if(MAX is None):
     MAX = 0
Current_ID = MAX +1

#loop on interval
start = round(time.time())
modulo_start = start%T_INTERVAL

if(check_rate("goliste.txt", T_INTERVAL) > T_RATE):
     print(f"The number of expressions to search with the given interval exceeds the allowed limits")
else:
     while(1):
          if(round(time.time())%T_INTERVAL == modulo_start):
               print(f"\t{datetime.datetime.utcnow()} :")
               with open("goliste.txt", 'r') as flux:
                    now = datetime.datetime.utcnow()
                    logname = "log"+str(now)+".json"
                    logfile = open(logname, 'a')
                    logfile.write(f'{{"scraplog" : "{now}"\n "record" : [')
                    erreur = 0
                    id_erreur = 1
                    for ligne in flux:
                         motcle = ligne.replace('\n', '')
                         print(f"\tSearch for {motcle}")
                         #fixed v1.12 nul from golist
                         if((len(motcle) != 0) and (motcle.count(" ") < len(motcle))):
                         #search loop on golist
                              for tweet in api.search(q=motcle, lang="fr", result_type='recent', count=100, tweet_mode='extended'):
                                   tweetsafe = tweet.full_text.replace("\n", "")
                                   tweetsafe = tweetsafe.replace("'", '"')
                                   #recording in DB or json recovery log
                                   try:
                                        cur.execute(f"INSERT INTO {BDDTABLE} (ID, Contenu, Date) VALUES ({Current_ID}, '{tweetsafe}', '{tweet.created_at}')")
                                        Current_ID = Current_ID + 1
                                        c.commit()
                                   except:
                                        print("\tFailed to save to database")
                                        #error catch for debuggin and logsave
                                        erreur_s = sys.exc_info()
                                        typerr = u"%s" % (erreur_s[0])
                                        typerr = typerr[typerr.find("'")+1:typerr.rfind("'")]
                                        msgerr = u"%s" % (erreur_s[1])
                                        logfile.write(f'\t{{\n\t\t"key": {id_erreur},\n\t\t"error": "{typerr}",\n\t\t"date": "{tweet.created_at}",\n\t\t"msg": "{tweetsafe}"\n\t}},\n')         
                                        id_erreur = id_erreur +1
                                        #fixed v1.12 cursor error
                                        c.rollback()
               logfile.write("  ]\n}")
               logfile.close()
               #no empty log
               if(id_erreur == 1):
                    os.remove(logname)
               else:
                    erreur = 1
               print("\tLoop complete, wait for the end of the interval")
               print("\t-----------------------------------------------------------------------------------------------------------")
          else:
               time.sleep(1)
c.close()
print(c)