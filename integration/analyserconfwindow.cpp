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
    //conf = ConfArray(CONF_FILENAME);
    load_types();
    patterns_model.setColumnCount(2);

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
    load_patterns(PATTERNS_FILENAME);
    load_results();
    analyze_results();

    tabs->addTab(patterns_tab, "Patrons");
    tabs->addTab(results_tab, "Résultats");
    gen_layout->addWidget(tabs);
    gen_layout->addLayout(but_layout);
    this->setGeometry(500,500,500,500);
    this->setModal(true);
    // Par précaution pour éviter les fuites de mémoire.
    this->setAttribute(Qt::WA_DeleteOnClose);

    QObject::connect(pt_add_pattern_button, SIGNAL(clicked()), this, SLOT(add_pattern()));
    QObject::connect(save_but, SIGNAL(clicked()), this, SLOT(save_patterns()));
    QObject::connect(close_but, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(pt_select_type, SIGNAL(currentTextChanged(QString)), this, SLOT(show_patterns_by_type(QString)));
    QObject::connect(rt_new_sample_but, SIGNAL(clicked()), this, SLOT(new_sample()));
    QObject::connect(rt_redo_analyze_but, SIGNAL(clicked()), this, SLOT(analyze_results()));
}

pattern::pattern()
{

}

pattern::pattern(int row)
{
    this->row = row;
    this->type_idx = -1;
}

void AnalyserConfWindow::load_types()
{
    QSqlDatabase db = QSqlDatabase::database();
    //QString hnm, unm, upw, dbn;
    QSqlQuery query;
    /*hnm = conf.getValue(DB_HOST);
    unm = conf.getValue(DB_USERNAME);
    upw = conf.getValue(DB_USER_PW);
    dbn = conf.getValue(DB_DBNAME);
    db.setHostName(hnm);
    db.setUserName(unm);
    db.setPassword(upw);
    db.setDatabaseName(dbn);*/
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

    pt_mid_scroll = new QScrollArea;
    pt_mid_widget = new QWidget;
    pt_mid_layout = new QGridLayout;
    pt_mid_layout->setSizeConstraint(QLayout::SetMinimumSize);
    pt_mid_widget->setLayout(pt_mid_layout);
    pt_mid_scroll->setWidget(pt_mid_widget);

    pt_bot_layout = new QHBoxLayout;
    pt_add_pattern_button = new QPushButton("Ajouter patron", this);
    pt_bot_layout->addWidget(pt_add_pattern_button);
    pt_bot_layout->setAlignment(Qt::AlignLeft);

    pt_layout->addLayout(pt_top_layout);
    pt_layout->addWidget(pt_mid_scroll);
    pt_layout->addLayout(pt_bot_layout);

    patterns_tab->setLayout(pt_layout);
}

void AnalyserConfWindow::init_rtab()
{
    rt_layout = new QVBoxLayout;

    rt_scroll = new QScrollArea;
    rt_scroll_lay = new QVBoxLayout;
    rt_scroll_widget = new QWidget;
    QLabel *lab;
    for (int i = 0; i < 100; i++)
    {
        lab = new QLabel;
        lab->setMaximumWidth(400);
        lab->setWordWrap(true);
        rt_scroll_lay->addWidget(lab);
    }
    rt_scroll_widget->setLayout(rt_scroll_lay);
    rt_scroll->setWidget(rt_scroll_widget);

    rt_but_lay = new QHBoxLayout;
    rt_redo_analyze_but = new QPushButton("Refaire l'analyse");
    rt_new_sample_but = new QPushButton("Nouvel échantillon");
    rt_but_lay->addWidget(rt_redo_analyze_but);
    rt_but_lay->addWidget(rt_new_sample_but);

    rt_layout->addWidget(rt_scroll);
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
    QString line;
    QStringList list;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        list = line.split('\t');
        add_pattern(list[0]);
        pattern *pt = &patterns_list[grid_row_count - 1];
        pt->regex = list[0];
        pt->type_idx = type_id_list.indexOf(list[1].toInt());
    }
    patterns_modified = false;
}

void AnalyserConfWindow::load_results()
{
    QSqlDatabase db = QSqlDatabase::database();
    //QString hnm, unm, upw, dbn;
    QSqlQuery query;
    QLabel *lab;
    int i = 0;
    /*hnm = conf.getValue(DB_HOST);
    unm = conf.getValue(DB_USERNAME);
    upw = conf.getValue(DB_USER_PW);
    dbn = conf.getValue(DB_DBNAME);
    db.setHostName(hnm);
    db.setUserName(unm);
    db.setPassword(upw);
    db.setDatabaseName(dbn);*/
    if (!db.open())
    {
        return;
    }
    query.exec("select contenu from corpus1 where haineux is not null order by random() limit 100;");
    while(query.next())
    {
        lab = qobject_cast<QLabel*>(rt_scroll_lay->itemAt(i++)->widget());
        lab->setText(query.value(0).toString());
    }
    rt_scroll_widget->adjustSize();
    db.close();
}

