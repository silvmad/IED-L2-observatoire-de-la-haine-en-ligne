#ifndef TESTconflist_H
#define TESTconflist_H

#include <QObject>
#include "conflist.h"

class TestConfList : public QObject
{
    Q_OBJECT

private slots:
    void initTestCase();
    void test_constr();
    void test_write();
    void test_get_and_set_value();
    void cleanupTestCase();
};

#endif // TESTconflist_H
