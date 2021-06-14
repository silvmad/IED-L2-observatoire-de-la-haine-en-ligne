#ifndef ANALYSERCONFWINDOW_H
#define ANALYSERCONFWINDOW_H

#include <QWidget>
#include <QTabWidget>
#include <QGridLayout>
#include <QComboBox>
#include <QPushButton>
#include <QLabel>
#include <QScrollArea>
#include <QDialog>
#include <QStandardItemModel>
#include <QTableView>
#include <QHeaderView>

#define PATTERNS_FILENAME "../../patterns"

/* Fenêtre de configuration de l'analyseur.
 * Elle contient deux onglet, l'un pour éditer les patrons et l'autre pour les
 * tester sur un échantillon de messages.
 * L'onglet patrons permet de charger, les patrons et les types ainsi que
 * d'ajouter, supprimer ou modifier des types et des patrons.
 * L'onglet résultats permet de tester les patrons sur un échantillon de
 * messages de la base de données. Il y est possible de changer d'échantillon
 * ou de recommencer l'analyse après avoir modifié les patrons. */
class AnalyserConfWindow : public QDialog
{
    Q_OBJECT

    friend class TestPatternWindow;
    friend class TestTypesWindow;
    friend class TestAnalyserConfWindow;
public:
    AnalyserConfWindow();
    void load_types();
    void init_ptab();
    void init_rtab();
    void load_patterns(QString);
    QList<int> get_type_id_list();
    QStringList get_type_name_list();
    QStandardItemModel* get_patterns_model();

signals:

public slots:
    void add_pattern();
    void suppr_pattern();
    void open_mod_pattern_win();
    void open_types_win();
    void modify_pattern(int, QString, QString);
    void save_patterns();
    void reject();
    void show_patterns_by_type(QString);
    void reload_types();

    void load_results();
    void analyze_results();
    void new_sample();

private:
    QList<int> type_id_list;
    QStringList type_name_list;

    QStandardItemModel *patterns_model;
    QStandardItemModel *results_model;

    bool patterns_modified = false;

    //fenêtre principale
    QVBoxLayout *gen_layout;
    QWidget *patterns_tab;
    QWidget *results_tab;
    QPushButton *close_but;
    QPushButton *save_but;

    //onglet patrons.
    QTableView *patterns_view;
    QComboBox *pt_select_type;
    QPushButton *pt_add_pattern_button;
    QPushButton *pt_modify_pattern_button;
    QPushButton *pt_suppr_pattern_button;
    QPushButton *pt_types_edit_but;

    //onglet résultats.
    QTableView *results_view;
    QPushButton *rt_redo_analyze_but;
    QPushButton *rt_new_sample_but;
};

#endif // ANALYSERCONFWINDOW_H
