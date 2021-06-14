#include "patternwindow.h"
#include <QMessageBox>

/* Constructeur pour la création de la fenêtre.
 * parent est la fenêtre parent (normalement une AnalyserConfWindow).
 * patterns_model est le modèle qui contient les patrons dans la fenêtre parent
 * type_name_list est la liste des types
 * row est la ligne du patron édité dans le modèle des patrons. */
PatternWindow::PatternWindow(QWidget *parent, QStandardItemModel *patterns_model, QStringList type_name_list,
                             int row)
{
    this->setWindowTitle("Patron");
    this->row = row;
    this->setModal(true);
    QString regex = patterns_model->item(row, 0)->text();
    QString type_name = patterns_model->item(row, 1)->text();

    types_box = new QComboBox;
    if (type_name == "")
    {
        type_name = "Choisir un type";
        types_box->addItem("Choisir un type");
    }
    QVBoxLayout *gen_lay = new QVBoxLayout;
    QHBoxLayout *but_lay = new QHBoxLayout;
    regex_edit = new QLineEdit;
    regex_edit->setText(regex);
    types_box->addItems(type_name_list);
    types_box->setCurrentText(type_name);
    QPushButton *cancel_but = new QPushButton("Annuler");
    validate_but = new QPushButton("Valider");

    but_lay->addWidget(cancel_but);
    but_lay->addWidget(validate_but);
    but_lay->setAlignment(Qt::AlignRight);

    gen_lay->addWidget(regex_edit);
    gen_lay->addWidget(types_box);
    gen_lay->addLayout(but_lay);

    setLayout(gen_lay);
    this->setGeometry(parent->x() + 25, parent->y() - 25, 450, 100);

    QObject::connect(cancel_but, SIGNAL(clicked()), this, SLOT(close()));
    QObject::connect(validate_but, SIGNAL(clicked()), this, SLOT(validate_clicked()));
    QObject::connect(this, SIGNAL(pattern_modification(int, QString, QString)), parent, SLOT(modify_pattern(int, QString, QString)));
}

/* Slot qui permet de fermer la fenêtre en conservant les modifications.
 * Si aucun type n'a été choisi, un avertissement est affiché et la fonction
 * retourne sans autre effet.
 * Sinon elle envoie le signal pattern_modification avec en argument la ligne
 * du patron dans le modèle, le nouveau texte du patron et le nouveau type.
 * Puis la fenêtre est fermée. */
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
