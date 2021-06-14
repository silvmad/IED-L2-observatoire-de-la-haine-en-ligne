#include <QtTest>
#include <QCoreApplication>

#include "testdbtab.h"

void TestDBTab::initTestCase()
{
    mw = new MainWindow;
    tw = new QTabWidget(mw);
    QFile f(CONF_FILENAME);
    if (!f.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible d'ouvrir le fichier de configuration de test.";
        exit(1);
    }
    QTextStream stream(&f);
    stream << DB_HOST << "\t" << "localhost\n";
    stream << DB_USERNAME << "\t" << "haine_obs_test_user\n";
    stream << DB_USER_PW << "\t" << "haine_obs_test_mdp\n";
    stream << DB_DBNAME << "\t" << "haine_obs_test\n";
    f.close();
}

void TestDBTab::test_save_values()
{
    QTest::ignoreMessage(QtWarningMsg, "QSqlDatabasePrivate::addDatabase: duplicate connection name 'qt_sql_default_connection', old connection removed.");
    DBTab *w = new DBTab(tw);
    w->hostname->setText("wrong");
    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    w->save_values();
    QCOMPARE(w->hostname->text(), "localhost");
    w->username->setText("haine_obs_test_user2");
    w->user_pw->setText("haine_obs_test_mdp2");
    w->dbname->setText("haine_obs_test2");
    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    w->save_values();
    QCOMPARE(w->hostname->text(), "localhost");
    QCOMPARE(w->username->text(), "haine_obs_test_user2");
    QCOMPARE(w->user_pw->text(), "haine_obs_test_mdp2");
    QCOMPARE(w->dbname->text(), "haine_obs_test2");
    QFile f(CONF_FILENAME);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&f);
    QCOMPARE(stream.readLine().split("\t")[1], "localhost");
    QCOMPARE(stream.readLine().split("\t")[1], "haine_obs_test_user");
    QCOMPARE(stream.readLine().split("\t")[1], "haine_obs_test_mdp");
    QCOMPARE(stream.readLine().split("\t")[1], "haine_obs_test");
    f.close();
    QTimer::singleShot(150, this, SLOT(tab_close_messagebox()));
    w->save_values();
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QCOMPARE(stream.readLine().split("\t")[1], "localhost");
    QCOMPARE(stream.readLine().split("\t")[1], "haine_obs_test_user2");
    QCOMPARE(stream.readLine().split("\t")[1], "haine_obs_test_mdp2");
    QCOMPARE(stream.readLine().split("\t")[1], "haine_obs_test2");
}

void TestDBTab::cleanupTestCase()
{
    QFile f(CONF_FILENAME);
    f.remove();
}

void TestDBTab::close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Enter);
}

void TestDBTab::tab_close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Tab);
    QTest::keyClick(amw, Qt::Key_Enter);
}

QTEST_MAIN(TestDBTab)
