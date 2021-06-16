#ifndef conflist_H
#define conflist_H
#include <QString>
#include <QFile>
#include <iostream>
#include <QTextStream>

#define CONF_FILENAME "../../config"

// Noms des champs du fichier du configuration
#define DB_HOST "Bdd_host"
#define DB_USERNAME "Bdd_login"
#define DB_USER_PW "Bdd_secret"
#define DB_DBNAME "Bdd"
#define T_API_KEY "T_api_key"
#define T_API_SKEY "T_api_secret"
#define T_API_TOK "T_api_token"
#define T_API_STOK "T_api_token_secret"
#define SC_TPR "nbre_tweet"
#define SC_LANG "langue"
#define SC_ITV "interval"
#define SC_RTL "rate_limite"

typedef struct {
    QString name;
    QString value;
} entry;

/* Classe qui permet de manipuler les information de configuration.
 * Contient une liste qui contient les couples clé / valeur contenues dans le
 * fichier de configuration.
 * Permet le chargement depuis un fichier, l'écriture dans un fichier, l'accès
 * et la modification de ces informations.
 * Les information doivent être écrite dans le fichier de la façon suivante :
 * le nom du champ suivi d'une tabulation suivie de la valeur correspondante
 * suivie d'un retour à la ligne. */
class ConfList
{
    friend class TestConfList;
public:
    ConfList();
    ConfList(QString);
    int load(const QString);
    int write(const QString);
    QString getValue(QString);
    int setValue(QString, QString);
    bool is_loaded();
private:
    QList<entry> entries;
    bool loaded;
};

#endif // conflist_H
