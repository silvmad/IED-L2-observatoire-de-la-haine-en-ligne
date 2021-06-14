#ifndef TESTANALYSEUR_H
#define TESTANALYSEUR_H

#include <QObject>
#include "conflist.h"

class TestAnalyseur : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test_load_patterns();
    void test_add_pattern();
    void test_open_db();
    void test_load_keywords();
    void test_parse_results();
    void test_match();
    void test_set_hateful();
    void test_set_type();
    void test_set_keyword();
    void cleanupTestCase();

private:
    ConfList conf;
};

#endif // TESTANALYSEUR_H
