#include "typeswindow.h"
#include <QtSql>
#include <QMessageBox>
#include <QSet>

/* Constructeur utilisé pour créer la fenêtre.
 * w est la fenêtre parent.
 * pat_model est le modèle des patrons de la fenêtre parent.
 * type_name_list est la liste des types
 */
TypesWindow::TypesWindow(AnalyserConfWindow *w, QStandardItemModel *pat_model,
                         QStringList type_name_list) : pat_model(pat_model)
{
    setWindowTitle("Types");
    setModal(true);
    QVBoxLayout *main_layout = new QVBoxLayout(this);
    unsaved_mod = false;

    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::Box);
    QVBoxLayout *types_lay = new QVBoxLayout;

    QHBoxLayout *types_but_lay = new QHBoxLayout;
    QPushButton *add_type_but = new QPushButton("Nouveau type");
    QPushButton *suppr_type_but = new QPushButton("Supprimer");
    types_but_lay->addWidget(add_type_but);
    types_but_lay->addWidget(suppr_type_but);
    types_but_lay->setAlignment(Qt::AlignLeft);

    types_model = new QStringListModel(type_name_list);
    view = new QListView;
    view->setModel(types_model);

    types_lay->addWidget(view);
    types_lay->addLayout(types_but_lay);
    frame->setLayout(types_lay);

    QHBoxLayout *but_lay = new QHBoxLayout;
    QPushButton *save_but = new QPushButton("Enregistrer");
    QPushButton *close_but = new QPushButton("Fermer");
    but_lay->addWidget(save_but);
    but_lay->addWidget(close_but);
    but_lay->setAlignment(Qt::AlignRight);

    main_layout->addWidget(frame);
    main_layout->addLayout(but_lay);

    QObject::connect(add_type_but, SIGNAL(clicked()), this, SLOT(add_type()));
    QObject::connect(suppr_type_but, SIGNAL(clicked()), this, SLOT(suppr_type()));
    QObject::connect(save_but, SIGNAL(clicked()), this, SLOT(save_types()));
    QObject::connect(close_but, SIGNAL(clicked()), this, SLOT(reject()));
    QObject::connect(types_model,
                     SIGNAL(dataChanged(const QModelIndex, const QModelIndex,
                                        const QVector<int>)),
                     this, SLOT(list_model_data_changed()));
    QObject::connect(this, SIGNAL(db_types_synced()), w, SLOT(reload_types()));
}

/* Slot qui ajoute un type dans le modèle.
 * Le nouveau type doit être édite par l'utilisateur. */
void TypesWindow::add_type()
{
    unsaved_mod = true;
    int rc = types_model->rowCount();
    types_model->insertRow(rc);
    types_model->setData(types_model->index(rc), "Nouveau type");
}

/* Slot qui supprime un type du modèle. */
void TypesWindow::suppr_type()
{
    QItemSelectionModel *select = view->selectionModel();
    QModelIndex idx = select->currentIndex();
    QString type = types_model->data(idx).toString();
    bool type_used = false;
    for (int i = 0; i < pat_model->rowCount(); i++)
    {
        if (pat_model->item(i, 1)->text() == type)
        {
            type_used = true;
            break;
        }
    }
    if (type_used)
    {
        QMessageBox::warning(this, "Type utilisé", "Ce type ne peut être supprimé car il est utilisé par un patron.\n"
                             "Supprimez d'abord tous les patrons qui utilisent ce type pour pouvoir supprimer le type.");
    }
    else
    {
        unsaved_mod = true;
        types_model->removeRow(idx.row());
    }
}

/* Slot qui sauvegarde dans la base de données les modifications de types
 * effectuées.
 * Les types suprimés sont supprimés de la base ainsi que toutes les entrées de
 * la table possede qui concernaient ce type.
 * Les types ajoutés sont ajoutés à la base.
 * Émet le signal db_types_synced pour signifier à la fenêtre de configuration
 * de l'analyseur (AnalyserConfWindow) qu'il faut recharger les types.
 */
void TypesWindow::save_types()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    QStringList db_types_list, model_types_list;
    QSet<QString> only_db_types, only_model_types;
    if (!db.open())
    {
        QMessageBox::warning(this, "Erreur", "Impossible de se connecter à la base de données.\n"
                             "Les modifications des types n'ont pas été sauvegardées.");
        return;
    }
    query.exec("select nom_type from type order by id_type;");
    while(query.next())
    {
        db_types_list << query.value(0).toString();
    }
    model_types_list = types_model->stringList();
    only_db_types = db_types_list.toSet().subtract(model_types_list.toSet());
    only_model_types = model_types_list.toSet().subtract(db_types_list.toSet());
    foreach (const QString type, only_db_types)
    {
        QSqlQuery select_query, delete_query;
        select_query.prepare("select id_type from type where nom_type = ?;");
        select_query.addBindValue(type);
        select_query.exec();
        select_query.next();
        /* Supprimer les entrées de possede pour respecter les contraintes de
         * clé étrangère. */
        delete_query.prepare("delete from possede where id_type = ?");
        delete_query.addBindValue(select_query.value(0));
        delete_query.exec();
        delete_query.prepare("delete from type where nom_type = ?");
        delete_query.addBindValue(type);
        delete_query.exec();
    }
    foreach (const QString type, only_model_types)
    {
        QSqlQuery insert_query;
        insert_query.prepare("insert into type (nom_type) values (?);");
        insert_query.addBindValue(type);
        insert_query.exec();
    }
    unsaved_mod = false;
    db.close();
    emit db_types_synced();
}

/* Réimplémentation du slot reject.
 * Permet de prévenir l'utilisateur si des modifications non sauvegardée seront
 * perdues à la fermeture de la fenêtre et lui donner la possibilité de
 * renoncer à la fermeture. */
void TypesWindow::reject()
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
void TypesWindow::list_model_data_changed()
{
    unsaved_mod = true;
}
