#ifndef TESTDBTAB_H
#define TESTDBTAB_H
#include <QObject>
#include "mainwindow.h"

class TestDBTab : public QObject
{
    Q_OBJECT
public slots:
    void close_messagebox();
    void tab_close_messagebox();
private:
    MainWindow *mw;
    QTabWidget *tw;

private slots:
    void initTestCase();
    void test_save_values();
    void cleanupTestCase();
};


#endif // TESTDBTAB_H
