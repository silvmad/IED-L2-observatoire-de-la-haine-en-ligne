#ifndef KEYWORDSWINDOW_H
#define KEYWORDSWINDOW_H

#include <QWidget>
#include <QDialog>
#include <QBoxLayout>
#include <QFormLayout>
#include <QPushButton>
#include <QScrollArea>
#include <QStringListModel>
#include <QListView>

#include "mainwindow.h"

#define KW_FILENAME "../../keywords"

/* Fenêtre des mots-clés.
 * Permet l'ajout, la suppression et l'édition des mots-clés.
 * Les mots-clés sont chargés depuis un fichier et comparés à ceux enregistrés
 * dans la base de données. S'il y a des différences la base de données est
 * mise à jour pour correspondre au fichier. */
class KeyWordsWindow : public QDialog
{
    Q_OBJECT

    friend class TestKeywordsWindow;
public:
    //KeyWordsWindow(MainWindow *w);
    explicit KeyWordsWindow(QWidget *parent = nullptr);
    void load_keywords(QString);
    static int synchronize_db(QStringList, QStringList list = QStringList());
    static int load_kw_from_file(QStringList&, QString);
    static int load_kw_from_db(QStringList&);

public slots:
    void add_kw();
    void suppr_kw();
    void save_kws();
    void reject();
    void list_model_data_changed();

private:
    QStringListModel *list_model;
    QListView *view;
    bool unsaved_mod;
};

#endif // KEYWORDSWINDOW_H
