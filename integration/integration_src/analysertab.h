#ifndef ANALYSERTAB_H
#define ANALYSERTAB_H

#include <QWidget>
#include <QLayout>
#include <QPushButton>
#include <QTextEdit>
#include <QLabel>

/* L'onglet analyseur de la fenêtre principale.
 * Permet de lancer ou d'arrêter l'analyseur et d'ouvrir la fenêtre de
 * configuration de l'analyseur. */
class AnalyserTab : public QWidget
{
    Q_OBJECT
    friend class TestAnalyserTab;
public:
    explicit AnalyserTab(QWidget *parent = nullptr);
    void set_label_started();
    void set_label_stopped();

signals:

public slots:
    void open_conf_window();

private :
    QLabel *bot_label;
};

#endif // ANALYSERTAB_H
