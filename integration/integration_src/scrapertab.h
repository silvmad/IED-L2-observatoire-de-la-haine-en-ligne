#ifndef ONGLETSCRAPER_H
#define ONGLETSCRAPER_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>

/* L'onglet du scraper de la fenêtre principale.
 * Permet de lancer / d'arrêter le scraper et d'ouvrir la fenêtre de
 * configuration du scraper et la fenêtre des mots-clés. */
class ScraperTab : public QWidget
{
    Q_OBJECT
    friend class TestScraperTab;
public:
    explicit ScraperTab(QWidget *parent = nullptr);
    void set_label_started();
    void set_label_stopped();

signals:

public slots:
    void open_conf_win();
    void open_kw_win();

private:
    QLabel *bot_label;
};

#endif // ONGLETSCRAPER_H
