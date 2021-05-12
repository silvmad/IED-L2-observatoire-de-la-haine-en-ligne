#include "dbtab.h"

DBTab::DBTab(QWidget *parent) : QWidget(parent)
{
    conf = ConfArray(CONF_FILENAME);

    QString hnm, unm, upw, dbn;
    hnm = conf.getValue(DB_HOST);
    unm = conf.getValue(DB_USERNAME);
    upw = conf.getValue(DB_USER_PW);
    dbn = conf.getValue(DB_DBNAME);

    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(hnm);
    db.setUserName(unm);
    db.setPassword(upw);
    db.setDatabaseName(dbn);

    main_layout = new QVBoxLayout(this);

    db_layout = new QFormLayout;
    hostname = new QLineEdit(hnm);
    username = new QLineEdit(unm);
    user_pw = new QLineEdit(upw);
    user_pw->setEchoMode(QLineEdit::Password);
    dbname = new QLineEdit(dbn);
    db_layout->addRow("Nom d'hôte", hostname);
    db_layout->addRow("Nom d'utilisateur", username);
    db_layout->addRow("Mot de passe", user_pw);
    db_layout->addRow("Nom de la base", dbname);

    button_layout = new QHBoxLayout;
    save_button = new QPushButton("Enregistrer");
    button_layout->addWidget(save_button);
    button_layout->setAlignment(Qt::AlignRight);

    main_layout->addLayout(db_layout);
    main_layout->addLayout(button_layout);
    //setLayout(main_layout);

    QObject::connect(save_button, SIGNAL(clicked()), this, SLOT(save_values()));
}

void DBTab::save_values()
{
    int ret;
    QString hostname, username, user_pw, dbname;
    hostname = this->hostname->text();
    username = this->username->text();
    user_pw = this->user_pw->text();
    dbname = this->dbname->text();
    QSqlDatabase db = QSqlDatabase::database();
    db.setHostName(hostname);
    db.setUserName(username);
    db.setPassword(user_pw);
    db.setDatabaseName(dbname);
    if (!db.open())
    {
        QMessageBox::critical(this, "Échec de la connexion", "Impossible de se connecter à la base de données avec les informations fournies, elles ne seront donc pas sauvegardées.");
    }
    else
    {
        ret = QMessageBox::warning(this, "Sauvegarde", "Les données existantes seront écrasées, êtes-vous sûr de vouloir continuer ?", QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            this->conf.setValue(DB_HOST, hostname);
            this->conf.setValue(DB_USERNAME, username);
            this->conf.setValue(DB_USER_PW, user_pw);
            this->conf.setValue(DB_DBNAME, dbname);
            this->conf.write(CONF_FILENAME);
        }
    }
    db.close();
}
