#ifndef TESTSCRAPERTAB_H
#define TESTSCRAPERTAB_H
#include <QObject>
#include "mainwindow.h"

class TestScraperTab : public QObject
{
    Q_OBJECT
private:
    MainWindow *mw;
    QTabWidget *tw;
private slots:
    void initTestCase();
    void test_set_label_started();
    void test_set_label_stopped();
};

#endif // TESTSCRAPERTAB_H
