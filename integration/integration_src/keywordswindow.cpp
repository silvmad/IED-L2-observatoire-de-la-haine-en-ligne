#include "keywordswindow.h"
#include <QFile>
#include <QTextStream>
#include <QtSql>
#include <QSqlQuery>
#include <QMessageBox>

/* Constructeur utilisé par la MainWindow pour synchroniser la base de données
 * et le fichier des mots-clés. *//*
KeyWordsWindow::KeyWordsWindow(MainWindow *w)
{
    list_model = new QStringListModel;
    QObject::connect(this, SIGNAL(db_kw_synced(bool)), w, SLOT(set_db_kw_synced(bool)));
}*/

/* Constructeur utilisé pour afficher la fenêtre. */
KeyWordsWindow::KeyWordsWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Mots-clés");
    setModal(true);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    unsaved_mod = false;

    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::Box);
    QVBoxLayout *kw_lay = new QVBoxLayout;

    QHBoxLayout *kw_but_lay = new QHBoxLayout;
    QPushButton *add_kw_but = new QPushButton("Nouveau mot-clé");
    QPushButton *suppr_kw_but = new QPushButton("Supprimer");
    kw_but_lay->addWidget(add_kw_but);
    kw_but_lay->addWidget(suppr_kw_but);
    kw_but_lay->setAlignment(Qt::AlignLeft);

    list_model = new QStringListModel;
    view = new QListView;
    view->setModel(list_model);

    kw_lay->addWidget(view);
    kw_lay->addLayout(kw_but_lay);
    frame->setLayout(kw_lay);

    QHBoxLayout *but_lay = new QHBoxLayout;
    QPushButton *save_but = new QPushButton("Enregistrer");
    QPushButton *close_but = new QPushButton("Fermer");
    but_lay->addWidget(save_but);
    but_lay->addWidget(close_but);
    but_lay->setAlignment(Qt::AlignRight);

    main_layout->addWidget(frame);
    main_layout->addLayout(but_lay);

    QObject::connect(add_kw_but, SIGNAL(clicked()), this, SLOT(add_kw()));
    QObject::connect(suppr_kw_but, SIGNAL(clicked()), this, SLOT(suppr_kw()));
    QObject::connect(close_but, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(save_but, SIGNAL(clicked()), this, SLOT(save_kws()));
    QObject::connect(list_model, SIGNAL(dataChanged(const QModelIndex, const QModelIndex, const QVector<int>)), this, SLOT(list_model_data_changed()));
}

/* Remplit le modèle list_model avec le contenu du fichier des mots-clés puis
 * compare le contenu du modèle avec celui de la table des mots-clés dans la
 * base de données. S'il y a des différences, la base de données est mise à
 * jour pour correspondre au modèle à l'aide de la fonction synchronize-db.
 * filename est le nom du fichier des mots-clés. */
void KeyWordsWindow::load_keywords(QString filename)
{
    QStringList file_kw_list, db_kw_list;
    if (load_kw_from_file(file_kw_list, filename) < 0)
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier des mots-clés.");
        close();
        return;
    }
    list_model->setStringList(file_kw_list);
    if (load_kw_from_db(db_kw_list) < 0)
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'accéder à la base de données.");
        close();
        return;
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
                close();
                return;
            }
        }
        if (synchronize_db(file_kw_list, db_kw_list) < 0)
        {
            QMessageBox::critical(this, "Erreur", "Impossible d'accéder à la base de données.\n"
                                  "La synchronisation a échoué.");
        }
    }
}

/* Fonction statique
 * Synchronise la table des mots-clés de la base de données avec le contenu de
 * la liste donnée en argument.
 * current_kw_list est la liste des mots-clés à écrire.
 * db_kw_list est la liste des mots-clés contenus dans la table mots-clés de la
 * base de données.
 * Tous les mots de db_kw_list non présents dans current_kw_list sont supprimés
 * de la base de données.
 * Tous les mots de current_kw_list non présents dans db_kw_list sont ajoutés à
 * la base de données.
 * Si des mots-clés sont supprimés de la base de données, les entrées
 * correspondantes dans la table contient sont également supprimées pour
 * respecter les contraintes de clé étrangère. */
