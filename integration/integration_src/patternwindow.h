#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include <QStandardItemModel>

/* Fenêtre permettant l'édition d'un patron.
 * Il est possible de modifier le patron ou de modifier le type de haine
 * associé.
 * row est la ligne du patron modifié dans le modèle des patrons.*/
class PatternWindow : public QDialog
{
    Q_OBJECT

    friend class TestPatternWindow;
public:
    PatternWindow(QWidget*, QStandardItemModel*, QStringList, int);
signals:
    void pattern_modification(int, QString, QString);

public slots:
    void validate_clicked();

private:
    int row;
    QLineEdit *regex_edit;
    QComboBox *types_box;
    QPushButton *validate_but;
};

#endif // PATTERNWINDOW_H
