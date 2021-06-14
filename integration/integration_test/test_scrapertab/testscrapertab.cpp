#include <QtTest>
#include "testscrapertab.h"

void TestScraperTab::initTestCase()
{
    mw = new MainWindow;
    tw = new QTabWidget(mw);
}

void TestScraperTab::test_set_label_started()
{
    ScraperTab w(tw);
    w.set_label_started();
    QCOMPARE(w.bot_label->text(), "Le scraper est en cours d'exécution.");
}

void TestScraperTab::test_set_label_stopped()
{
    ScraperTab w(tw);
    w.set_label_stopped();
    QCOMPARE(w.bot_label->text(), "Le scraper est arrêté.");
}

QTEST_MAIN(TestScraperTab)
