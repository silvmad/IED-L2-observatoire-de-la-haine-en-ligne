#ifndef SCRAPERCONFWINDOW_H
#define SCRAPERCONFWINDOW_H

#include <QWidget>
#include <QBoxLayout>
#include <QFormLayout>
#include <QLabel>
#include <QLineEdit>
#include <QSpinBox>
#include <QComboBox>
#include <QPushButton>
#include <QDialog>

#include "confarray.h"

class ScraperConfWindow : public QDialog
{
    Q_OBJECT
public:
    explicit ScraperConfWindow(QWidget *parent = nullptr);

signals:

public slots:
    void save_values();

private:
    ConfArray conf;
    QVBoxLayout *main_layout;
    QLabel *twit_api_lab;
    QFormLayout *twit_api_lay;
    QLineEdit *key;
    QLineEdit *skey;
    QLineEdit *tok;
    QLineEdit *stok;
    QLabel *search_opt_lab;
    QFormLayout *search_opt_lay;
    QSpinBox *sopt_tpr;
    QComboBox *sopt_lang;
    QSpinBox *sopt_itv;
    QHBoxLayout *but_lay;
    QPushButton *save_but;
    QPushButton *close_but;
};

#endif // SCRAPERCONFWINDOW_H
