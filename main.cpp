#include <iostream>
#include "main.h"

using namespace std;

int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);
    QString patterns_file = "patterns";
    QString keywords_file = "keywords";
    QMap<int,QStringList> patterns;
    QList<keyword> keywords;
    load_patterns(patterns, patterns_file);
    load_keywords(keywords, keywords_file);
    QSqlDatabase db = open_db();
    QSqlQuery query;
    while (true)
    {
        query.exec("select id, contenu from corpus1 where haineux is null order by id limit 10000;");
        if (query.size() == 0)
            break; // Pour plus tard : à la place se mettre en pause et attendre un signal du scraper pour recommencer
        parse_result(query, patterns, keywords);
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
        cout << "Erreur, impossible d'ouvrir le fichier patterns." << endl; //futur : prévenir l'interface
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

int load_keywords(QList<keyword>& keywords, QString file_name)
{
    /* Remplit la liste keywords avec le contenu du fichier keywords. */
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "Erreur, impossible d'ouvrir le fichier keywords." << endl; //futur : prévenir l'interface
    }
    QTextStream stream(&file);
    QString line;
    QStringList list;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        list = line.split('\t');
        keyword kw;
        kw.word = list[0];
        kw.id = list[1].toInt();
        keywords << kw;
    }
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

void parse_result(QSqlQuery query, const QMap<int,QStringList>& patterns, const QList<keyword>& keywords)
{
    bool hateful;
    QString text, id;
    QMap<int, QStringList>::const_iterator i;
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
        if (hateful)
        {
            for (int i = 0; i < keywords.size(); i++)
            {
                if (text.contains(keywords[i].word, Qt::CaseInsensitive))
                {
                    set_keyword(id, keywords[i].id);
                }
            }
        }
        else
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

void set_keyword(QString id, int kw_id)
{
    QSqlQuery update_type;
    update_type.prepare("insert into contient values (:id, :type);");
    update_type.bindValue(":id", id);
    update_type.bindValue(":type", kw_id);
    update_type.exec();
}
