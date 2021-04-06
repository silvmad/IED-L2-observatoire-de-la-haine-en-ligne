#ifndef MAIN_H
#define MAIN_H
#include <QtSql>

#define MAX_PATTERNS 2048

typedef struct { QString regex; int type; uint padding; } pattern;

int load_patterns(QMap<int, QStringList>&, QString file_name);
int add_pattern(QMap<int, QStringList> &patterns, QString regex, int type);
QSqlDatabase open_db();
void parse_result(QSqlQuery query, QMap<int,QStringList> patterns);
int match(QString text, QStringList list);
void set_hateful(QString id, QString val);
void set_type(QString id, int type);

#endif // MAIN_H
