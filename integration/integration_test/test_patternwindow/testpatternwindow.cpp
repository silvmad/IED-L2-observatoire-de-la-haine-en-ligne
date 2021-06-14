#include "testpatternwindow.h"
#include "analyserconfwindow.h"

#include <QMessageBox>

void TestPatternWindow::initTestCase()
{
    parent = new QWidget;
    model = new QStandardItemModel;
    model->setColumnCount(2);
    model->insertRow(0);
    model->setItem(0, 0, new QStandardItem("pat1"));
    model->setItem(0, 1, new QStandardItem("type1"));
    model->setItem(1, 0, new QStandardItem("pat2"));
    model->setItem(1, 1, new QStandardItem(""));
    list << "type1" << "type2";
}

void TestPatternWindow::test_PatternWindow()
{
    QTest::ignoreMessage(QtWarningMsg, "QObject::connect: No such slot QWidget::modify_pattern(int, QString, QString)");// in ../../../../IED-L2-observatoire-de-la-haine-en-ligne/integration/integration_src/patternwindow.cpp:48");
    w = new PatternWindow(parent, model, list, 0);
    QCOMPARE(w->types_box->currentText(), "type1");
    QTest::ignoreMessage(QtWarningMsg, "QObject::connect: No such slot QWidget::modify_pattern(int, QString, QString)");// in ../../../../IED-L2-observatoire-de-la-haine-en-ligne/integration/integration_src/patternwindow.cpp:48");
    w = new PatternWindow(parent, model, list, 1);
    QCOMPARE(w->types_box->currentText(), "Choisir un type");
}

void TestPatternWindow::test_validate_clicked()
{
    QTest::ignoreMessage(QtWarningMsg, "QObject::connect: No such slot QWidget::modify_pattern(int, QString, QString)");// in ../../../../IED-L2-observatoire-de-la-haine-en-ligne/integration/integration_src/patternwindow.cpp:48");
    w = new PatternWindow(parent, model, list, 1);
    w->setModal(false);
    QSignalSpy sig(w,  SIGNAL(pattern_modification(int, QString, QString)));
    QTimer::singleShot(50, this, SLOT(close_messagebox()));
    QTest::mouseClick(w->validate_but, Qt::LeftButton);

    QCOMPARE(sig.count(), 0);

    w->types_box->setCurrentText("type1");
    QTest::mouseClick(w->validate_but, Qt::LeftButton);
    QCOMPARE(sig.count(), 1);


}

void TestPatternWindow::close_messagebox()
{
    QWidget *amw = QApplication::activeModalWidget();
    QTest::keyClick(amw, Qt::Key_Enter);
}

QTEST_MAIN(TestPatternWindow)
