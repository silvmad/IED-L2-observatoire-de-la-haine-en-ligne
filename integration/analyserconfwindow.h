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

#include "confarray.h"

#define PATTERNS_FILENAME QString("patterns")

class AnalyserConfWindow : public QDialog
{
    Q_OBJECT
public:
    AnalyserConfWindow();
    void load_types();
    void init_ptab();
    void init_rtab();
    void load_patterns(QString);
    void add_pattern(QString);
    void suppr_pattern_from_list(int);
    QList<int> get_type_id_list();
    QStringList get_type_name_list();
    QStandardItemModel* get_patterns_model();

signals:

public slots:
    void add_pattern();
    void suppr_pattern();
    void open_mod_pattern_win();
    void modify_pattern(int, QString, QString);
    void save_patterns();
    void reject();
    void show_patterns_by_type(QString);

    void load_results();
    void analyze_results();
    void new_sample();

private:
    QList<int> type_id_list;
    QStringList type_name_list;

    QStandardItemModel *patterns_model;
    QStandardItemModel *results_model;

    //QList<pattern> patterns_list;
    bool patterns_modified = false;

    // Widgets et layout de la fenêtre
    QVBoxLayout *gen_layout;
    QTabWidget *tabs;
    QWidget *patterns_tab;
    QWidget *results_tab;
    QHBoxLayout *but_layout;
    QPushButton *close_but;
    QPushButton *save_but;

    //Widgets et layouts de l'onglet patrons.
    QVBoxLayout *pt_layout;
    QHBoxLayout *pt_top_layout;
    QTableView *patterns_view;
    QHBoxLayout *pt_bot_layout;
    QComboBox *pt_select_type;
    QPushButton *pt_add_pattern_button;
    QPushButton *pt_modify_pattern_button;
    QPushButton *pt_suppr_pattern_button;

    //Widgets et layouts de l'onglet résultats.
    QVBoxLayout *rt_layout;
    QTableView *results_view;
/*    QScrollArea *rt_scroll;
    QWidget *rt_scroll_widget;
    QVBoxLayout *rt_scroll_lay;*/
    QHBoxLayout *rt_but_lay;
    QPushButton *rt_redo_analyze_but;
    QPushButton *rt_new_sample_but;
};

#endif // ANALYSERCONFWINDOW_H
