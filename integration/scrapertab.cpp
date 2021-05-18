#include "scrapertab.h"
#include "scraperconfwindow.h"
#include "keywordswindow.h"

ScraperTab::ScraperTab(QWidget *parent) : QWidget(parent)
{
    main_layout = new QVBoxLayout(this);

    top_label = new QLabel("Description du scraper");
    top_label->setFrameStyle(QFrame::Box);

    top_but_lay = new QHBoxLayout;
    conf_but = new QPushButton("Configuration");
    kw_but = new QPushButton("Mots-clés");
    top_but_lay->addWidget(conf_but);
    top_but_lay->addWidget(kw_but);
    top_but_lay->setAlignment(Qt::AlignRight);

    bot_label = new QLabel("Le scraper est prêt à être lancé.");
    bot_label->setFrameStyle(QFrame::Box);

    bot_but_lay = new QHBoxLayout;
    start_but = new QPushButton("Démarrer");
    stop_but = new QPushButton("Arrêter");
    bot_but_lay->addWidget(start_but);
    bot_but_lay->addWidget(stop_but);
    bot_but_lay->setAlignment(Qt::AlignRight);

    main_layout->addWidget(top_label);
    main_layout->addLayout(top_but_lay);
    main_layout->addWidget(bot_label);
    main_layout->addLayout(bot_but_lay);

    QObject::connect(conf_but, SIGNAL(clicked()), this, SLOT(open_conf_win()));
    QObject::connect(kw_but, SIGNAL(clicked()), this, SLOT(open_kw_win()));
    QObject::connect(start_but, SIGNAL(clicked()), this->parent()->parent(), SLOT(start_scraper()));
    QObject::connect(stop_but, SIGNAL(clicked()), this->parent()->parent(), SLOT(stop_scraper()));
}

void ScraperTab::open_conf_win()
{
    ScraperConfWindow *w = new ScraperConfWindow;
    w->show();
}

void ScraperTab::open_kw_win()
{
    KeyWordsWindow *w = new KeyWordsWindow;
    w->show();
}

void ScraperTab::set_label_started()
{
    bot_label->setText("Le scraper est en cours d'exécution.");
}

void ScraperTab::set_label_stopped()
{
    bot_label->setText("Le scraper est arrêté.");
}
