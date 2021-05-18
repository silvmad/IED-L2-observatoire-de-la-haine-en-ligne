#include "analysertab.h"
#include "analyserconfwindow.h"

AnalyserTab::AnalyserTab(QWidget *parent) : QWidget(parent)
{
    gen_layout = new QVBoxLayout(this);
    top_label = new QLabel("Texte du haut.");
    top_button_layout = new QHBoxLayout;
    conf_button = new QPushButton("Configuration");
    //hline = new QFrame;
    bot_label = new QLabel("L'analyseur est prêt à être lancé.");
    bot_buttons_layout = new QHBoxLayout;
    run_button = new QPushButton("Lancer l'analyse");
    stop_button = new QPushButton("Stopper l'analyse");

    top_label->setFrameStyle(QFrame::Box);
    bot_label->setFrameStyle(QFrame::Panel);

    top_button_layout->addWidget(conf_button);
    top_button_layout->setAlignment(Qt::AlignRight);

    //hline->setFrameStyle(QFrame::HLine);

    bot_buttons_layout->addWidget(stop_button);
    bot_buttons_layout->addWidget(run_button);
    bot_buttons_layout->setAlignment(Qt::AlignRight);

    gen_layout->addWidget(top_label);
    gen_layout->addLayout(top_button_layout);
    //gen_layout->addWidget(hline);
    gen_layout->addWidget(bot_label);
    gen_layout->addLayout(bot_buttons_layout);

    QObject::connect(conf_button, SIGNAL(clicked()), this, SLOT(open_conf_window()));
    QObject::connect(run_button, SIGNAL(clicked()), this->parent()->parent(), SLOT(start_analyser()));
    QObject::connect(stop_button, SIGNAL(clicked()), this->parent()->parent(), SLOT(stop_analyser()));
}

void AnalyserTab::open_conf_window()
{
    AnalyserConfWindow *win = new AnalyserConfWindow;
    win->show();
}

void AnalyserTab::set_label_started()
{
    bot_label->setText("L'analyseur est en cours d'exécution.");
}

void AnalyserTab::set_label_stopped()
{
    bot_label->setText("L'analyseur est arrêté.");
}
