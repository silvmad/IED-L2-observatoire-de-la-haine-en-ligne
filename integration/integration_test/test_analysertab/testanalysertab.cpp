#include <QtTest>
#include <QCoreApplication>
#include "testanalysertab.h"

void TestAnalyserTab::initTestCase()
{
    mw = new MainWindow;
    tw = new QTabWidget(mw);
}

void TestAnalyserTab::test_set_label_started()
{
    AnalyserTab w(tw);
    w.set_label_started();
    QCOMPARE(w.bot_label->text(), "L'analyseur est en cours d'exécution.");
}

void TestAnalyserTab::test_set_label_stopped()
{
    AnalyserTab w(tw);
    w.set_label_stopped();
    QCOMPARE(w.bot_label->text(), "L'analyseur est arrêté.");
}

QTEST_MAIN(TestAnalyserTab)

