#ifndef ANALYSERTAB_H
#define ANALYSERTAB_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

class AnalyserTab : public QWidget
{
    Q_OBJECT
public:
    explicit AnalyserTab(QWidget *parent = nullptr);
    void set_label_started();
    void set_label_stopped();

signals:

public slots:
    void open_conf_window();

private :
    QVBoxLayout *gen_layout;
    QLabel *top_label;
    QHBoxLayout *top_button_layout;
    QPushButton *conf_button;
    //QFrame *hline;
    QLabel *bot_label;
    QHBoxLayout *bot_buttons_layout;
    QPushButton *run_button, *stop_button;
};

#endif // ANALYSERTAB_H
