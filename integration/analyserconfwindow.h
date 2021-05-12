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
#include "confarray.h"

#define PATTERNS_FILENAME QString("patterns")

typedef struct pattern {
    int row;
    QString regex;
    // L'index du type dans les listes type_id_list et type_name_list
    int type_idx;
    pattern();
    pattern(int);
} pattern;

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
    QList<pattern> get_patterns_list();

signals:

public slots:
    void add_pattern();
    void suppr_pattern();
    void open_mod_pattern_win();
    void modify_pattern(int, QString, QString);
    void save_patterns();
    void closeEvent(QCloseEvent*);
    void show_patterns_by_type(QString);

    void load_results();
    void analyze_results();
    void new_sample();

private:
    //ConfArray conf;
    QList<int> type_id_list;
    QStringList type_name_list;

    QStandardItemModel patterns_model;
    QList<pattern> patterns_list;
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
    QWidget *pt_mid_widget;
    QScrollArea *pt_mid_scroll;
    QGridLayout *pt_mid_layout;
    QHBoxLayout *pt_bot_layout;
    QComboBox *pt_select_type;
    QPushButton *pt_add_pattern_button;
    int grid_row_count = 0;

    //Widgets et layouts de l'onglet résultats.
    QVBoxLayout *rt_layout;
    QScrollArea *rt_scroll;
    QWidget *rt_scroll_widget;
    QVBoxLayout *rt_scroll_lay;
    QHBoxLayout *rt_but_lay;
    QPushButton *rt_redo_analyze_but;
    QPushButton *rt_new_sample_but;
};

#endif // ANALYSERCONFWINDOW_H
