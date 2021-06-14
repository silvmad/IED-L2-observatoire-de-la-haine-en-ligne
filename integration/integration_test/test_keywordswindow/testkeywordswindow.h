#ifndef TESTKEYWORDSWINDOW_H
#define TESTKEYWORDSWINDOW_H

#include <QtTest>

class TestKeywordsWindow : public QObject
{
    Q_OBJECT

public slots:
    void close_messagebox();
    void tab_close_messagebox();

private slots:
    void initTestCase();
    void test_load_keywords();
    void test_synchronize_db();
    void test_load_kw_from_file();
    void test_load_kw_from_db();
    void test_add_kw();
    void test_suppr_kw();
    void test_save_kws();
    void cleanupTestCase();
};

#endif // TESTKEYWORDSWINDOW_H
