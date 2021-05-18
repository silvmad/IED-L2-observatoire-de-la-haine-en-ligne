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

#define KW_FILENAME "keywords"

class KeyWordsWindow : public QDialog
{
    Q_OBJECT
public:
    explicit KeyWordsWindow(QWidget *parent = nullptr);
    void load_keywords();

signals:

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

    QVBoxLayout *main_layout;
    //QFormLayout *kw_lay;
    QHBoxLayout *but_lay;
    QPushButton *save_but;
    QPushButton *close_but;
    QPushButton *add_kw_but;
};

#endif // KEYWORDSWINDOW_H
