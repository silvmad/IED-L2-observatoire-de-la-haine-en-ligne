#include <QtTest>
#include "testanalyserconfwindow.h"
#include "analyserconfwindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>

void TestAnalyserConfWindow::initTestCase()
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName("localhost");
    db.setUserName("haine_obs_test_user");
    db.setPassword("haine_obs_test_mdp");
    db.setDatabaseName("haine_obs_test");
    if (!db.open())
    {
        qDebug() << "Échec de la connexion à la bdd de test.";
        exit(1);
    }
    QSqlQuery q(db);
    q.exec("insert into type values (1, 'type1');");
    q.exec("insert into type values (2, 'type2');");
    q.exec("insert into type values (3, 'type3');");
    q.exec("insert into messages (id, contenu, date) values (1, 'msg1pat1', 'date1');");
    q.exec("insert into messages (id, contenu, date) values (2, 'msg2pat2', 'date2');");
    q.exec("insert into messages (id, contenu, date) values (3, 'msg3pat3', 'date3');");
    q.exec("insert into messages (id, contenu, date) values (4, 'msg4pat1pat3', 'date4');");
    q.exec("insert into messages (id, contenu, date) values (5, 'msg5', 'date5');");
    q.exec("insert into messages (id, contenu, date) values (6, 'msg6', 'date6');");
    db.close();
    QFile f(PATTERNS_FILENAME);
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&f);
    stream << "pat1\t1\n" << "pat2\t1\n" << "pat3\t2\n" << "pat4\t4\n" << "pat5\n" << "wrong\twrong\n";
    f.close();
}

void TestAnalyserConfWindow::test_load_types()
{
    AnalyserConfWindow w;
    w.load_types();
    QCOMPARE(w.type_id_list[0], 1);
    QCOMPARE(w.type_id_list[1], 2);
    QCOMPARE(w.type_id_list[2], 3);
    QCOMPARE(w.type_name_list[0], "type1");
    QCOMPARE(w.type_name_list[1], "type2");
    QCOMPARE(w.type_name_list[2], "type3");
}

void TestAnalyserConfWindow::test_load_patterns()
{
    AnalyserConfWindow w;
    w.patterns_model->clear();
    w.load_patterns(PATTERNS_FILENAME);
    QModelIndex idx = w.patterns_model->index(0, 0);
    QCOMPARE(w.patterns_model->data(idx), "pat1");
    idx = w.patterns_model->index(0, 1);
    QCOMPARE(w.patterns_model->data(idx), "type1");
    idx = w.patterns_model->index(1, 0);
    QCOMPARE(w.patterns_model->data(idx), "pat2");
    idx = w.patterns_model->index(1, 1);
    QCOMPARE(w.patterns_model->data(idx), "type1");
    idx = w.patterns_model->index(2, 0);
    QCOMPARE(w.patterns_model->data(idx), "pat3");
    idx = w.patterns_model->index(2, 1);
    QCOMPARE(w.patterns_model->data(idx), "type2");
    idx = w.patterns_model->index(3, 0);
    QCOMPARE(w.patterns_model->data(idx), "pat4");
    idx = w.patterns_model->index(3, 1);
    QCOMPARE(w.patterns_model->data(idx), "");
    idx = w.patterns_model->index(4, 0);
    QCOMPARE(w.patterns_model->data(idx), "pat5");
    idx = w.patterns_model->index(4, 1);
    QCOMPARE(w.patterns_model->data(idx).toString(), "");
    QCOMPARE(w.patterns_model->rowCount(), 5);
}

void TestAnalyserConfWindow::test_load_results()
{
    AnalyserConfWindow w;
    w.results_model->clear();
    w.load_results();
    QStringList msg_list;
    QModelIndex idx;
    for (int i = 0; i < 6; ++i)
    {
        idx = w.results_model->index(i, 0);
        msg_list << w.results_model->data(idx).toString();
        idx = w.results_model->index(i, 1);
        QCOMPARE(w.results_model->data(idx), "");
    }
    msg_list.sort();
    QCOMPARE(msg_list[0], "msg1pat1");
    QCOMPARE(msg_list[1], "msg2pat2");
    QCOMPARE(msg_list[2], "msg3pat3");
    QCOMPARE(msg_list[3], "msg4pat1pat3");
    QCOMPARE(msg_list[4], "msg5");
    QCOMPARE(msg_list[5], "msg6");
}

/* La difficulté ici est que les résultats sont mis dans le modèle dans un
 * ordre aléatoire. On crée donc une structure qui contient deux QString
 * pour lier message et type et on le trie en prenant en compte uniquement
 * le message avec une lambda pour pouvoir faire les comparaisons.
 * Solution pour le tri trouvée ici :
 * https://stackoverflow.com/questions/21233597/sorting-a-list-of-structs
 */
