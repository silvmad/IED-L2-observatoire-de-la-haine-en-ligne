#ifndef TYPESWINDOW_H
#define TYPESWINDOW_H

#include <QDialog>
#include <QStandardItemModel>
#include <QStringListModel>
#include <QListView>
#include "analyserconfwindow.h"

/* Fenêtre d'édition des types.
 * Affiche la liste des types dans une vue et permet d'ajouter de modifier ou
 * de supprimer des types.
 * Un type ne peut être supprimé que si aucun patron ne l'utilise. */
class TypesWindow : public QDialog
{
    Q_OBJECT
    friend class TestTypesWindow;
public:
    TypesWindow(AnalyserConfWindow *w, QStandardItemModel*, QStringList);

public slots:
    void add_type();
    void suppr_type();
    void save_types();
    void reject();
    void list_model_data_changed();

signals:
    void db_types_synced();

private:
    QStandardItemModel *pat_model;
    QStringListModel *types_model;
    QListView *view;
    bool unsaved_mod;
};

#endif // TYPESWINDOW_H