int KeyWordsWindow::synchronize_db(QStringList current_kw_list, QStringList db_kw_list)
{
    QSqlQuery query;
    QSqlDatabase db = QSqlDatabase::database();
    if (!db.open())
    {
        return -1;
    }
    if (db_kw_list.size() == 0)
    {
        query.exec("select mot from mot_clé;");
        while (query.next())
        {
            db_kw_list << query.value(0).toString();
        }
    }
    QSet<QString> only_file_kw, only_db_kw;
    only_db_kw = db_kw_list.toSet().subtract(current_kw_list.toSet());
    only_file_kw = current_kw_list.toSet().subtract(db_kw_list.toSet());
    foreach (const QString kw, only_db_kw)
    {
        QSqlQuery del_query, select_query;
        select_query.prepare("select id_mot_clé from mot_clé where mot = ?");
        select_query.addBindValue(kw);
        select_query.exec();
        select_query.next();
        /* Il faut d'abord supprimer les entrées de contient pour respecter les contraintes
         * de clé étrangère */
        del_query.prepare("delete from contient where id_mot_clé = ?");
        del_query.addBindValue(select_query.value(0));
        del_query.exec();
        del_query.prepare("delete from mot_clé where mot = ?;");
        del_query.addBindValue(kw);
        del_query.exec();
    }
    foreach (const QString kw, only_file_kw)
    {
        QSqlQuery insert_query;
        insert_query.prepare("insert into mot_clé (mot) values (?);");
        insert_query.addBindValue(kw);
        insert_query.exec();
    }
    db.close();
    return 0;
}

/* Fonction statique
 * Remplit la liste donnée en argument avec les mots contenus dans le fichier
 * dont le nom est donné en argument. */
int KeyWordsWindow::load_kw_from_file(QStringList& list,QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return -1;
    }
    QTextStream stream(&file);
    QString kw;
    QStringList file_kw_list, db_kw_list;
    while (!stream.atEnd())
    {
        kw = stream.readLine();
        list << kw;
    }
    return 0;
}

/* Fonction statique
 * Remplit la liste donnée en argument avec le contenu de la table mot de la
 * base de données. */
int KeyWordsWindow::load_kw_from_db(QStringList& list)
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery select_query;
    if (!db.open())
    {
        return -1;
    }
    select_query.exec("select mot from mot_clé;");
    while (select_query.next())
    {
        list << select_query.value(0).toString();
    }
    db.close();
    return 0;
}

/* Slot qui ajoute un mot-clé dans le modèle.
 * Le nouveau mot clé doit être édité par l'utilisateur et n'a pas de type. */
void KeyWordsWindow::add_kw()
{
    unsaved_mod = true;
    int rc = list_model->rowCount();
    list_model->insertRow(rc);
    list_model->setData(list_model->index(rc), "Nouveau mot-clé");
}

/* Slot qui supprime un mot-clé du modèle. */
void KeyWordsWindow::suppr_kw()
{
    unsaved_mod = true;
    QItemSelectionModel *select = view->selectionModel();
    QModelIndex idx = select->currentIndex();
    list_model->removeRow(idx.row());
}

/* Slot qui enregistre les mots-clé du modèle dans le fichier des mots-clés et
 * synchronise la base de données avec le modèle. */
void KeyWordsWindow::save_kws()
{    
    QFile file(KW_FILENAME);
    QStringList list = list_model->stringList();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier des mots-clés.\n"
                              "La sauvegarde a échoué.");
        return;
    }
    QTextStream stream(&file);
    for (int i = 0; i < list.size(); i++)
    {
        stream << list[i] << endl;
    }
    if (synchronize_db(list) < 0)
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'accéder à la base de données.\n"
                              "La synchronisation a échoué.");
    }
    file.close();
    unsaved_mod = false;
}

/* Réimplémentation du slot reject.
 * Permet de prévenir l'utilisateur si des modifications non sauvegardée seront
 * perdues à la fermeture de la fenêtre et lui donner la possibilité de
 * renoncer à la fermeture. */
void KeyWordsWindow::reject()
{
    int ret;
    if (unsaved_mod)
    {
        ret = QMessageBox::warning(this, "Modifications non sauvegardées", "Les modifications effectuées seront perdues, voulez-vous continuer ?",
                                   QMessageBox::No | QMessageBox::Yes);
        if (ret == QMessageBox::No)
        {
            return;
        }
    }
    QDialog::reject();
}

/* Slot qui permet de signifier que des modifications ont été faites sur le
 * modèle. */
void KeyWordsWindow::list_model_data_changed()
{
    unsaved_mod = true;
}
