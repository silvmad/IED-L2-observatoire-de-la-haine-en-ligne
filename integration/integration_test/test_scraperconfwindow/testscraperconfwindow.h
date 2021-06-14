#ifndef TESTANALYSERTAB_H
#define TESTANALYSERTAB_H
#include <QObject>

class TestScraperConfWindow : public QObject
{
    Q_OBJECT

private:

private slots:
    void initTestCase();
    void test_save_values();
    void cleanupTestCase();

};

#endif // TESTANALYSERTAB_H