void TestAnalyserConfWindow::test_analyze_results()
{
    AnalyserConfWindow w;
    w.results_model->clear();
    w.load_results();
    w.analyze_results();
    QModelIndex idx;
    struct result { QString msg; QString types;} res;
    QList<struct result> list;
    QStringList list1, list2;
    for (int i = 0; i < 6; ++i)
    {
        idx = w.results_model->index(i, 0);
        list1 << w.results_model->data(idx).toString();
        res.msg = w.results_model->data(idx).toString();
        idx = w.results_model->index(i, 1);
        list2 << w.results_model->data(idx).toString();
        res.types = w.results_model->data(idx).toString();
        list << res;
    }
    std::sort(list.begin(), list.end(),
        [](const result& a, const result& b){
        return a.msg < b.msg;
    } );
    list1.sort();
    list2.sort();
    QCOMPARE(list1[0], "msg1pat1");
    QCOMPARE(list1[1], "msg2pat2");
    QCOMPARE(list1[2], "msg3pat3");
    QCOMPARE(list1[3], "msg4pat1pat3");
    QCOMPARE(list1[4], "msg5");
    QCOMPARE(list1[5], "msg6");
    QCOMPARE(list[0].types, "type1");
    QCOMPARE(list[1].types, "type1");
    QCOMPARE(list[2].types, "type2");
    QCOMPARE(list[3].types, "type1, type2");
    QCOMPARE(list[4].types, "");
    QCOMPARE(list[5].types, "");
}

void TestAnalyserConfWindow::test_add_pattern()
{
    AnalyserConfWindow w;
    int rc = w.patterns_model->rowCount();
    w.add_pattern();
    QCOMPARE(w.patterns_model->rowCount(), rc + 1);
    QModelIndex idx = w.patterns_model->index(rc, 0);
    QCOMPARE(w.patterns_model->data(idx), "Nouveau patron");
    idx = w.patterns_model->index(rc, 1);
    QCOMPARE(w.patterns_model->data(idx), "");
}

void TestAnalyserConfWindow::test_suppr_pattern()
{
    AnalyserConfWindow w;
    int rc = w.patterns_model->rowCount();
    // Si pas de sélection : pas d'effet
    w.suppr_pattern();
    QCOMPARE(w.patterns_model->rowCount(), rc);

    QModelIndex idx = w.patterns_model->index(rc - 2, 0);
    QString second_last_pat = w.patterns_model->data(idx).toString();
    idx = w.patterns_model->index(rc - 2, 1);
    QString second_last_type = w.patterns_model->data(idx).toString();
    idx = w.patterns_model->index(rc - 1, 0);
    w.patterns_view->setCurrentIndex(idx);

    // Clic sur non : pas d'effet.
    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    w.suppr_pattern();
    QCOMPARE(w.patterns_model->rowCount(), rc);

    // Clic sur oui : suppression de la ligne courante.
    QTimer::singleShot(150, this, SLOT(tab_close_messagebox()));
    w.suppr_pattern();
    QCOMPARE(w.patterns_model->rowCount(), rc - 1);
    rc = w.patterns_model->rowCount();
    /* L'avant dernière ligne est toujours la même : c'est bien la ligne
     * sélectionnée (la dernière ici) qui a été supprimée. */
    idx = w.patterns_model->index(rc - 1, 0);
    QCOMPARE(second_last_pat, w.patterns_model->data(idx).toString());
    idx = w.patterns_model->index(rc - 1, 1);
    QCOMPARE(second_last_type, w.patterns_model->data(idx).toString());
}

void TestAnalyserConfWindow::test_save_patterns()
{
    AnalyserConfWindow w;
    w.save_patterns();
    QFile f(PATTERNS_FILENAME);
    f.open(QIODevice::ReadOnly | QIODevice::Text);
    QTextStream stream(&f);
    QStringList list;
    while (!stream.atEnd())
    {
        list << stream.readLine();
    }
    QCOMPARE(list[0], "pat1\t1");
    QCOMPARE(list[1], "pat2\t1");
    QCOMPARE(list[2], "pat3\t2");
    QCOMPARE(list[3], "pat4");
    QCOMPARE(list[4], "pat5");
}

void TestAnalyserConfWindow::test_show_patterns_by_type()
{
    AnalyserConfWindow w;
    w.pt_select_type->setCurrentIndex(1);
    QVERIFY(!w.patterns_view->isRowHidden(0));
    QVERIFY(!w.patterns_view->isRowHidden(1));
    QVERIFY(w.patterns_view->isRowHidden(2));
    QVERIFY(w.patterns_view->isRowHidden(3));
    QVERIFY(w.patterns_view->isRowHidden(4));
    w.pt_select_type->setCurrentText("Tous les types");
    for (int i = 0; i < 5; ++i)
    {
        QVERIFY(!w.patterns_view->isRowHidden(i));
    }
}

void TestAnalyserConfWindow::cleanupTestCase()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery q(db);
    q.exec("truncate type, mot_clé, messages, contient, possede;");
    db.close();
    QFile f(PATTERNS_FILENAME);
    f.remove();
}

void TestAnalyserConfWindow::close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Enter);
}

void TestAnalyserConfWindow::tab_close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Tab);
    QTest::keyClick(amw, Qt::Key_Enter);
}

QTEST_MAIN(TestAnalyserConfWindow)

