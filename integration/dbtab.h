#ifndef DBTAB_H
#define DBTAB_H

#include <QWidget>
#include <QPushButton>
#include <QFormLayout>
#include <QLineEdit>
#include <QtSql/QSqlDatabase>
#include <QMessageBox>
#include "confarray.h"

class DBTab : public QWidget
{
    Q_OBJECT
public:
    explicit DBTab(QWidget *parent = nullptr);

signals:

public slots:
    void save_values();

private:
    QLineEdit *hostname;
    QLineEdit *username;
    QLineEdit *user_pw;
    QLineEdit *dbname;
    QPushButton *save_button;
    QVBoxLayout *main_layout;
    QHBoxLayout *button_layout;
    QFormLayout *db_layout;
    ConfArray conf;
};

#endif // DBTAB_H
