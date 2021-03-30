#include <iostream>
//#include <QFile>
//#include <QTextStream>
#include <QtDebug>
#include "main.h"

using namespace std;

int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);
    QString patterns_file = "patterns";
    QMap<int,QStringList> patterns;
    load_patterns(patterns, patterns_file);
    QSqlDatabase db = open_db();
    QSqlQuery query;
    while (true)
    {
        query.exec("select id, contenu from corpus1 where haineux is null order by id limit 10000;");
        if (query.size() == 0)
            break; // Pour plus tard : à la place se mettre en pause et attendre un signal du scraper pour recommencer
        parse_result(query, patterns);
        query.finish();
    }
    return 0;
}

int load_patterns(QMap<int, QStringList>& patterns, QString file_name)
{
    /* Remplit le dictionnaire patterns avec le contenu du fichier patterns. */
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "Erreur, imposible d'ouvrir le fichier patterns."; //futur : prévenir l'interface
    }
    QTextStream stream(&file);
    QString line;
    QStringList list;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        list = line.split('\t');
        add_pattern(patterns, list[0], list[1].toInt());
    }
    return 0;
}

int add_pattern(QMap<int, QStringList>& patterns, QString regex, int type)
{
    if (!patterns.contains(type))
    {
        patterns.insert(type, QStringList());
    }
    patterns[type] << regex;
    return 0;
}

QSqlDatabase open_db()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setUserName("haine_test_user");
    db.setPassword("haine_test_usermdp");
    db.setDatabaseName("haine_test");
    if (!db.open())
    {
        qDebug("Erreur : impossible d'ouvrir la base de données.");
    }
    return db;
}

void parse_result(QSqlQuery query, QMap<int,QStringList> patterns)
{
    bool hateful;
    QString text, id;
    QMap<int, QStringList>::iterator i;
    while (query.next())
    {
        hateful = false;
        id = query.value(0).toString();
        text = query.value(1).toString();
        for (i = patterns.begin(); i != patterns.end(); i++)
        {
            if (match(text, i.value()))
            {
                if (!hateful)
                {
                    hateful = true;
                    set_hateful(id, "true");
                }
                set_type(id, i.key());
            }
        }
        if (!hateful)
        {
            set_hateful(id, "false");
        }
    }
}

int match(QString text, QStringList list)
{
    QRegExp regex;
    for (int i = 0; i < list.size(); i++)
    {
        regex.setPattern(list[i]);
        if (text.contains(regex))
        {
            return i + 1;
        }
    }
    return 0;
}

void set_hateful(QString id, QString val)
{
    QSqlQuery update_hateful;
    update_hateful.prepare("update corpus1 set haineux=:b where id=:id;");
    update_hateful.bindValue(":b", val);
    update_hateful.bindValue(":id", id);
    update_hateful.exec();
}

void set_type(QString id, int type)
{
    QSqlQuery update_type;
    update_type.prepare("insert into possede values (:id, :type);");
    update_type.bindValue(":id", id);
    update_type.bindValue(":type", type);
    update_type.exec();
}
