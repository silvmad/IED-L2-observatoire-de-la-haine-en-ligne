#include <QtTest>
#include "testconflist.h"

void TestConfList::initTestCase()
{
    QFile file("conf.test");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible de créer le premier fichier de configuration de test.";
        return;
    }
    QTextStream stream(&file);
    stream << "A\t1" << endl;
    stream << "B\t2" << endl;
    stream << "C\t3" << endl;
    stream << "D4" << endl;
    stream << "D\t4\twrong" << endl;
    file.close();

    file.setFileName("conf2.test");
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << "Impossible de créer le deuxième fichier de configuration de test.";
        return;
    }
    file.close();
}

void TestConfList::test_constr()
{
    ConfList conf("wrong");
    QVERIFY(!conf.is_loaded());
    conf = ConfList("conf.test");
    QVERIFY(conf.is_loaded());
    QCOMPARE(conf.getValue("A"), "1");
    QCOMPARE(conf.getValue("B"), "2");
    QCOMPARE(conf.getValue("C"), "3");
    QCOMPARE(conf.getValue("D"), nullptr);
    QCOMPARE(conf.entries.size(), 3);
}

void TestConfList::test_write()
{
    ConfList conf("conf.test");
    conf.setValue("A", "3");
    conf.setValue("B", "1");
    conf.setValue("C", "2");
    conf.write("conf2.test");
    conf = ConfList("conf2.test");
    QCOMPARE(conf.getValue("A"), "3");
    QCOMPARE(conf.getValue("B"), "1");
    QCOMPARE(conf.getValue("C"), "2");
    QCOMPARE(conf.write("wrong"), -1);
}

void TestConfList::test_get_and_set_value()
{
    ConfList conf("conf.test");
    conf.setValue("A", "3");
    conf.setValue("B", "1");
    conf.setValue("C", "2");
    QCOMPARE(conf.getValue("A"), "3");
    QCOMPARE(conf.getValue("B"), "1");
    QCOMPARE(conf.getValue("C"), "2");
    QCOMPARE(conf.getValue("D"), nullptr);
    QCOMPARE(conf.setValue("D", "4"), -1);
    QCOMPARE(conf.setValue("A", "1"), 0);
}

void TestConfList::cleanupTestCase()
{
    QFile f("conf.test");
    f.remove();
    f.setFileName("conf2.test");
    f.remove();
}


QTEST_APPLESS_MAIN(TestConfList)
