#include "mainwindow.h"

MainWindow::MainWindow()
{
    tabs = new QTabWidget;
    scraper_tab = new ScraperTab;
    analyser_tab = new AnalyserTab;
    db_tab = new DBTab;
    tabs->addTab(scraper_tab, "Scraper");
    tabs->addTab(analyser_tab, "Analyseur");
    tabs->addTab(db_tab, "Base de données");
    this->setCentralWidget(tabs);
    setGeometry(500,500,500,500);
}

MainWindow::~MainWindow()
{

}
