#include "analysertab.h"
#include "analyserconfwindow.h"

AnalyserTab::AnalyserTab(QWidget *parent) : QWidget(parent)
{
    QVBoxLayout *gen_layout = new QVBoxLayout(this);
    QLabel *top_label = new QLabel("L'analyseur permet de déterminer si les messages de la base de données sont haineux et le type de haine concerné.\n\n"
                                   "Il utilise pour cela des expressions régulières associées à un type (patrons).\n\n"
                                   "La fenêtre de configuration permet de gérer et de tester les patrons.");
    top_label->setWordWrap(true);
    top_label->setAlignment(Qt::AlignTop | Qt::AlignLeft);
    QHBoxLayout *top_button_layout = new QHBoxLayout;
    QPushButton *conf_button = new QPushButton("Configuration");
    bot_label = new QLabel("  L'analyseur est prêt à être lancé.");
    bot_label->setFixedHeight(50);
    QHBoxLayout *bot_buttons_layout = new QHBoxLayout;
    QPushButton *run_button = new QPushButton("Démarrer");
    QPushButton *stop_button = new QPushButton("Arrêter");

    top_label->setFrameStyle(QFrame::Box);
    bot_label->setFrameStyle(QFrame::Panel);

    top_button_layout->addWidget(conf_button);
    top_button_layout->setAlignment(Qt::AlignRight);

    bot_buttons_layout->addWidget(run_button);
    bot_buttons_layout->addWidget(stop_button);
    bot_buttons_layout->setAlignment(Qt::AlignRight);

    gen_layout->addWidget(top_label);
    gen_layout->addLayout(top_button_layout);
    gen_layout->addWidget(bot_label);
    gen_layout->addLayout(bot_buttons_layout);

    QObject::connect(conf_button, SIGNAL(clicked()), this, SLOT(open_conf_window()));
    QObject::connect(run_button, SIGNAL(clicked()), this->parent()->parent(), SLOT(start_analyser()));
    QObject::connect(stop_button, SIGNAL(clicked()), this->parent()->parent(), SLOT(stop_analyser()));
}

/* Slot qui ouvre la fenêtre de configuration de l'analyseur. */
void AnalyserTab::open_conf_window()
{
    AnalyserConfWindow *win = new AnalyserConfWindow();
    win->show();
}

/* Slot qui met à jour le label qui renseigne sur le statut de l'analyseur pour
 * signifier qu'il est démarré. */
void AnalyserTab::set_label_started()
{
    bot_label->setText("L'analyseur est en cours d'exécution.");
}

/* Slot qui met à jour le label qui renseigne sur le statut de l'analyseur pour
 * signifier qu'il est arrêté. */
void AnalyserTab::set_label_stopped()
{
    bot_label->setText("L'analyseur est arrêté.");
}
