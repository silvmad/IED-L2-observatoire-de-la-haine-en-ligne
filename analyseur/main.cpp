#include <iostream>
#include <unistd.h>
#include "main.h"

using namespace std;

int main(int argc, char** argv)
{
    QCoreApplication a(argc, argv);
    QString patterns_file = "patterns";
    QString keywords_file = "keywords";
    QMap<int,QStringList> patterns;
    QList<keyword> keywords;
    QSqlDatabase db = open_db();
    load_patterns(patterns, patterns_file);
    load_keywords(keywords, keywords_file);
    QSqlQuery query;
    while (true)
    {
        query.exec("select id, contenu from corpus1 where haineux is null order by id limit 10000;");
        if (query.size() == 0)
        {
            pause();
            //break; // Pour plus tard : à la place se mettre en pause et attendre un signal du scraper pour recommencer
        }
        parse_result(query, patterns, keywords);
        query.finish();
    }
    //return 0;
}

