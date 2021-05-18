#include <QtTest/QTest>
#include "testanalyseur.h"
#include "main.h"

/*TestAnalyseur::TestAnalyseur(QObject *parent) : QObject(parent)
{

}*/
void TestAnalyseur::test_load_patterns()
{
    QMap<int, QStringList> map;
    load_patterns(map, "patterns.test");
    QCOMPARE(map.value(1)[0], "value1");
    QCOMPARE(map.value(1)[1], "value2");
    QCOMPARE(map.value(2)[0], "value3");
    QVERIFY(false);
}

QTEST_APPLESS_MAIN(TestAnalyseur)

