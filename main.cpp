#include <iostream>
#include <QString>
#include <QFile>
#include <QTextStream>
#include <QtDebug>
#include <QtSql>
//#include <QtCore/QtCore>

#define MAX_PATTERNS 2048

using namespace std;

typedef struct { QString regex; int type; uint padding; } pattern;

int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);
    pattern patterns[MAX_PATTERNS];
    QFile file("patterns");
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "Erreur, imposible d'ouvrir le fichier patterns.";
    }
    QTextStream stream(&file);
    QString line;
    QStringList list;
    int nb_patterns = 0;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        list = line.split('\t');
        patterns[nb_patterns].regex = list[0];
        patterns[nb_patterns++].type = list[1].toInt();
    }
    /*for (int j = 0; j < nb_patterns; j++)
    {
        qDebug("%s : %i\n", patterns[j].regex.toLocal8Bit().data(), patterns[j].type);
    }*/
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setUserName("haine_test_user");
    db.setPassword("haine_test_usermdp");
    db.setDatabaseName("haine_test");
    if (!db.open())
    {
        qDebug("Erreur : impossible d'ouvrir la base de données.");
    }
    QSqlQuery query, update_hateful, update_type;
    update_hateful.prepare("update corpus1 set haineux=:b where id=:id;");
    update_type.prepare("insert into possede values (:id, :type);");
    QString text, id;
    bool hateful;
    while (true)
    {
        if (!query.exec("select id, contenu from corpus1 where haineux is null limit 10000;"))
            //if (!query.exec("select id, contenu from corpus1 where id = (select min(id) from corpus1 where haineux is null);"))
        {
            qDebug("Erreur : %s\n", query.lastError().text().toLocal8Bit().data());
        }
        while (query.next())
        {
            hateful = false;
            if (!query.next())
            {
                qDebug("Erreur : la requète n'a rien renvoyé.");

                break;
            }
            id = query.value(0).toString();
            text = query.value(1).toString();
            //qDebug("Trouvé : %s, %s\n", id.toLocal8Bit().data(), text.toLocal8Bit().data());
            for (int i = 0; i < nb_patterns; i++)
            {
                QRegExp regex(patterns[i].regex);
                if (text.contains(regex))
                {
                    if (!hateful)
                    {
                        hateful = true;
                        update_hateful.addBindValue("true");
                        update_hateful.addBindValue(id);
                        update_hateful.exec();
                        // éditer corpus1 pour mettre le champ haineux à 1.
                    }
                    // éditer Possède pour ajouter le type
                    update_type.bindValue(":id", id); // VERIFIER le type attendu ici
                    update_type.bindValue(":type", patterns[i].type); // /!\ ATTENTION /!\ modifier la table type pour ajouter une clé entière !!
                    update_type.exec();
                }
            }
            if (!hateful)
            {
                update_hateful.bindValue(":b", "false");
                update_hateful.bindValue(":id", id);
                if (!update_hateful.exec())
                {
                    cout << "Erreur : " << update_hateful.lastError().text().toStdString();
                }
            }
        }
        /* Plus de messages à analyser.
         * Se mettre en pause et attendre un signal du scraper.
         */
    }
    return 0;
}
