#include "mainwindow.h"
#include <csignal>

MainWindow::MainWindow()
{
    setWindowTitle("Observatoire de la haine en ligne");

    scraper_process = new QProcess();
    scraper_process->setProgram(SCRAPER_PATH);
    analyser_process = new QProcess();
    analyser_process->setProgram(ANALYZER_PATH);

    tabs = new QTabWidget(this);
    scraper_tab = new ScraperTab(tabs);
    analyser_tab = new AnalyserTab(tabs);
    db_tab = new DBTab(tabs);
    tabs->addTab(scraper_tab, "Scraper");
    tabs->addTab(analyser_tab, "Analyseur");
    tabs->addTab(db_tab, "Base de données");
    this->setCentralWidget(tabs);

    QObject::connect(scraper_process, SIGNAL(finished(int, QProcess::ExitStatus)),
                     this, SLOT(scraper_end_handler(int, QProcess::ExitStatus)));
    QObject::connect(analyser_process, SIGNAL(finished(int, QProcess::ExitStatus)),
                     this, SLOT(analyser_end_handler(int, QProcess::ExitStatus)));
    QObject::connect(scraper_process, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(read_scraper_stdout()));

}

void MainWindow::start_scraper()
{
    scraper_tab->set_label_started();
    scraper_process->start();
}

void MainWindow::start_analyser()
{
    analyser_tab->set_label_started();
    analyser_process->start();
}

void MainWindow::stop_scraper()
{
    scraper_tab->set_label_stopped();
    scraper_process->kill();
}

void MainWindow::stop_analyser()
{
    analyser_tab->set_label_stopped();
    analyser_process->kill();
}

void MainWindow::scraper_end_handler(int code, QProcess::ExitStatus s)
{
    if (s == QProcess::CrashExit)
    {
        if (scraper_process->error() != QProcess::UnknownError)
        {
            QMessageBox::critical(this, "Erreur", "Le scraper a recontré un problème et s'est arrêté.");
        }
    }
    else
    {
        switch (code)
        {
        case PT_FILE_ERR :
            QMessageBox::critical(this, "Erreur", "Le scraper n'arrive pas à ouvrir le fichier des patrons.");
            break;
        case KW_FILE_ERR :
            QMessageBox::critical(this, "Erreur", "Le scraper n'arrive pas à ouvrir le fichier des mots-clés.");
            break;
        case DB_ERR :
            QMessageBox::critical(this, "Erreur", "Le scraper n'arrive pas à se connecter à la base de données.");
            break;
        }
    }
}

void MainWindow::analyser_end_handler(int code, QProcess::ExitStatus s)
{
    if (s == QProcess::CrashExit)
    {
        if (analyser_process->error() != QProcess::UnknownError)
        {
            QMessageBox::critical(this, "Erreur", "L'analyseur a recontré un problème et s'est arrêté.");
        }
    }
    else
    {
        switch (code)
        {
        case PT_FILE_ERR :
            QMessageBox::critical(this, "Erreur", "L'analyseur n'arrive pas à ouvrir le fichier des patrons.");
            break;
        case KW_FILE_ERR :
            QMessageBox::critical(this, "Erreur", "L'analyseur n'arrive pas à ouvrir le fichier des mots-clés.");
            break;
        case DB_ERR :
            QMessageBox::critical(this, "Erreur", "L'analyseur n'arrive pas à se connecter à la base de données.");
            break;
        }
    }
}

void MainWindow::read_scraper_stdout()
{
    QString msg = scraper_process->readAllStandardOutput();
    if (msg == "ADDED_DB_DATA" && analyser_tab)
    {
        kill(qvariant_cast<int>(analyser_process->pid()), SIGCONT);
    }
}

void MainWindow::closeEvent(QCloseEvent *e)
{
    scraper_process->kill();
    analyser_process->kill();
    QMainWindow::closeEvent(e);
}
