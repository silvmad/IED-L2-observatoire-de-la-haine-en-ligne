#include "scrapertab.h"
#include "scraperconfwindow.h"
#include "keywordswindow.h"

ScraperTab::ScraperTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    QLabel *top_label = new QLabel("Le scraper permet de récupérer sur twitter des messages contenant des mots-clés.\n\n"
                                   "Pour cela il doit se connecter à l'api de twitter, ce qui nécessite d'avoir un projet validé sur https://developer.twitter.com/en");
    top_label->setFrameStyle(QFrame::Box);
    top_label->setAlignment(Qt::AlignTop);
    top_label->setWordWrap(true);

    QHBoxLayout *top_but_lay = new QHBoxLayout;
    QPushButton *conf_but = new QPushButton("Configuration");
    QPushButton *kw_but = new QPushButton("Mots-clés");
    top_but_lay->addWidget(conf_but);
    top_but_lay->addWidget(kw_but);
    top_but_lay->setAlignment(Qt::AlignRight);

    bot_label = new QLabel("  Le scraper est prêt à être lancé.");
    bot_label->setFrameStyle(QFrame::Box);
    bot_label->setFixedHeight(50);

    QHBoxLayout *bot_but_lay = new QHBoxLayout;
    QPushButton *start_but = new QPushButton("Démarrer");
    QPushButton *stop_but = new QPushButton("Arrêter");
    bot_but_lay->addWidget(start_but);
    bot_but_lay->addWidget(stop_but);
    bot_but_lay->setAlignment(Qt::AlignRight);

    main_layout->addWidget(top_label);
    main_layout->addLayout(top_but_lay);
    main_layout->addWidget(bot_label);
    main_layout->addLayout(bot_but_lay);

    QObject::connect(conf_but, SIGNAL(clicked()), this, SLOT(open_conf_win()));
    QObject::connect(kw_but, SIGNAL(clicked()), this, SLOT(open_kw_win()));
    QObject::connect(start_but, SIGNAL(clicked()), this->parent()->parent(),
                     SLOT(start_scraper()));
    QObject::connect(stop_but, SIGNAL(clicked()), this->parent()->parent(),
                     SLOT(stop_scraper()));
}

/* Slot qui ouvre la fenêtre de configuration du scraper. */
void ScraperTab::open_conf_win()
{
    ScraperConfWindow *w = new ScraperConfWindow;
    w->show();
}

/* Slot qui ouvre la fenêtre des mots-clés. */
void ScraperTab::open_kw_win()
{
    KeyWordsWindow *w = new KeyWordsWindow(this);
    w->show();
    w->load_keywords(KW_FILENAME);
}

/* Slot qui édite le label du bas pour signifier que le scraper est démarré. */
void ScraperTab::set_label_started()
{
    bot_label->setText("Le scraper est en cours d'exécution.");
}

/* Slot qui édite le label du bas pour signifier que le scraper est arrêté. */
void ScraperTab::set_label_stopped()
{
    bot_label->setText("Le scraper est arrêté.");
}
