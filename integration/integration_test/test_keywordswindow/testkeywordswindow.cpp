#include <QtSql>
#include "testkeywordswindow.h"
#include "keywordswindow.h"

void TestKeywordsWindow::initTestCase()
{
    QSqlDatabase test_db = QSqlDatabase::addDatabase("QPSQL");
    test_db.setHostName("localhost");
    test_db.setUserName("haine_obs_test_user");
    test_db.setPassword("haine_obs_test_mdp");
    test_db.setDatabaseName("haine_obs_test");
    if (!test_db.open())
    {
        qDebug() << "Échec de la connexion à la bdd de test.";
        exit(1);
    }
    QSqlQuery q(test_db);
    q.exec("insert into mot_clé values (1, 'mot1');");
    q.exec("insert into mot_clé values (2, 'mot2');");
    q.exec("insert into mot_clé values (3, 'mot3');");
    test_db.close();

    // Création du fichier de mots-clés de test.
    QFile file("keywords.test");
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

    // Création du deuxième fichier de mots-clés de test.
    file.setFileName("keywords2.test");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible de créer le deuxième fichier de mots-clés de test.";
        return;
    }
    stream << "mot4" << endl;
    stream << "mot5" << endl;
    stream << "mot6" << endl;
    file.close();
}

void TestKeywordsWindow::test_load_keywords()
{
    QSqlDatabase db;
    KeyWordsWindow *w = new KeyWordsWindow;

    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    w->load_keywords("wrong");
    QCOMPARE(w->list_model->rowCount(), 0);

    w->load_keywords("keywords.test");
    QModelIndex idx = w->list_model->index(0);
    QCOMPARE(w->list_model->data(idx).toString(), "mot1");
    idx = w->list_model->index(1);
    QCOMPARE(w->list_model->data(idx).toString(), "mot2");
    idx = w->list_model->index(2);
    QCOMPARE(w->list_model->data(idx).toString(), "mot3");

    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    w->load_keywords("keywords2.test");
    db = QSqlDatabase::database();
    db.open();
    QSqlQuery q(db);
    q.exec("select mot from mot_clé;");
    q.next();
    QCOMPARE(q.value(0).toString(), "mot1");
    q.next();
    QCOMPARE(q.value(0).toString(), "mot2");
    q.next();
    QCOMPARE(q.value(0).toString(), "mot3");
    idx = w->list_model->index(0);
    QCOMPARE(w->list_model->data(idx).toString(), "mot4");
    idx = w->list_model->index(1);
    QCOMPARE(w->list_model->data(idx).toString(), "mot5");
    idx = w->list_model->index(2);
    QCOMPARE(w->list_model->data(idx).toString(), "mot6");
    db.close();

    QTimer::singleShot(50, this, SLOT(tab_close_messagebox()));
    w->load_keywords("keywords2.test");
    db.open();
    QStringList list;
    q.exec("select mot from mot_clé;");
    q.next();
    list << q.value(0).toString();
    q.next();
    list << q.value(0).toString();
    q.next();
    list << q.value(0).toString();
    list.sort();
    QCOMPARE(list[0], "mot4");
    QCOMPARE(list[1], "mot5");
    QCOMPARE(list[2], "mot6");
    idx = w->list_model->index(0);
    QCOMPARE(w->list_model->data(idx).toString(), "mot4");
    idx = w->list_model->index(1);
    QCOMPARE(w->list_model->data(idx).toString(), "mot5");
    idx = w->list_model->index(2);
    QCOMPARE(w->list_model->data(idx).toString(), "mot6");
    db.close();
}

void TestKeywordsWindow::test_synchronize_db()
{
    QStringList list1, list2;
    list1 << "mot4" << "mot5" << "mot6";
    list2 << "mot1" << "mot2" << "mot3";
    KeyWordsWindow::synchronize_db(list2);
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery q(db);
    QStringList list;
    q.exec("select mot from mot_clé;");
    q.next();
    list << q.value(0).toString();
    q.next();
    list << q.value(0).toString();
    q.next();
    list << q.value(0).toString();
    list.sort();
    QCOMPARE(list[0], "mot1");
    QCOMPARE(list[1], "mot2");
    QCOMPARE(list[2], "mot3");
    db.close();

    KeyWordsWindow::synchronize_db(list1, list2);
    list.clear();
    db.open();
    q.exec("select mot from mot_clé;");
    q.next();
    list << q.value(0).toString();
    q.next();
    list << q.value(0).toString();
    q.next();
    list << q.value(0).toString();
    list.sort();
    QCOMPARE(list[0], "mot4");
    QCOMPARE(list[1], "mot5");
    QCOMPARE(list[2], "mot6");
    db.close();
}

void TestKeywordsWindow::test_load_kw_from_file()
{
    QStringList list;
    QCOMPARE(KeyWordsWindow::load_kw_from_file(list, "wrong"), -1);
    QCOMPARE(KeyWordsWindow::load_kw_from_file(list, "keywords.test"), 0);
    QCOMPARE(list[0], "mot1");
    QCOMPARE(list[1], "mot2");
    QCOMPARE(list[2], "mot3");
}

void TestKeywordsWindow::test_load_kw_from_db()
{
    QStringList list;
    QSqlDatabase db = QSqlDatabase::database();
    db.setHostName("wrong");
    QCOMPARE(KeyWordsWindow::load_kw_from_db(list), -1);
    db.setHostName("localhost");
    QCOMPARE(KeyWordsWindow::load_kw_from_db(list), 0);
    list.sort();
    QCOMPARE(list[0], "mot4");
    QCOMPARE(list[1], "mot5");
    QCOMPARE(list[2], "mot6");
}

void TestKeywordsWindow::test_add_kw()
{
    KeyWordsWindow w;
    w.add_kw();
    QModelIndex idx = w.list_model->index(0);
    QCOMPARE(w.list_model->data(idx).toString(), "Nouveau mot-clé");
}

void TestKeywordsWindow::test_suppr_kw()
{
    KeyWordsWindow w;
    w.add_kw();
    w.view->setCurrentIndex(w.list_model->index(0));
    w.suppr_kw();
    QCOMPARE(w.list_model->rowCount(), 0);
}

void TestKeywordsWindow::test_save_kws()
{
    KeyWordsWindow w;
    QStringList list1, list2;
    list1 << "mot1" << "mot2" << "mot3";
    w.list_model->setStringList(list1);
    w.save_kws();
    QFile f(KW_FILENAME);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&f);
    while (! stream.atEnd())
    {
        list2 << stream.readLine();
    }
    QCOMPARE(list1, list2);
}

void TestKeywordsWindow::cleanupTestCase()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery q(db);
    q.exec("truncate type, mot_clé, messages, contient, possede;");
    db.close();
    QFile f("keywords.test");
    f.remove();
    f.setFileName("keywords2.test");
    f.remove();
    f.setFileName(KW_FILENAME);
    f.remove();
}


void TestKeywordsWindow::close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Enter);
}

void TestKeywordsWindow::tab_close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Tab);
    QTest::keyClick(amw, Qt::Key_Enter);
}

QTEST_MAIN(TestKeywordsWindow)

//#include "tst_testpatternwindow.moc"
