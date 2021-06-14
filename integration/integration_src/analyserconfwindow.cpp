#include <QLineEdit>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "analyserconfwindow.h"
#include "patternwindow.h"
#include "typeswindow.h"
#include "conflist.h"

AnalyserConfWindow::AnalyserConfWindow()
{
    setWindowTitle("Configuration de l'analyseur");
    load_types();

    patterns_model = new QStandardItemModel;
    patterns_model->setColumnCount(2);
    patterns_model->setHeaderData(0, Qt::Horizontal, "Patron");
    patterns_model->setHeaderData(1, Qt::Horizontal, "Type");
    load_patterns(PATTERNS_FILENAME);
    results_model = new QStandardItemModel;
    results_model->setColumnCount(2);
    results_model->setHeaderData(0, Qt::Horizontal, "Message");
    results_model->setHeaderData(1, Qt::Horizontal, "Type");
    new_sample();

    QTabWidget *tabs = new QTabWidget;
    gen_layout = new QVBoxLayout(this);
    QHBoxLayout *but_layout = new QHBoxLayout;
    save_but = new QPushButton("Enregistrer");
    close_but = new QPushButton("Fermer");
    but_layout->addWidget(save_but);
    but_layout->addWidget(close_but);
    but_layout->setAlignment(Qt::AlignRight);

    patterns_tab = new QWidget;
    results_tab = new QWidget;

    init_ptab();
    init_rtab();

    tabs->addTab(patterns_tab, "Patrons");
    tabs->addTab(results_tab, "Résultats");
    gen_layout->addWidget(tabs);
    gen_layout->addLayout(but_layout);
    this->resize(500, 500);
    this->setModal(true);
    // Par précaution pour éviter les fuites de mémoire.
    this->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(pt_add_pattern_button, SIGNAL(clicked()), this, SLOT(add_pattern()));
    QObject::connect(save_but, SIGNAL(clicked()), this, SLOT(save_patterns()));
    QObject::connect(close_but, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(pt_select_type, SIGNAL(currentTextChanged(QString)), this, SLOT(show_patterns_by_type(QString)));
    QObject::connect(rt_new_sample_but, SIGNAL(clicked()), this, SLOT(new_sample()));
    QObject::connect(rt_redo_analyze_but, SIGNAL(clicked()), this, SLOT(analyze_results()));
    QObject::connect(pt_suppr_pattern_button, SIGNAL(clicked()), this, SLOT(suppr_pattern()));
    QObject::connect(pt_modify_pattern_button, SIGNAL(clicked()), this, SLOT(open_mod_pattern_win()));
    QObject::connect(pt_types_edit_but, SIGNAL(clicked()), this, SLOT(open_types_win()));
    QObject::connect(results_view->horizontalHeader(),SIGNAL(sectionResized(int, int, int)),
        results_view, SLOT(resizeRowsToContents()));
}

/* Charge les types depuis la base de données.
 * Remplit les listes type_id_list et type_name_list avec les informations
 * récoltées. */
void AnalyserConfWindow::load_types()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    if (!db.open())
    {
        return;
    }
    query.exec("select id_type, nom_type from type order by id_type;");
    while(query.next())
    {
        type_id_list << query.value(0).toInt();
        type_name_list << query.value(1).toString();
    }
    query.finish();
    db.close();
}

/* Initialise l'onglet des patrons. */
void AnalyserConfWindow::init_ptab()
{
    QVBoxLayout *pt_layout = new QVBoxLayout;

    QHBoxLayout *pt_top_layout = new QHBoxLayout;
    pt_select_type = new QComboBox();
    pt_types_edit_but = new QPushButton("Gérer les types");
    pt_select_type->addItem("Tous les types");
    pt_select_type->addItems(type_name_list);
    pt_top_layout->addWidget(pt_select_type);
    pt_top_layout->addWidget(pt_types_edit_but);
    pt_top_layout->setAlignment(Qt::AlignLeft);

    patterns_view = new QTableView;
    patterns_view->verticalHeader()->hide();
    patterns_view->setModel(patterns_model);
    patterns_view->resizeColumnToContents(0);
    patterns_view->setEditTriggers(QTableView::NoEditTriggers);
    patterns_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    patterns_view->setSelectionMode(QAbstractItemView::SingleSelection);

    QHBoxLayout *pt_bot_layout = new QHBoxLayout;
    pt_add_pattern_button = new QPushButton("Ajouter patron", this);
    pt_modify_pattern_button = new QPushButton("Modifier", this);
    pt_suppr_pattern_button = new QPushButton("Supprimer", this);

    pt_bot_layout->addWidget(pt_add_pattern_button);
    pt_bot_layout->addWidget(pt_modify_pattern_button);
    pt_bot_layout->addWidget(pt_suppr_pattern_button);
    pt_bot_layout->setAlignment(Qt::AlignLeft);

    pt_layout->addLayout(pt_top_layout);
    pt_layout->addWidget(patterns_view);
    pt_layout->addLayout(pt_bot_layout);

    patterns_tab->setLayout(pt_layout);
}

