#ifndef CONFARRAY_H
#define CONFARRAY_H
#include <QString>
#include <QFile>
#include <iostream>
#include <QTextStream>

#define CONF_LINE_COUNT 14
#define CONF_FILENAME "config.sys"

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

typedef struct {
    QString name;
    QString value;
} entry;

class ConfArray
{
public:
    ConfArray();
    ConfArray(QString);
    int write(const QString);
    QString getValue(QString);
    int setValue(QString, QString);
private:
    entry entries[CONF_LINE_COUNT];
};

#endif // CONFARRAY_H
