#include <QtTest>
#include <QCoreApplication>

#include "testscraperconfwindow.h"
#include "scraperconfwindow.h"

void TestScraperConfWindow::initTestCase()
{
    QFile f(CONF_FILENAME);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible d'ouvrir le fichier de configuration de test.";
        exit(1);
    }
    QTextStream stream(&f);
    stream << T_API_KEY << "\ttapikey\n";
    stream << T_API_SKEY << "\ttapiskey\n";
    stream << T_API_TOK << "\ttapitok\n";
    stream << T_API_STOK << "\ttapistok\n";
    stream << SC_TPR << "\t1\n";
    stream << SC_LANG << "\tsclang\n";
    stream << SC_ITV << "\t1\n";
    stream << SC_RTL << "\t1\n";
    f.close();
}

void TestScraperConfWindow::test_save_values()
{
    ScraperConfWindow w;
    w.key->setText("tapikey2");
    w.skey->setText("tapiskey2");
    w.tok->setText("tapitok2");
    w.stok->setText("tapistok2");
    w.sopt_tpr->setValue(2);
    w.sopt_lang->setCurrentText("Anglais");
    w.sopt_itv->setValue(2);
    w.sopt_rate_limit->setValue(2);
    w.save_values();
    QFile f(CONF_FILENAME);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&f);
    QCOMPARE(stream.readLine().split("\t")[1], "tapikey2");
    QCOMPARE(stream.readLine().split("\t")[1], "tapiskey2");
    QCOMPARE(stream.readLine().split("\t")[1], "tapitok2");
    QCOMPARE(stream.readLine().split("\t")[1], "tapistok2");
    QCOMPARE(stream.readLine().split("\t")[1], "2");
    QCOMPARE(stream.readLine().split("\t")[1], "eng");
    QCOMPARE(stream.readLine().split("\t")[1], "2");
    QCOMPARE(stream.readLine().split("\t")[1], "2");
}

void TestScraperConfWindow::cleanupTestCase()
{
    QFile f(CONF_FILENAME);
    f.remove();
}

QTEST_MAIN(TestScraperConfWindow)

