#ifndef TESTTYPESWINDOW_H
#define TESTTYPESWINDOW_H
#include <QObject>
#include "typeswindow.h"

class TestTypesWindow : public QObject
{
    Q_OBJECT
private:
    AnalyserConfWindow *w;

private slots:
    void initTestCase();
    void test_add_type();
    void test_suppr_type();
    void test_save_types();

    void cleanupTestCase();

public slots:
    void close_messagebox();
};

#endif // TESTTYPESWINDOW_H
