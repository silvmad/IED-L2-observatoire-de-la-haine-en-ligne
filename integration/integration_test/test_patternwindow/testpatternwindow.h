#ifndef TESTPATTERNWINDOW_H
#define TESTPATTERNWINDOW_H

#include <QtTest>
#include <QObject>
#include <QtSql>

#include "patternwindow.h"
#include "analyserconfwindow.h"
#include "conflist.h"

class TestPatternWindow : public QObject
{
    Q_OBJECT

public slots:
    void close_messagebox();

private slots:
    void initTestCase();
    void test_PatternWindow();
    void test_validate_clicked();

private:
    QWidget *parent;
    QStandardItemModel *model;
    QStringList list;
    PatternWindow *w;
};


#endif // TESTPATTERNWINDOW_H
