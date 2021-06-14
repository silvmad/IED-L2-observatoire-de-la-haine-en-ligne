#include "mainwindow.h"
#include <csignal>
#include "keywordswindow.h"

MainWindow::MainWindow()
{
    setWindowTitle("Observatoire de la haine en ligne");

    scraper_process = new QProcess(this);
    scraper_process->setProgram("python3");
    scraper_process->setArguments(QStringList() << SCRAPER_PATH);
    analyser_process = new QProcess(this);
    analyser_process->setProgram(ANALYZER_PATH);

    QTabWidget *tabs = new QTabWidget(this);
    scraper_tab = new ScraperTab(tabs);
    analyser_tab = new AnalyserTab(tabs);
    DBTab *db_tab = new DBTab(tabs);
    tabs->addTab(scraper_tab, "Scraper");
    tabs->addTab(analyser_tab, "Analyseur");
    tabs->addTab(db_tab, "Base de données");
    this->setCentralWidget(tabs);

    QObject::connect(scraper_process, SIGNAL(finished(int, QProcess::ExitStatus)),
                     this, SLOT(scraper_end_handler(int)));
    QObject::connect(analyser_process, SIGNAL(finished(int, QProcess::ExitStatus)),
                     this, SLOT(analyser_end_handler(int)));
    QObject::connect(scraper_process, SIGNAL(readyReadStandardOutput()),
                     this, SLOT(read_scraper_stdout()));
}

/* Slot qui démarre le scraper et édite le texte du label correspondant dans
 * l'onglet scraper. */
void MainWindow::start_scraper()
{
    if (verify_kw_sync() < 0)
    {
        return;
    }
    scraper_tab->set_label_started();
    scraper_process->start();
}

/* Slot qui démarre l'analyseur et édite le texte du label correspondant dans
 * l'onglet analyseur. */
void MainWindow::start_analyser()
{
    if (verify_kw_sync() < 0)
    {
        return;
    }
    analyser_tab->set_label_started();
    analyser_process->start();
}

int MainWindow::verify_kw_sync()
{
    QStringList file_kw_list, db_kw_list;
    if (KeyWordsWindow::load_kw_from_file(file_kw_list, KW_FILENAME) < 0)
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier des mots-clés.");
        return -1;
    }
    if (KeyWordsWindow::load_kw_from_db(db_kw_list) < 0)
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'accéder à la base de données.");
        return -1;
    }
    db_kw_list.sort();
    file_kw_list.sort();
    if (db_kw_list != file_kw_list)
    {
        /* Si la table des mots-clés est vide, on la remplit silencieusement,
         * sinon on affiche un avertissement avant. */
        if (db_kw_list.size() > 0)
        {
            int ret = QMessageBox::warning(this, "Mise à jour de la base", "Les contenus du fichier mot-clé et de la base de données diffèrent.\n"
                                                                 "Le contenu de la base de données va donc être mis à jour.\n"
                                                                 "Voulez-vous continuer ?", QMessageBox::No | QMessageBox::Yes);
            if (ret == QMessageBox::No)
            {
                return 0;
            }
        }
        if (KeyWordsWindow::synchronize_db(file_kw_list, db_kw_list) < 0)
        {
            QMessageBox::critical(this, "Erreur", "Impossible d'accéder à la base de données.\n"
                                  "La synchronisation a échoué.");
            return -1;
        }
    }
    return 0;
}

/* Slot qui arrête le scraper et édite le texte du label correspondant dans
 * l'onglet scraper. */
void MainWindow::stop_scraper()
{
    scraper_tab->set_label_stopped();
    scraper_process->terminate();
}

/* Slot qui arrête l'analyseyr et édite le texte du label correspondant dans
 * l'onglet analyseur. */
void MainWindow::stop_analyser()
{
    analyser_tab->set_label_stopped();
    analyser_process->terminate();
}

/* Slot qui gère l'arrêt du scraper et affiche un message dans une fenêtre en
 * cas d'erreur. */
void MainWindow::scraper_end_handler(int code)
{
    switch (code)
    {
    case SC_API_ERR :
        QMessageBox::critical(this, "Erreur", "Le scraper n'arrive pas à se connecter à l'api twitter.");
        break;
    case SC_DB_ERR :
        QMessageBox::critical(this, "Erreur", "Le scraper n'arrive pas à se connecter à la base de données.");
        break;
    case SC_CONF_FILE_ERR :
        QMessageBox::critical(this, "Erreur", "Le scraper n'arrive pas à lire le fichier de configuration.");
        break;
    case SC_KW_FILE_ERR :
        QMessageBox::critical(this, "Erreur", "Le scraper n'arrive pas à lire le fichier des mots-clés.");
        break;
    case SC_RATE_ERR :
        QMessageBox::critical(this, "Erreur", "La configuration du scraper ne respecte pas les restrictions de twitter.");
        break;
    case SC_UNEXP_ERR :
        QMessageBox::critical(this, "Erreur", "Le scraper a rencontré une erreur et a quitté.");
        break;
    }
    scraper_tab->set_label_stopped();
}

/* Slot qui gère l'arrêt de l'analyseur et affiche un message dans une fenêtre en
 * cas d'erreur. */
void MainWindow::analyser_end_handler(int code)
{
    switch (code)
    {
    case AN_PT_FILE_ERR :
        QMessageBox::critical(this, "Erreur", "L'analyseur n'arrive pas à ouvrir le fichier des patrons.");
        break;
    case AN_KW_FILE_ERR :
        QMessageBox::critical(this, "Erreur", "L'analyseur n'arrive pas à ouvrir le fichier des mots-clés.");
        break;
    case AN_DB_ERR :
        QMessageBox::critical(this, "Erreur", "L'analyseur n'arrive pas à se connecter à la base de données.");
        break;
    case AN_CONF_FILE_ERR :
        QMessageBox::critical(this, "Erreur", "L'analyseur n'arrive pas à lire le fichier de configuration.");
        break;
    default :
        QMessageBox::critical(this, "Erreur", "L'analyseur a rencontré une erreur et a quitté.");
        break;
    }
    analyser_tab->set_label_stopped();
}

/* Slot qui lie la sortie standard du scraper et traite l'information.
 * Si le scraper informe qu'il a ajouté d'autres messages à la base de données,
 * un signal est envoyé à l'analyseur pour qu'il se remette en route.
 * Pour le moment il s'agit du seul message possible. Si d'autres sont ajoutés
 * ultérieurement, cette fonction devra être modifiée. */
void MainWindow::read_scraper_stdout()
{
    QString msg = scraper_process->readAllStandardOutput();
    if (msg.trimmed() == "205 new data available")
    {
        kill(qvariant_cast<int>(analyser_process->pid()), SIGCONT);
    }
}

/* Réécriture du slot closeEvent pour que le scraper et l'analyseur soient
 * arrêtés en cas de fermeture de la fenêtre afin d'éviter d'avoir des
 * processus orphelins. */
void MainWindow::closeEvent(QCloseEvent *e)
{
    scraper_process->terminate();
    analyser_process->terminate();
    QMainWindow::closeEvent(e);
}