/* Initialise l'onglet des résultats. */
void AnalyserConfWindow::init_rtab()
{
    QVBoxLayout *rt_layout = new QVBoxLayout;

    results_view = new QTableView;
    results_view->verticalHeader()->hide();
    results_view->setModel(results_model);
    results_view->setEditTriggers(QTableView::NoEditTriggers);
    results_view->setSelectionMode(QAbstractItemView::NoSelection);
    results_view->horizontalHeader()->resizeSection(0, 340);
    results_view->resizeRowsToContents();

    QHBoxLayout *rt_but_lay = new QHBoxLayout;
    rt_redo_analyze_but = new QPushButton("Refaire l'analyse");
    rt_new_sample_but = new QPushButton("Nouvel échantillon");
    rt_but_lay->addWidget(rt_redo_analyze_but);
    rt_but_lay->addWidget(rt_new_sample_but);

    rt_layout->addWidget(results_view);
    rt_layout->addLayout(rt_but_lay);

    results_tab->setLayout(rt_layout);
}

/* Remplit le modèle des patrons patterns_model avec les informations contenues
 * dans le fichier des patrons.
 * Le fichier doit être de la forme suivante : le patron suivi d'une tabulation
 * suivie de l'id du type suivie d'un retour à la ligne.
 * Le cas où le type serait absent est géré mais il est conseillé à
 * l'utilisateur de ne pas modifier le fichier directement et de passer par
 * l'interface graphique pour éditer les patrons.
 * Un patron qui n'a pas de type sera ignoré par l'analyseur.*/
void AnalyserConfWindow::load_patterns(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text | QIODevice::ExistingOnly))
    {
        QMessageBox::critical(this, "Erreur", QString("Impossible d'ouvrir le fichier ") + PATTERNS_FILENAME + "\nPeut-être n'existe-t-il pas ?");
        return;
    }
    QTextStream stream(&file);
    QString line, type;
    QStringList list;
    int rc, id, idx;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        list = line.split('\t');

        rc = patterns_model->rowCount();
        if (list.size() == 1)
        {
            if (line.size() > 0)
            {
                patterns_model->insertRow(rc);
                patterns_model->setItem(rc, 0, new QStandardItem(list[0]));
                patterns_model->setItem(rc, 1, new QStandardItem(""));
            }
        }
        else
        {
            bool *ok = new bool;
            id = list[1].toInt(ok);
            if (*ok)
            {
                patterns_model->insertRow(rc);
                patterns_model->setItem(rc, 0, new QStandardItem(list[0]));
                idx = type_id_list.indexOf(id);
                if (idx >= 0)
                {
                    type = type_name_list[idx];
                }
                else
                {
                    type = "";
                }
                patterns_model->setItem(rc, 1, new QStandardItem(type));
            }
        }
    }
    patterns_modified = false;
}

/* Slot qui remplit le modèle des résultats results_model avec 100 messages
 * tirés de la base de données.
 * Seule la première colonne du modèle est remplit, la seconde le sera par la
 * fonction analyse_results. */
void AnalyserConfWindow::load_results()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    int i = 0;
    if (!db.open())
    {
        return;
    }
    query.exec("select contenu from messages order by random() limit 100;");
    while(query.next())
    {
        results_model->insertRow(i);
        results_model->setItem(i, 0, new QStandardItem(query.value(0).toString()));
        results_model->setItem(i++, 1, new QStandardItem(""));

    }
    db.close();
}

/* Slot qui analyse les résultats contenus dans le modèle results_model.
 * Chaque message est comparé à tous les patrons et en cas de match le type
 * correspondant est ajouté à la deuxième colonne dans le modèle.
 * Un message peut avoir plusieurs types. */
void AnalyserConfWindow::analyze_results()
{
    QString message, pt_regex, type_string;
    QRegExp regex;
    QSet<QString> set;
    bool hateful;
    for (int i = 0; i < 100 && i < results_model->rowCount(); i++)
    {
        hateful = false;
        set.clear();
        QModelIndex idx = results_model->index(i, 0);
        message = results_model->data(idx).toString();
        for (int j = 0; j < patterns_model->rowCount(); j++)
        {
            idx = patterns_model->index(j, 0);
            pt_regex = patterns_model->data(idx).toString();
            regex.setPattern(pt_regex);
            if (message.contains(regex))
            {
                idx = patterns_model->index(j, 1);
                set << patterns_model->data(idx).toString();
                results_model->item(i)->setBackground(QColor(Qt::red));
                hateful = true;
            }
        }
        if (!hateful)
        {
            results_model->item(i)->setBackground(QBrush());
            results_model->item(i, 1)->setText("");
        }
        else
        {
            type_string.clear();
            QStringList list = set.values();
            list.sort();
            for (int i = 0; i < list.size(); ++i)
            {
                type_string.append(list[i] + ", ");
            }
            type_string.chop(2);
            results_model->setItem(i, 1, new QStandardItem(type_string));
        }

    }
}

