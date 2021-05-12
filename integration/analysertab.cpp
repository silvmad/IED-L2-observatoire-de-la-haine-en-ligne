#include "analysertab.h"
#include "analyserconfwindow.h"

AnalyserTab::AnalyserTab(QWidget *parent) : QWidget(parent)
{
    gen_layout = new QVBoxLayout(this);
    top_label = new QLabel("Texte du haut.");
    top_button_layout = new QHBoxLayout;
    conf_button = new QPushButton("Configuration");
    //hline = new QFrame;
    bot_label = new QLabel("Texte du bas.");
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
}

void AnalyserTab::open_conf_window()
{
    AnalyserConfWindow *win = new AnalyserConfWindow;
    win->show();
}
