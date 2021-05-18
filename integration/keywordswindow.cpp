#include "keywordswindow.h"
#include <QFile>
#include <QTextStream>
#include <QtSql>
#include <QMessageBox>


KeyWordsWindow::KeyWordsWindow(QWidget *parent) : QDialog(parent)
{
    setWindowTitle("Mots-clés");
    setModal(true);
    main_layout = new QVBoxLayout(this);
    unsaved_mod = false;

    QFrame *frame = new QFrame;
    frame->setFrameStyle(QFrame::Box);
    QVBoxLayout *kw_lay = new QVBoxLayout;

    QHBoxLayout *kw_but_lay = new QHBoxLayout;
    add_kw_but = new QPushButton("Nouveau mot-clé");
    QPushButton *suppr_kw_but = new QPushButton("Supprimer");
    kw_but_lay->addWidget(add_kw_but);
    kw_but_lay->addWidget(suppr_kw_but);
    kw_but_lay->setAlignment(Qt::AlignLeft);

    list_model = new QStringListModel;
    view = new QListView;
    load_keywords();
    view->setModel(list_model);

    kw_lay->addWidget(view);
    kw_lay->addLayout(kw_but_lay);
    frame->setLayout(kw_lay);

    but_lay = new QHBoxLayout;
    save_but = new QPushButton("Enregistrer");
    close_but = new QPushButton("Fermer");
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

void KeyWordsWindow::load_keywords()
{
    QFile file(KW_FILENAME);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Erreur", "Impossible d'ouvrir le fichier des mots-clés.");
        close();
    }
    QTextStream stream(&file);
    QString kw, ret;
    QStringList file_kw_list, db_kw_list;
    QSet<QString> only_file_kw, only_db_kw;
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery select_query, insert_query, del_query;
    //select_query.prepare("select mot from mot_clé where mot = ?;");
    if (!db.open())
    {

    }
    while (!stream.atEnd())
    {
        kw = stream.readLine();
        file_kw_list << kw;
    }
    list_model->setStringList(file_kw_list);
    select_query.exec("select mot from mot_clé;");
    while (select_query.next())
    {
        db_kw_list << select_query.value(0).toString();
    }
    db_kw_list.sort();
    file_kw_list.sort();
    if (db_kw_list != file_kw_list)
    {
        QMessageBox::warning(this, "Mise à jour de la base", "Les contenus du fichier mot-clé et de la base de données diffèrent.\n"
                                   "Le contenu de la base de données va donc être mis à jour.");
        only_db_kw = db_kw_list.toSet().subtract(file_kw_list.toSet());
        only_file_kw = file_kw_list.toSet().subtract(db_kw_list.toSet());
        foreach (const QString kw, only_db_kw)
        {
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
            insert_query.prepare("insert into mot_clé (mot) values (?);");
            insert_query.addBindValue(kw);
            insert_query.exec();
        }
    }
}

void KeyWordsWindow::add_kw()
{
    unsaved_mod = true;
    int rc = list_model->rowCount();
    list_model->insertRow(rc);
    list_model->setData(list_model->index(rc), "Nouveau mot-clé");
}

void KeyWordsWindow::suppr_kw()
{
    unsaved_mod = true;
    QItemSelectionModel *select = view->selectionModel();
    QModelIndex idx = select->currentIndex();
    list_model->removeRow(idx.row());
}

void KeyWordsWindow::save_kws()
{
    unsaved_mod = false;
    QFile file(KW_FILENAME);
    QStringList list = list_model->stringList();
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        return;
    }
    QTextStream stream(&file);
    for (int i = 0; i < list.size(); i++)
    {
        stream << list[i] << endl;
    }
    file.close();
}

void KeyWordsWindow::reject()
{
    int ret;
    if (unsaved_mod)
    {
        ret = QMessageBox::warning(this, "Modifications non sauvegardées", "Les modifications effectuées seront perdues, voulez-vous continuer ?", QMessageBox::No | QMessageBox::Yes);
        if (ret == QMessageBox::No)
        {
            return;
        }
    }
    QDialog::reject();
}

void KeyWordsWindow::list_model_data_changed()
{
    unsaved_mod = true;
}
