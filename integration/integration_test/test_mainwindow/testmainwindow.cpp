#include <QtTest>
#include <QCoreApplication>
#include <QSqlQuery>
#include "mainwindow.h"
#include "testmainwindow.h"
#include "keywordswindow.h"

void TestMainWindow::initTestCase()
{
    QFile f(CONF_FILENAME);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible de créer le fichier de configuration.";
        exit(1);
    }
    QTextStream stream(&f);
    stream << DB_HOST << "\tlocalhost\n";
    stream << DB_USERNAME << "\thaine_obs_test_user\n";
    stream << DB_USER_PW << "\thaine_obs_test_mdp\n";
    stream << DB_DBNAME << "\thaine_obs_test\n";
    f.close();
}

void TestMainWindow::test_verify_kw_sync()
{
    MainWindow w;
    QSqlDatabase db = QSqlDatabase::database();
    db.setUserName("haine_obs_test_user");
    db.setPassword("haine_obs_test_mdp");
    db.setDatabaseName("haine_obs_test");
    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    QCOMPARE(w.verify_kw_sync(), -1);
    db.setHostName("wrong");
    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    QCOMPARE(w.verify_kw_sync(), -1);
    db.setHostName("localhost");
    // Création du fichier de mots-clés de test.
    QFile file(KW_FILENAME);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible de créer le premier fichier de mots-clés de test.";
        return;
    }
    QTextStream stream(&file);
    stream << "mot1" << endl;
    stream << "mot2" << endl;
    stream << "mot3" << endl;
    file.close();
    QSqlQuery q(db);
    QCOMPARE(w.verify_kw_sync(), 0);
    QStringList list;
    db.open();
    q.exec("select mot from mot_clé;");
    for (int i = 0; i < 3; ++i)
    {
        q.next();
        list << q.value(0).toString();
    }
    list.sort();
    QCOMPARE(list[0], "mot1");
    QCOMPARE(list[1], "mot2");
    QCOMPARE(list[2], "mot3");
    db.close();
    file.open(QIODevice::WriteOnly | QIODevice::Text);
    stream << "mot4" << endl;
    stream << "mot5" << endl;
    stream << "mot6" << endl;
    file.close();
    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    QCOMPARE(w.verify_kw_sync(), 0);
    db.open();
    q.exec("select mot from mot_clé;");
    list.clear();
    for (int i = 0; i < 3; ++i)
    {
        q.next();
        list << q.value(0).toString();
    }
    list.sort();
    QCOMPARE(list[0], "mot1");
    QCOMPARE(list[1], "mot2");
    QCOMPARE(list[2], "mot3");
    db.close();
    QTimer::singleShot(150, this, SLOT(tab_close_messagebox()));
    QCOMPARE(w.verify_kw_sync(), 0);
    db.open();
    q.exec("select mot from mot_clé;");
    list.clear();
    for (int i = 0; i < 3; ++i)
    {
        q.next();
        list << q.value(0).toString();
    }
    list.sort();
    QCOMPARE(list[0], "mot4");
    QCOMPARE(list[1], "mot5");
    QCOMPARE(list[2], "mot6");
    db.close();


}

void TestMainWindow::cleanupTestCase()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery q(db);
    q.exec("truncate type, mot_clé, messages, contient, possede;");
    db.close();
    QFile f(KW_FILENAME);
    f.remove();
    f.setFileName(CONF_FILENAME);
    f.remove();
}

void TestMainWindow::close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Enter);
}

void TestMainWindow::tab_close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Tab);
    QTest::keyClick(amw, Qt::Key_Enter);
}

QTEST_MAIN(TestMainWindow)
