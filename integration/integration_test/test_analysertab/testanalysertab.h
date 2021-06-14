#ifndef TESTANALYSERTAB_H
#define TESTANALYSERTAB_H
#include <QObject>
#include "mainwindow.h"

class TestAnalyserTab : public QObject
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

#endif // TESTANALYSERTAB_H
