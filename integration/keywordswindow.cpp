#include "keywordswindow.h"
#include <QFile>
#include <QTextStream>
#include <QtSql>
#include <QMessageBox>


KeyWordsWindow::KeyWordsWindow(QWidget *parent) : QDialog(parent)
{
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

    //main_layout->addWidget(scroll);
    main_layout->addWidget(frame);
    main_layout->addLayout(but_lay);



    QObject::connect(add_kw_but, SIGNAL(clicked()), this, SLOT(add_kw()));
    QObject::connect(suppr_kw_but, SIGNAL(clicked()), this, SLOT(suppr_kw()));
    QObject::connect(close_but, SIGNAL(clicked()), this, SLOT(close_clicked()));
    QObject::connect(save_but, SIGNAL(clicked()), this, SLOT(save_kws()));
    QObject::connect(list_model, SIGNAL(dataChanged(const QModelIndex, const QModelIndex, const QVector<int>)), this, SLOT(list_model_data_changed()));
}

void KeyWordsWindow::load_keywords()
{
    QFile file(KW_FILENAME);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {

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
        //QPushButton *pb = new QPushButton("Supprimer");
        //kw_lay->addRow(kw, pb);
    }
    list_model->setStringList(file_kw_list);
    /*select_query.exec("select mot from mot_clé;");
    while (select_query.next())
    {
        db_kw_list << select_query.value(0).toString();
    }
    only_db_kw = db_kw_list.toSet().subtract(file_kw_list.toSet());
    only_file_kw = file_kw_list.toSet().subtract(db_kw_list.toSet());
    foreach (const QString kw, only_db_kw)
    {
        del_query.prepare("delete from mot_clé where mot = ?;");
        del_query.addBindValue(kw);
        del_query.exec();
    }
    foreach (const QString kw, only_file_kw)
    {
        insert_query.prepare("insert into mot_clé (mot) values (?);");
        insert_query.addBindValue(kw);
        insert_query.exec();
    }*/
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

void KeyWordsWindow::close_clicked()
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
    this->close();
}
void KeyWordsWindow::list_model_data_changed()
{
    unsaved_mod = true;
}
