#include <unistd.h>
#include <signal.h>
#include "main.h"

using namespace std;

int main()
{
    signal(SIGCONT, signal_handler);
    QString patterns_file = PATTERNS_FILENAME;
    QString keywords_file = KEYWORDS_FILENAME;
    QMap<int,QStringList> patterns;
    QList<keyword> keywords;
    ConfList conf(CONF_FILENAME);
    if (!conf.is_loaded())
    {
        return CONF_FILE_ERR;
    }
    QSqlDatabase db = open_db(conf);
    if (!db.isOpen())
    {
        return DB_ERR;
    }
    if (load_patterns(patterns, patterns_file) < 0)
    {
        return PT_FILE_ERR;
    }
    if (load_keywords(keywords, keywords_file) < 0)
    {
        return KW_FILE_ERR;
    }
    QSqlQuery query;
    while (true)
    {
        query.exec("select id, contenu from messages where haineux is null order by id limit 10000;");
        if (query.size() == 0)
        {
            pause();
        }
        parse_result(query, patterns, keywords);
        query.finish();
    }
}

