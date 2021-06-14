#include <QtTest/QTest>
#include "testanalyseur.h"
#include "main.h"

void TestAnalyseur::initTestCase()
{
    // Création du fichier de configuration de test.
    QFile file("conf.test");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible de créer le fichier de configuration de test.";
        return;
    }
    QTextStream stream(&file);
    stream << DB_HOST << "\t" << "localhost" << endl;
    stream << DB_USERNAME << "\t" << "haine_obs_test_user" << endl;
    stream << DB_USER_PW << "\t" << "haine_obs_test_mdp" << endl;
    stream << DB_DBNAME << "\t" << "haine_obs_test" << endl;
    file.close();

    conf = ConfList("conf.test");
    QSqlDatabase test_db = QSqlDatabase::addDatabase("QPSQL", "test");
    test_db.setHostName(conf.getValue(DB_HOST));
    test_db.setUserName(conf.getValue(DB_USERNAME));
    test_db.setPassword(conf.getValue(DB_USER_PW));
    test_db.setDatabaseName(conf.getValue(DB_DBNAME));
    if (!test_db.open())
    {
        qDebug() << "Échec de la connexion à la bdd de test.";
        exit(1);
    }
    QSqlQuery q(test_db);
    q.exec("insert into mot_clé values (1, 'mot1');");
    q.exec("insert into mot_clé values (2, 'mot2');");
    q.exec("insert into mot_clé values (3, 'mot3');");
    q.exec("insert into type values (1, 'type1');");
    q.exec("insert into type values (2, 'type2');");
    q.exec("insert into messages values (1, 'msg1 pat1 mot1', 'date1');");
    q.exec("insert into messages values (2, 'msg2 pat2 mot2', 'date2');");
    q.exec("insert into messages values (3, 'msg3 pat3 mot3', 'date3');");
    q.exec("insert into messages values (4, 'msg4 0pat 0mot', 'date4');");
    test_db.close();

    // Création du fichier de patrons de test.
    file.setFileName("patterns.test");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible de créer le fichier de patrons de test.";
        return;
    }
    stream << "pat1" << "\t" << "1" << endl;
    stream << "pat2" << "\t" << "1" << endl;
    stream << "pat3" << "\t" << "2" << endl;
    file.close();

    // Création du fichier de mots-clés de test.
    file.setFileName("keywords.test");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible de créer le fichier de mots-clés de test.";
        return;
    }
    stream << "mot1" << endl;
    stream << "mot2" << endl;
    stream << "mot3" << endl;
    file.close();
}

void TestAnalyseur::test_load_patterns()
{
    QMap<int, QStringList> map;
    load_patterns(map, "patterns.test");
    QCOMPARE(map.value(1)[0], "pat1");
    QCOMPARE(map.value(1)[1], "pat2");
    QCOMPARE(map.value(2)[0], "pat3");
}

void TestAnalyseur::test_add_pattern()
{
    QMap<int, QStringList> map;
    load_patterns(map, "patterns.test");
    add_pattern(map, "new1", 1);
    QCOMPARE(map.value(1)[2], "new1");
    add_pattern(map, "new2", 3);
    QCOMPARE(map.value(3)[0], "new2");
}

void TestAnalyseur::test_open_db()
{
    ConfList c("conf.test");
    {
        QSqlDatabase db = open_db(c);
        QCOMPARE(db.driverName(), "QPSQL");
        QVERIFY(db.isOpen());
        QCOMPARE(db.hostName(), c.getValue(DB_HOST));
        QCOMPARE(db.userName(), c.getValue(DB_USERNAME));
        QCOMPARE(db.password(), c.getValue(DB_USER_PW));
        QCOMPARE(db.databaseName(), c.getValue(DB_DBNAME));
        db.close();
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
    c.setValue(DB_USER_PW, "wrong");
    {
        QSqlDatabase db = open_db(c);
        QVERIFY(!db.isOpen());
        db.close();
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void TestAnalyseur::test_load_keywords()
{
    ConfList c("conf.test");
    {
        QSqlDatabase db = open_db(c);
        QList<keyword> list;
        load_keywords(list, "keywords.test");
        QCOMPARE(list[0].word, QString("mot1"));
        QCOMPARE(list[1].word, QString("mot2"));
        QCOMPARE(list[2].word, QString("mot3"));
        QCOMPARE(list[0].id, 1);
        QCOMPARE(list[1].id, 2);
        QCOMPARE(list[2].id, 3);
        db.close();
    }
    QSqlDatabase::removeDatabase("qt_sql_default_connection");
}

void TestAnalyseur::test_parse_results()
{
    QSqlDatabase test_db = QSqlDatabase::addDatabase("QPSQL");
    test_db.setHostName(conf.getValue(DB_HOST));
    test_db.setUserName(conf.getValue(DB_USERNAME));
    test_db.setPassword(conf.getValue(DB_USER_PW));
    test_db.setDatabaseName(conf.getValue(DB_DBNAME));
    test_db.open();
    QSqlQuery q;
    q.exec("select id, contenu from messages where haineux is null order by id;");
    QMap<int, QStringList> map;
    map.insert(1, QStringList() << "pat1" << "pat2");
    map.insert(2, QStringList() << "pat3");
    QList<keyword> list;
    keyword k;
    k.id = 1;
    k.word = "mot1";
    list << k;
    k.id = 2;
    k.word = "mot2";
    list << k;
    k.id = 3;
    k.word = "mot3";
    list << k;
    parse_result(q, map, list);
    q.exec("select * from messages;");
    q.next();
    QCOMPARE(q.value(3).toString(), "true");
    test_db.close();
}

void TestAnalyseur::test_match()
{

}

void TestAnalyseur::test_set_hateful()
{
    QSqlDatabase test_db = QSqlDatabase::database();
    test_db.open();
    QSqlQuery q("insert into messages values (5, 'msg5', 'date5');");
    set_hateful("5", "true");
    q.exec("select haineux from messages where id = 5");
    q.next();
    QCOMPARE(q.value(0).toString(), "true");
    set_hateful("5", "false");
    q.exec("select haineux from messages where id = 5");
    q.next();
    QCOMPARE(q.value(0).toString(), "false");
    test_db.close();
}

void TestAnalyseur::test_set_type()
{
    QSqlDatabase test_db = QSqlDatabase::database();
    test_db.open();
    set_type("5", 1);
    QSqlQuery q("select id_type from possede where id = 5;");
    q.next();
    QCOMPARE(q.value(0).toString(), "1");
    test_db.close();
}

void TestAnalyseur::test_set_keyword()
{
    QSqlDatabase test_db = QSqlDatabase::database();
    test_db.open();
    set_keyword("5", 1);
    QSqlQuery q("select id_mot_clé from contient where id = 5;");
    q.next();
    QCOMPARE(q.value(0).toString(), "1");
    test_db.close();
}

void TestAnalyseur::cleanupTestCase()
{
    QSqlDatabase test_db = QSqlDatabase::database("test");
    test_db.open();
    QSqlQuery q(test_db);
    q.exec("truncate type, mot_clé, messages, contient, possede;");
    test_db.close();
    QFile file("conf.test");
    file.remove();
    file.setFileName("patterns.test");
    file.remove();
    file.setFileName("keywords.test");
    file.remove();
}

QTEST_APPLESS_MAIN(TestAnalyseur)

