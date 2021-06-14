#ifndef MAIN_H
#define MAIN_H
#include <QtSql>
#include "conflist.h"

#define PATTERNS_FILENAME "../../patterns"
#define KEYWORDS_FILENAME "../../keywords"

enum { PT_FILE_ERR = 1, KW_FILE_ERR, DB_ERR, CONF_FILE_ERR };

typedef struct { QString word; int id; } keyword;

int load_patterns(QMap<int, QStringList>&, QString);
bool is_int(QString);
int add_pattern(QMap<int, QStringList>&, QString, int);
int load_keywords(QList<keyword>&, QString);
QSqlDatabase open_db(ConfList);
void parse_result(QSqlQuery, const QMap<int,QStringList>&, const QList<keyword>&);
int match(QString, QStringList);
void set_hateful(QString, QString);
void set_type(QString, int);
void set_keyword(QString, int);
void signal_handler(int);

#endif // MAIN_H
