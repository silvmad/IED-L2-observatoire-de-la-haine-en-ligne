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

#include "conflist.h"

/* Fenêtre de configuration du scraper.
 * Permet d'accéder aux information de configuration du scraper et de les
 * modifier. */
class ScraperConfWindow : public QDialog
{
    Q_OBJECT
    friend class TestScraperConfWindow;
public:
    explicit ScraperConfWindow(QWidget *parent = nullptr);

signals:

public slots:
    void save_values();

private:
    ConfList conf;
    QLineEdit *key;
    QLineEdit *skey;
    QLineEdit *tok;
    QLineEdit *stok;
    QSpinBox *sopt_tpr;
    QComboBox *sopt_lang;
    QSpinBox *sopt_itv;
    QSpinBox *sopt_rate_limit;
};

#endif // SCRAPERCONFWINDOW_H
