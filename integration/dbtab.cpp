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
    QString old_hostname, old_username, old_user_pw, old_dbname;
    QSqlDatabase db = QSqlDatabase::database();
    old_hostname =  db.hostName();
    old_username = db.userName();
    old_user_pw = db.password();
    old_dbname = db.databaseName();
    db.setHostName(hostname->text());
    db.setUserName(username->text());
    db.setPassword(user_pw->text());
    db.setDatabaseName(dbname->text());
    if (!db.open())
    {
        QMessageBox::critical(this, "Échec de la connexion", "Impossible de se connecter à la base de données avec les informations fournies, elles ne seront donc pas sauvegardées.");
        db.setHostName(old_hostname);
        db.setUserName(old_username);
        db.setPassword(old_user_pw);
        db.setDatabaseName(old_dbname);
    }
    else
    {
        ret = QMessageBox::warning(this, "Sauvegarde", "Les données existantes seront écrasées, êtes-vous sûr de vouloir continuer ?", QMessageBox::Yes | QMessageBox::No);
        if (ret == QMessageBox::Yes)
        {
            this->conf.setValue(DB_HOST, hostname->text());
            this->conf.setValue(DB_USERNAME, username->text());
            this->conf.setValue(DB_USER_PW, user_pw->text());
            this->conf.setValue(DB_DBNAME, dbname->text());
            this->conf.write(CONF_FILENAME);
        }
    }
    db.close();
}