void AnalyserConfWindow::analyze_results()
{
    QString text;
    QLabel *lab;
    QRegExp regex;
    bool hateful = false;
    for (int i = 0; i < 100; i++)
    {
        lab = qobject_cast<QLabel*>(rt_scroll_lay->itemAt(i)->widget());
        text = lab->text();
        for (int j = 0; j < patterns_list.size(); j++)
        {
            regex.setPattern(patterns_list[j].regex);
            if (text.contains(regex))
            {
                lab->setStyleSheet("QLabel { background-color : red;}");
                hateful = true;
            }
        }
        if (!hateful)
        {
            lab->setStyleSheet("");
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
    this->patterns_modified = true;
    QLabel *line = new QLabel("Label numéro " + QString::number(grid_row_count));
    line->setFrameStyle(QFrame::Box);
    QPushButton *mod = new QPushButton("Modifier");
    QPushButton *suppr = new QPushButton("Supprimer");
    this->pt_mid_layout->addWidget(line, this->grid_row_count, 0);
    this->pt_mid_layout->addWidget(mod, this->grid_row_count, 1);
    this->pt_mid_layout->addWidget(suppr, this->grid_row_count, 2);
    patterns_list << *(new pattern(grid_row_count++));

    QObject::connect(suppr, SIGNAL(clicked()), this, SLOT(suppr_pattern()));
    QObject::connect(mod, SIGNAL(clicked()), this, SLOT(open_mod_pattern_win()));
}

void AnalyserConfWindow::add_pattern(QString regex)
{
    add_pattern();
    QLabel *lab = qobject_cast<QLabel*>(pt_mid_layout->itemAtPosition(grid_row_count - 1, 0)->widget());
    lab->setText(regex);
}

void AnalyserConfWindow::suppr_pattern()
{
    this->patterns_modified = true;
    int rep = QMessageBox::question(this, "Suppression", "Êtes-vous sûr de vouloir supprimer ce patron ?", QMessageBox::Yes | QMessageBox::No);
    if (rep == QMessageBox::No)
    {
        return;
    }
    QPushButton* pButton = qobject_cast<QPushButton*>(sender());
    if (pButton)
    {
        QGridLayout *layout = qobject_cast<QGridLayout*>(pButton->parentWidget()->layout());
        QWidget *widget;
        int idx, *row = new int, *pad = new int;
        idx = layout->indexOf(pButton);
        // Seule la ligne nous intéresse.
        layout->getItemPosition(idx, row, pad, pad, pad);

        widget = layout->itemAtPosition(*row, 0)->widget();
        widget->setParent(nullptr);
        delete widget;
        widget = layout->itemAtPosition(*row, 1)->widget();
        widget->setParent(nullptr);
        delete widget;
        widget = layout->itemAtPosition(*row, 2)->widget();
        widget->setParent(nullptr);
        delete widget;
        layout->update();
        this->pt_mid_widget->adjustSize();

        suppr_pattern_from_list(*row);
    }
}

void AnalyserConfWindow::suppr_pattern_from_list(int row)
{
    for (int i = 0; i < patterns_list.size(); i++)
    {
        if (patterns_list[i].row == row)
        {
            patterns_list.removeAt(i);
            return;
        }
    }
}

void AnalyserConfWindow::open_mod_pattern_win()
{
    PatternWindow *w = nullptr;
    QPushButton* but = qobject_cast<QPushButton*>(sender());
    QGridLayout *lay = qobject_cast<QGridLayout*>(but->parentWidget()->layout());
    int idx = lay->indexOf(but);
    int *row = new int, *pad = new int;
    lay->getItemPosition(idx, row, pad, pad, pad);
    for (int i = 0; i < patterns_list.size(); i++)
    {
        if (*row == patterns_list[i].row)
        {
            w = new PatternWindow(this, i);
            break;
        }
    }
    w->show();
}

void AnalyserConfWindow::modify_pattern(int pt_idx, QString regex, QString type)
{
    QLabel *lab;
    //QPushButton *but;
    int row = patterns_list[pt_idx].row;
    patterns_list[pt_idx].regex = regex;
    patterns_list[pt_idx].type_idx = type_name_list.indexOf(type);
    lab = qobject_cast<QLabel*>(pt_mid_layout->itemAtPosition(row, 0)->widget());
    lab->setText(regex);
    //but = qobject_cast<QPushButton*>(pt_mid_layout->itemAtPosition(row, 1))
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
    pattern pt;
    for (int i = 0; i < patterns_list.size(); i++)
    {
        pt = patterns_list[i];
        stream << pt.regex << "\t" << type_id_list[pt.type_idx] << endl;
    }
    file.close();
    patterns_modified = false;
}

void AnalyserConfWindow::show_patterns_by_type(QString type)
{
    pattern pt;
    int type_idx, row;
    if (type == "Tous les types")
    {
        type_idx = -1;
    }
    else
    {
        type_idx = type_name_list.indexOf(type);
        //type_id = type_id_list[idx];
    }
    for (int i = 0 ; i < patterns_list.size(); i++)
    {
        pt = patterns_list[i];
        row = pt.row;
        if (type_idx == -1 || pt.type_idx == type_idx)
        {
            pt_mid_layout->itemAtPosition(row, 0)->widget()->show();
            pt_mid_layout->itemAtPosition(row, 1)->widget()->show();
            pt_mid_layout->itemAtPosition(row, 2)->widget()->show();
        }
        else
        {
            pt_mid_layout->itemAtPosition(row, 0)->widget()->hide();
            pt_mid_layout->itemAtPosition(row, 1)->widget()->hide();
            pt_mid_layout->itemAtPosition(row, 2)->widget()->hide();
        }
    }
    pt_mid_widget->adjustSize();
}

void AnalyserConfWindow::closeEvent(QCloseEvent *event)
{
    if (patterns_modified)
    {
        int rep = QMessageBox::critical(this, "Fermeture de la fenêtre", "Les modifications des patrons ne seront pas enregistrées, êtes-vous sûr de vouloir fermer la fenêtre ?", QMessageBox::Yes | QMessageBox::No);
        if (rep == QMessageBox::No)
        {
            return;
        }
    }
    QWidget::closeEvent(event);
}

QList<int> AnalyserConfWindow::get_type_id_list()
{
    return type_id_list;
}

QStringList AnalyserConfWindow::get_type_name_list()
{
    return type_name_list;
}

QList<pattern> AnalyserConfWindow::get_patterns_list()
{
    return patterns_list;
}
