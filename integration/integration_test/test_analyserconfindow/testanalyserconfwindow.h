#ifndef TESTANALYSERCONFWINDOW_H
#define TESTANALYSERCONFWINDOW_H
#include <QObject>

class TestAnalyserConfWindow : public QObject
{
    Q_OBJECT

public slots:
    void close_messagebox();
    void tab_close_messagebox();

private slots:
    void initTestCase();
    void test_load_types();
    void test_load_patterns();
    void test_load_results();
    void test_analyze_results();
    void test_add_pattern();
    void test_suppr_pattern();
    void test_save_patterns();
    void test_show_patterns_by_type();

    void cleanupTestCase();
};

#endif // TESTANALYSERCONFWINDOW_H
