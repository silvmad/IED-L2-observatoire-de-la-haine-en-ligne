#ifndef ONGLETSCRAPER_H
#define ONGLETSCRAPER_H

#include <QWidget>
#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>

class ScraperTab : public QWidget
{
    Q_OBJECT
public:
    explicit ScraperTab(QWidget *parent = nullptr);
    void set_label_started();
    void set_label_stopped();

signals:

public slots:
    void open_conf_win();
    void open_kw_win();

private:
    QVBoxLayout *main_layout;
    QHBoxLayout *top_but_lay;
    QHBoxLayout *bot_but_lay;
    QPushButton *conf_but;
    QPushButton *kw_but;
    QPushButton *start_but;
    QPushButton *stop_but;
    QLabel *top_label;
    QLabel *bot_label;
};

#endif // ONGLETSCRAPER_H
