#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <scrapertab.h>
#include <dbtab.h>
#include <analysertab.h>

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QTabWidget *tabs;
    ScraperTab *scraper_tab;
    AnalyserTab *analyser_tab;
    DBTab *db_tab;

public:
    MainWindow();
    ~MainWindow();


};

#endif // MAINWINDOW_H