/* Slot qui charge un nouvel échantillon de résultats et les analyse. */
void AnalyserConfWindow::new_sample()
{
    load_results();
    analyze_results();
}

/* Slot qui ajoute un patron au modèle.
 * Le nouveau patron doit être édité par l'utilisateur. */
void AnalyserConfWindow::add_pattern()
{
    patterns_modified = true;
    QList<QStandardItem*> list;
    list << new QStandardItem("Nouveau patron") << new QStandardItem("");
    patterns_model->appendRow(list);
}

/* Slot qui supprime un patron du modèle.
 * Demande confirmation à l'utilisateur. */
void AnalyserConfWindow::suppr_pattern()
{
    QItemSelectionModel *select = patterns_view->selectionModel();
    if (!select->hasSelection())
    {
        return;
    }
    this->patterns_modified = true;
    int rep = QMessageBox::question(this, "Suppression", "Êtes-vous sûr de vouloir supprimer ce patron ?", QMessageBox::Yes | QMessageBox::No);
    if (rep == QMessageBox::No)
    {
        return;
    }
    QModelIndex idx = select->currentIndex();
    patterns_model->removeRow(idx.row());
}

/* Slot qui ouvre la fenêtre de modification d'un patron. */
void AnalyserConfWindow::open_mod_pattern_win()
{
    QItemSelectionModel *select = patterns_view->selectionModel();
    if (!select->hasSelection())
    {
        return;
    }
    QModelIndex idx = select->currentIndex();
    PatternWindow *w = new PatternWindow(this, patterns_model, type_name_list, idx.row());
    w->show();
}

/* Slot qui ouvre la fenêtre d'édition des types. */
void AnalyserConfWindow::open_types_win()
{
    TypesWindow *w = new TypesWindow(this, patterns_model, type_name_list);
    w->show();
}

/* Slot qui modifie un patron du modèle.
 * row est la ligne concernée, regex la patron et type le type associé. */
void AnalyserConfWindow::modify_pattern(int row, QString regex, QString type)
{
    patterns_modified = true;
    patterns_model->setItem(row, 0, new QStandardItem(regex));
    patterns_model->setItem(row, 1, new QStandardItem(type));
}

/* Slot qui permet d'enregistrer les patrons sur le disque. */
void AnalyserConfWindow::save_patterns()
{
    QFile file(PATTERNS_FILENAME);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Erreur", QString("Impossible d'ouvrir le fichier ") + PATTERNS_FILENAME);
        return;
    }
    QTextStream stream(&file);

    for (int i = 0; i < patterns_model->rowCount(); i++)
    {
        QModelIndex mod_idx = patterns_model->index(i, 1);
        int idx = type_name_list.indexOf(patterns_model->data(mod_idx).toString());
        stream << patterns_model->item(i, 0)->text();
        if (idx >= 0)
        {
            stream << "\t" << type_id_list[idx];
        }
        stream << endl;
    }
    file.close();
    patterns_modified = false;
}

/* Slot qui permet de n'afficher que les patrons du type donné en argument
 * dans la vue des patrons (patterns_view). */
void AnalyserConfWindow::show_patterns_by_type(QString type)
{
    int rc = patterns_model->rowCount();
    for (int i = 0; i < rc; i++)
    {
        QModelIndex idx = patterns_model->index(i, 1);
        if (type == "Tous les types" || patterns_model->data(idx).toString() == type)
        {
            patterns_view->showRow(i);
        }
        else
        {
            patterns_view->hideRow(i);
        }
    }
}

/* Slot qui réactualise les types dans la liste des types et dans la QCombobox
 * pt_select_type qui permet à l'utilisateur de filtrer les patrons par type.
 * Les types sont rechargés à partir de la base de données. */
void AnalyserConfWindow::reload_types()
{
    type_id_list.clear();
    type_name_list.clear();
    load_types();
    pt_select_type->clear();
    pt_select_type->addItem("Tous les types");
    pt_select_type->addItems(type_name_list);
}

/* Réimplémentation de la fonction reject.
 * Demande confirmation à l'utilisateur avant de fermer la fenêtre s'il y a des
 * modifications non enregistrées. */
void AnalyserConfWindow::reject()
{
    if (patterns_modified)
    {
        int rep = QMessageBox::critical(this, "Fermeture de la fenêtre", "Les modifications des patrons ne seront pas enregistrées, êtes-vous sûr de vouloir fermer la fenêtre ?", QMessageBox::Yes | QMessageBox::No);
        if (rep == QMessageBox::No)
        {
            return;
        }
    }
    QDialog::reject();
}

QList<int> AnalyserConfWindow::get_type_id_list()
{
    return type_id_list;
}

QStringList AnalyserConfWindow::get_type_name_list()
{
    return type_name_list;
}

QStandardItemModel* AnalyserConfWindow::get_patterns_model()
{
    return patterns_model;
}
