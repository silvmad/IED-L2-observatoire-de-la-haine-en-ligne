#include <QLineEdit>
#include <QMessageBox>
#include <QFile>
#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include "analyserconfwindow.h"
#include "patternwindow.h"

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

    tabs = new QTabWidget;
    gen_layout = new QVBoxLayout(this);
    but_layout = new QHBoxLayout;
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
    QObject::connect(results_view->horizontalHeader(),SIGNAL(sectionResized(int, int, int)),
        results_view, SLOT(resizeRowsToContents()));
}

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
    db.close();
}

void AnalyserConfWindow::init_ptab()
{
    pt_layout = new QVBoxLayout;

    pt_top_layout = new QHBoxLayout;
    pt_select_type = new QComboBox();
    pt_select_type->addItem("Tous les types");
    pt_select_type->addItems(type_name_list);
    pt_top_layout->addWidget(pt_select_type);
    pt_top_layout->setAlignment(Qt::AlignLeft);

    patterns_view = new QTableView;
    patterns_view->verticalHeader()->hide();
    patterns_view->setModel(patterns_model);
    patterns_view->resizeColumnToContents(0);
    patterns_view->setEditTriggers(QTableView::NoEditTriggers);
    patterns_view->setSelectionBehavior(QAbstractItemView::SelectRows);
    patterns_view->setSelectionMode(QAbstractItemView::SingleSelection);

    pt_bot_layout = new QHBoxLayout;
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

void AnalyserConfWindow::init_rtab()
{
    rt_layout = new QVBoxLayout;

    results_view = new QTableView;
    results_view->verticalHeader()->hide();
    results_view->setModel(results_model);
    results_view->setEditTriggers(QTableView::NoEditTriggers);
    results_view->setSelectionMode(QAbstractItemView::NoSelection);
    results_view->horizontalHeader()->resizeSection(0, 340);
    results_view->resizeRowsToContents();

    rt_but_lay = new QHBoxLayout;
    rt_redo_analyze_but = new QPushButton("Refaire l'analyse");
    rt_new_sample_but = new QPushButton("Nouvel échantillon");
    rt_but_lay->addWidget(rt_redo_analyze_but);
    rt_but_lay->addWidget(rt_new_sample_but);

    rt_layout->addWidget(results_view);
    rt_layout->addLayout(rt_but_lay);

    results_tab->setLayout(rt_layout);
}

void AnalyserConfWindow::load_patterns(QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        QMessageBox::critical(this, "Erreur", QString("Impossible d'ouvrir le fichier ") + PATTERNS_FILENAME);
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
        patterns_model->insertRow(rc);
        patterns_model->setItem(rc, 0, new QStandardItem(list[0]));
        id = list[1].toInt();
        idx = type_id_list.indexOf(id);
        type = type_name_list[idx];
        patterns_model->setItem(rc, 1, new QStandardItem(type));
    }
    patterns_modified = false;
}

void AnalyserConfWindow::load_results()
{
    QSqlDatabase db = QSqlDatabase::database();
    QSqlQuery query;
    int i = 0;
    if (!db.open())
    {
        return;
    }
    query.exec("select contenu from corpus1 where haineux is not null order by random() limit 100;");
    while(query.next())
    {
        results_model->insertRow(i);
        results_model->setItem(i, 0, new QStandardItem(query.value(0).toString()));
        results_model->setItem(i++, 1, new QStandardItem(""));

    }
    db.close();
}

void AnalyserConfWindow::analyze_results()
{
    QString message, pt_regex, type_string;
    QRegExp regex;
    QSet<QString> set;
    bool hateful;
    for (int i = 0; i < 100; i++)
    {
        hateful = false;
        set.clear();
        message = results_model->item(i, 0)->text();
        for (int j = 0; j < patterns_model->rowCount(); j++)
        {
            pt_regex = patterns_model->item(j, 0)->text();
            regex.setPattern(pt_regex);
            if (message.contains(regex))
            {
                set << patterns_model->item(j, 1)->text();
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
            QSetIterator<QString> it(set);
            while(it.hasNext())
            {
                type_string.append(it.next() + ", ");
            }
            type_string.chop(2);
            results_model->setItem(i, 1, new QStandardItem(type_string));
        }

    }
}

void AnalyserConfWindow::new_sample()
{
    load_results();
    analyze_results();
}

void AnalyserConfWindow::add_pattern()
{
    patterns_modified = true;
    QList<QStandardItem*> list;
    list << new QStandardItem("Nouveau patron") << new QStandardItem("");
    patterns_model->appendRow(list);
}

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

void AnalyserConfWindow::open_mod_pattern_win()
{
    QItemSelectionModel *select = patterns_view->selectionModel();
    if (!select->hasSelection())
    {
        return;
    }
    QModelIndex idx = select->currentIndex();
    PatternWindow *w = new PatternWindow(this, idx.row());
    w->show();
}

void AnalyserConfWindow::modify_pattern(int row, QString regex, QString type)
{
    patterns_modified = true;
    patterns_model->setItem(row, 0, new QStandardItem(regex));
    patterns_model->setItem(row, 1, new QStandardItem(type));
}

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
        int idx = type_name_list.indexOf(patterns_model->item(i, 1)->text());
        stream << patterns_model->item(i, 0)->text() << "\t"
               << type_id_list[idx] << endl;
    }
    file.close();
    patterns_modified = false;
}

void AnalyserConfWindow::show_patterns_by_type(QString type)
{
    for (int i = 0; i < patterns_model->rowCount(); i++)
    {
        if (type == "Tous les types" || patterns_model->item(i, 1)->text() == type)
        {
            patterns_view->showRow(i);
        }
        else
        {
            patterns_view->hideRow(i);
        }
    }
}

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
