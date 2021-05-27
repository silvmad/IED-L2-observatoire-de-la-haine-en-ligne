#**********************************************************************************************************#
#  This program was written by Mickael D. Pernet on 05/17/2021                                             #
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
#  Twitter Scraper version 1.14:                                                                           #
#        Functional version for terminal - tested under Ubuntu 20.x                                        #
#            Add    - no user mention (functions removeat & cleanTweet)                                    #
#                   - message formatting for the interface                                                 #
#                   - exception                                                                            #
#            change - use a golist list instead of the file in the search loop                             #
#            fixed  - simple quote problem for recording because ' often use as ’ in string                #
#                   - destruction of empty log files in case of crash                                      #
#  Next: finalization and integration into the project  ...                                                #
#                                                                                                          #
#**********************************************************************************************************#                                   

import tweepy
import psycopg2
import datetime
import time
import os
import sys

#**********************************************************************************************************#                                   
#                                                Auxiliary functions                                       #
#**********************************************************************************************************#                                   

# loading config setting
def load_config(dictionnaire):
     with open("config.sys", 'r') as flux:
          load = flux.readlines()
          for ligne in load:
               conf = ligne.replace("\n", "")
               conf = conf.split("\t")
               try:     
                    dictionnaire[conf[0]] = conf[1]
               except IndexError:
                    print(f'204 Missing information for the field: {ligne}')

def check_rate(golist, interval):
     elements = 0
     with open(golist, 'r') as flux:
          for ligne in flux:
               elements = elements + 1
     return (900.0/interval) * elements 

#v1.14 cleanning fuction + removing username: boolean detection
def removeat(mot):
     result =  mot.startswith('@')
     return result

def cleanTweet(tweet):
     result = ""
     if hasattr(tweet,'retweeted_status'):
          tweetsafe = tweet.retweeted_status.full_text.replace("\n", "")
     else:
          tweetsafe = tweet.full_text.replace("\n", "")
     #1.14 fixed ' problem     
     tweetsafe = tweetsafe.replace("'", "''")
     for word in tweetsafe.split():
          if not(removeat(word)):
               result = result + word + " "
     return result[0: -1]      

#v1.14 log
def closelog(id_erreur, logfile):
     erreur_s = sys.exc_info()
     typerr = u"%s" % (erreur_s[0])
     typerr = typerr[typerr.find("'")+1:typerr.rfind("'")]
     if tweet:
          createAt = tweet.created_at
     else:
          createAt = ""
     logfile.write(f'\t{{\n\t\t"key": {id_erreur},\n\t\t"error": "{typerr}",\n\t\t"date": "{createAt}",\n\t\t"msg": "{tweetsafe}"\n\t}},\n')         
     return (id_erreur + 1)    


#**********************************************************************************************************#                                   
#                                                    Main                                                  #
#**********************************************************************************************************#                                   

# var init
config = {}
load_config(config)
status = True
logfile = None
erreur = 0
id_erreur = 1
tweet = ""
tweetsafe = "" 

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

#v1.14 check Twitter API connexion
try:
    api.verify_credentials()
    print("100 Authentication OK")
except:
    print("401 Error during authentication")
    status = False

# Postgres connexion
HOST = config.get("Bdd_host")
USER = config.get("Bdd_login")
PASSWORD = config.get("Bdd_secret")
DATABASE = config.get("Bdd")
BDDTABLE = config.get("Bdd_table")
try:
    c = psycopg2.connect("host=%s dbname=%s user=%s password=%s" % (HOST, DATABASE, USER, PASSWORD))
except:
    print("401 Error during databse connexion")

try:
     cur = c.cursor()
except NameError:
     print("503 Error during cursor creation")
     status = False

#ID init
try:
     cur.execute(f"SELECT max(ID) FROM {BDDTABLE}")
     MAX_tuple = cur.fetchone()
     MAX = MAX_tuple[0]
     if(MAX is None):
          MAX = 0
     Current_ID = MAX +1
except NameError:
     print("503 Error during ID init")

#setting fot interval
start = round(time.time())
modulo_start = start%T_INTERVAL
nbr_tweet = config.get("nbre_tweet")
bdd_table = config.get("Bdd_table")

#logfile
now = datetime.datetime.utcnow()
logname = "log"+str(now)+".json"
logfile = open(logname, 'a')
logfile.write(f'{{"scraplog" : "{now}"\n "record" : [')

#v1.14 search field list save in a list
list = []
with open("goliste.txt", 'r') as flux:
     for ligne in flux:
          motcle = ligne.replace('\n', '')                   
          if((len(motcle) != 0) and (motcle.count(" ") < len(motcle))):
               list.append(motcle)

# main loop
if(check_rate("goliste.txt", T_INTERVAL) > T_RATE):
     print(f"406 The number of expressions to search with the given interval exceeds the allowed limits")
else:
     while(status):
          if(round(time.time())%T_INTERVAL == modulo_start):
               #v1.14 change: search loop on list
               try:
                    for element in list:
                         try:
                              for tweet in api.search(q=element, lang="fr", result_type='recent', count=nbr_tweet , tweet_mode='extended'):
                                   #fixed 1.14: cleanning function for tweet
                                   tweetsafe = cleanTweet(tweet)
                                   #recording in DB or json recovery log
                                   cur.execute(f"SELECT count(*) FROM {bdd_table} WHERE Contenu LIKE '{tweetsafe}%' AND Date LIKE '{str(tweet.created_at)}';")
                                   duplicate_test = cur.fetchone()
                                   if(duplicate_test[0] == 0):
                                        try:
                                             cur.execute(f"INSERT INTO {BDDTABLE} (ID, Contenu, Date) VALUES ({Current_ID}, '{tweetsafe}', '{tweet.created_at}')")
                                             Current_ID = Current_ID + 1
                                             c.commit()
                                        except:
                                             #v1.14 text message for interface
                                             print("\t503 Failed to save to database")
                                             id_erreur = closelog(id_erreur, logfile)
                                             c.rollback()
                         except:
                              print("503 erreur API twitter")
               #V1.14 end log file in case of crash
               except:
                    id_erreur = closelog(id_erreur, logfile)
                    status = False
                    print(f'400 fatal error')
               #v1.14 text message for interface
               print("205 new data available")
          else:
               try:
                    time.sleep(1)
               except:
                    id_erreur = closelog(id_erreur, logfile)
                    status = False
                    print(f'400 fatal error')
if not(logfile.closed):
     logfile.write("  ]\n}")
     logfile.close()
#no empty log
if (id_erreur == 1):
     os.remove(logname)
try:
     c.close()
except NameError:
     print("503 no cursor to close")