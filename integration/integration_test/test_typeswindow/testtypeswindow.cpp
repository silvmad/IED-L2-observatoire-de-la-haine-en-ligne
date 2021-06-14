#include <QtTest>
#include "testtypeswindow.h"
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QTimer>

void TestTypesWindow::initTestCase()
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
    q.exec("insert into type values (1, 'type1');");
    q.exec("insert into type values (2, 'type2');");
    q.exec("insert into type values (3, 'type3');");
    test_db.close();
    QFile f(PATTERNS_FILENAME);
    f.open(QIODevice::WriteOnly | QIODevice::Text);
    QTextStream stream(&f);
    stream << "pat1\t1\n" << "pat2\t1\n" << "pat3\t2\n";
    f.close();
    w = new AnalyserConfWindow;
}

void TestTypesWindow::test_add_type()
{
    TypesWindow *tw = new TypesWindow(w, w->patterns_model, w->type_name_list);
    tw->add_type();
    int rc = tw->types_model->rowCount();
    QModelIndex idx = tw->types_model->index(rc - 1);
    QCOMPARE(tw->types_model->data(idx).toString(), "Nouveau type");
}

void TestTypesWindow::test_suppr_type()
{
    TypesWindow *tw = new TypesWindow(w, w->patterns_model, w->type_name_list);
    tw->view->setCurrentIndex(tw->types_model->index(0));
    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    tw->suppr_type();
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery q(db);
    q.exec("select nom_type from type where id_type = 1;");
    q.next();
    QCOMPARE(q.value(0).toString(), "type1");
    db.close();
    QCOMPARE(tw->types_model->rowCount(), 3);
    tw->view->setCurrentIndex(tw->types_model->index(2));
    tw->suppr_type();
    QCOMPARE(tw->types_model->rowCount(), 2);
    QModelIndex idx = tw->types_model->index(0);
    QCOMPARE(tw->types_model->data(idx), "type1");
    idx = tw->types_model->index(1);
    QCOMPARE(tw->types_model->data(idx), "type2");
    db.open();
    q.exec("select * from type where id = 3;");
    QVERIFY(!q.next());
    q.exec("insert into types values (3, 'type3');");
    db.close();
}

void TestTypesWindow::test_save_types()
{
    TypesWindow *tw = new TypesWindow(w, w->patterns_model, w->type_name_list);
    QStringList list1, list2;
    list1 << "type4" << "type5" << "type6";
    tw->types_model->setStringList(list1);
    tw->save_types();
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery q(db);
    q.exec("select nom_type from type;");
    q.next();
    list2 << q.value(0).toString();
    q.next();
    list2 << q.value(0).toString();
    q.next();
    list2 << q.value(0).toString();
    list2.sort();
    QCOMPARE(list1, list2);
}

void TestTypesWindow::cleanupTestCase()
{
    QSqlDatabase db = QSqlDatabase::database();
    db.open();
    QSqlQuery q(db);
    q.exec("truncate type, mot_clé, messages, contient, possede;");
    db.close();
    QFile f(PATTERNS_FILENAME);
    f.remove();
}

void TestTypesWindow::close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Enter);
}

QTEST_MAIN(TestTypesWindow)
