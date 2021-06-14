#ifndef DBTAB_H
#define DBTAB_H

#include <QWidget>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>
#include "conflist.h"

/* L'onglet base de données de la fenêtre principale.
 * Permet d'entrer, de valider et le cas échéant d'écrire dans le fichier de
 * configuration les informations de connexion à la base de données. */
class DBTab : public QWidget
{
    Q_OBJECT
    friend class TestDBTab;
public:
    explicit DBTab(QWidget *parent = nullptr);

signals:

public slots:
    void save_values();

private:
    QLineEdit *hostname;
    QLineEdit *username;
    QLineEdit *user_pw;
    QLineEdit *dbname;
    ConfList conf;
};

#endif // DBTAB_H
