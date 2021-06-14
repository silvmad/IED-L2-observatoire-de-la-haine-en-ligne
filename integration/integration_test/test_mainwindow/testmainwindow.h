#ifndef TESTMAINWINDOW_H
#define TESTMAINWINDOW_H
#include <QObject>

class TestMainWindow : public QObject
{
    Q_OBJECT
public slots:
    void close_messagebox();
    void tab_close_messagebox();

private slots:
    void initTestCase();
    void test_verify_kw_sync();
    void cleanupTestCase();
};

#endif // TESTMAINWINDOW_H
