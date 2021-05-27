#ifndef PATTERNWINDOW_H
#define PATTERNWINDOW_H

#include <QWidget>
#include <QBoxLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QPushButton>
#include "analyserconfwindow.h"

class PatternWindow : public QDialog
{
    Q_OBJECT
public:
    PatternWindow(AnalyserConfWindow*, int);

signals:
    void pattern_modification(int, QString, QString);

public slots:
    void validate_clicked();

private:
    int row;
    AnalyserConfWindow *conf_w;
    QVBoxLayout *gen_lay;
    QHBoxLayout *but_lay;
    QLineEdit *regex_edit;
    QComboBox *types_box;
    QPushButton *cancel_but;
    QPushButton *validate_but;
};

#endif // PATTERNWINDOW_H
