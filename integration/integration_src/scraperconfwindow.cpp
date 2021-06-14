#include "scraperconfwindow.h"

ScraperConfWindow::ScraperConfWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Configuration du scraper");
    setModal(true);
    resize(600, 400);

    conf = ConfList(CONF_FILENAME);
    QVBoxLayout *main_layout = new QVBoxLayout(this);

    QLabel *twit_api_lab = new QLabel("API twitter");
    QFormLayout *twit_api_lay = new QFormLayout;
    key = new QLineEdit(conf.getValue(T_API_KEY));
    skey = new QLineEdit(conf.getValue(T_API_SKEY));
    tok = new QLineEdit(conf.getValue(T_API_TOK));
    stok = new QLineEdit(conf.getValue(T_API_STOK));
    twit_api_lay->addRow("Key :", key);
    twit_api_lay->addRow("Secret Key :", skey);
    twit_api_lay->addRow("Token :", tok);
    twit_api_lay->addRow("Secret token :", stok);

    QLabel *search_opt_lab = new QLabel("Options de recherche");
    QFormLayout *search_opt_lay = new QFormLayout;
    sopt_tpr = new QSpinBox;
    sopt_tpr->setMinimum(1);
    sopt_tpr->setMaximum(1000);
    sopt_tpr->setValue(conf.getValue(SC_TPR).toInt());
    sopt_lang = new QComboBox;
    sopt_lang->addItem("Français", "fr");
    sopt_lang->addItem("Anglais", "eng");
    sopt_lang->setCurrentIndex(sopt_lang->findData(conf.getValue(SC_LANG)));
    sopt_itv = new QSpinBox;
    sopt_itv->setMaximum(3600);
    sopt_itv->setMinimum(1);
    sopt_itv->setValue(conf.getValue(SC_ITV).toInt());
    sopt_rate_limit = new QSpinBox;
    sopt_rate_limit->setMaximum(1800);
    sopt_rate_limit->setMinimum(1);
    sopt_rate_limit->setValue(conf.getValue(SC_RTL).toInt());
    search_opt_lay->addRow("Tweets par requète :", sopt_tpr);
    search_opt_lay->addRow("Langue :", sopt_lang);
    search_opt_lay->addRow("Intervalle :", sopt_itv);
    search_opt_lay->addRow("Débit limite", sopt_rate_limit);

    QHBoxLayout *but_lay = new QHBoxLayout;
    QPushButton *save_but = new QPushButton("Valider");
    QPushButton *close_but = new QPushButton("Annuler");
    but_lay->addWidget(close_but);
    but_lay->addWidget(save_but);
    but_lay->setAlignment(Qt::AlignRight);

    main_layout->addWidget(twit_api_lab);
    main_layout->addLayout(twit_api_lay);
    main_layout->addSpacing(10);
    main_layout->addWidget(search_opt_lab);
    main_layout->addLayout(search_opt_lay);
    main_layout->addSpacing(10);
    main_layout->addLayout(but_lay);

    QObject::connect(save_but, SIGNAL(clicked()), this, SLOT(save_values()));
    QObject::connect(close_but, SIGNAL(clicked()), this, SLOT(close()));
}

/* Slot qui enregistre les valeurs contenues dans les champs de texte dans le
 * fichier de configuration. */
void ScraperConfWindow::save_values()
{
    conf.setValue(T_API_KEY, key->text());
    conf.setValue(T_API_SKEY, skey->text());
    conf.setValue(T_API_TOK, tok->text());
    conf.setValue(T_API_STOK, stok->text());
    conf.setValue(SC_TPR, QString::number(sopt_tpr->value()));
    conf.setValue(SC_LANG, sopt_lang->currentData().toString());
    conf.setValue(SC_ITV, QString::number(sopt_itv->value()));
    conf.setValue(SC_RTL, QString::number(sopt_rate_limit->value()));
    conf.write(CONF_FILENAME);
    this->close();
}
