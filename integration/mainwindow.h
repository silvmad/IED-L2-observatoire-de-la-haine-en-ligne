#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QProcess>

#include <scrapertab.h>
#include <dbtab.h>
#include <analysertab.h>

#define SCRAPER_PATH "/home/victor/cours ied/L2/Projet/sources/IED-L2-observatoire-de-la-haine-en-ligne/scraper.py"
#define ANALYZER_PATH "/home/victor/cours ied/L2/Projet/sources/build/analyseur"

enum { PT_FILE_ERR = 1, KW_FILE_ERR, DB_ERR };

class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QProcess *scraper_process;
    QProcess *analyser_process;
    QTabWidget *tabs;
    ScraperTab *scraper_tab;
    AnalyserTab *analyser_tab;
    DBTab *db_tab;

public:
    MainWindow();

public slots:
    void start_scraper();
    void start_analyser();
    void stop_scraper();
    void stop_analyser();
    void scraper_end_handler(int, QProcess::ExitStatus);
    void analyser_end_handler(int, QProcess::ExitStatus);
    void read_scraper_stdout();
    void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
