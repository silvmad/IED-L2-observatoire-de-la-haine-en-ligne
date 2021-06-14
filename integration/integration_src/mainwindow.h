#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QLayout>
#include <QProcess>

#include "scrapertab.h"
#include "dbtab.h"
#include "analysertab.h"

#define SCRAPER_PATH "/home/victor/cours ied/L2/Projet/sources/IED-L2-observatoire-de-la-haine-en-ligne/scraper/scraper.py"
#define ANALYZER_PATH "/home/victor/cours ied/L2/Projet/sources/build/analyseur/analyseur_src/analyseur_src"

enum { AN_PT_FILE_ERR = 1, AN_KW_FILE_ERR, AN_DB_ERR, AN_CONF_FILE_ERR };
enum { SC_API_ERR = 1, SC_DB_ERR, SC_CONF_FILE_ERR, SC_KW_FILE_ERR, SC_RATE_ERR, SC_UNEXP_ERR};

/* La fenêtre principale de l'interface.
 * Elle comprend trois onglets pour respectivement le scraper, l'analyseur
 * et la base de données.
 * Elle possède également deux QProcess permettant de gérer l'exécution et
 * l'arrêt du scraper et l'analyseur. */
class MainWindow : public QMainWindow
{
    Q_OBJECT
private:
    QProcess *scraper_process;
    QProcess *analyser_process;
    ScraperTab *scraper_tab;
    AnalyserTab *analyser_tab;

public:
    MainWindow();
    int verify_kw_sync();

public slots:
    void start_scraper();
    void start_analyser();
    void stop_scraper();
    void stop_analyser();
    void scraper_end_handler(int);
    void analyser_end_handler(int);
    void read_scraper_stdout();
    void closeEvent(QCloseEvent*);
};

#endif // MAINWINDOW_H
