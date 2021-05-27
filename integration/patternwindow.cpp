#include "patternwindow.h"
#include <QMessageBox>

PatternWindow::PatternWindow(AnalyserConfWindow *w, int row)
{
    setWindowTitle("Patron");
    this->row = row;
    this->setModal(true);
    conf_w = w;
    QStandardItemModel *patterns_model = w->get_patterns_model();
    QStringList type_name_list = w->get_type_name_list();
    QList<int> type_id_list = w->get_type_id_list();
    QString regex = patterns_model->item(row, 0)->text();
    QString type_name = patterns_model->item(row, 1)->text();

    types_box = new QComboBox;
    if (type_name == "")
    {
        type_name = "Choisir un type";
        types_box->addItem("Choisir un type");
    }
    gen_lay = new QVBoxLayout;
    but_lay = new QHBoxLayout;
    regex_edit = new QLineEdit;
    regex_edit->setText(regex);
    types_box->addItems(type_name_list);
    types_box->setCurrentText(type_name);
    cancel_but = new QPushButton("Annuler");
    validate_but = new QPushButton("Valider");

    but_lay->addWidget(cancel_but);
    but_lay->addWidget(validate_but);
    but_lay->setAlignment(Qt::AlignRight);

    gen_lay->addWidget(regex_edit);
    gen_lay->addWidget(types_box);
    gen_lay->addLayout(but_lay);

    setLayout(gen_lay);
    this->setGeometry(w->x() + 25, w->y() - 25, 450, 100);

    QObject::connect(cancel_but, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(validate_but, SIGNAL(clicked()), this, SLOT(validate_clicked()));
    QObject::connect(this, SIGNAL(pattern_modification(int, QString, QString)), w, SLOT(modify_pattern(int, QString, QString)));
}

void PatternWindow::validate_clicked()
{
    QString box_text = types_box->currentText();
    if (box_text == "Choisir un type")
    {
        QMessageBox::critical(this, "Validation impossible", "Vous devez choisir un type pour valider votre pattern.");
        return;
    }
    else
    {
        emit pattern_modification(row, regex_edit->text(), box_text);
    }
    this->close();
}
