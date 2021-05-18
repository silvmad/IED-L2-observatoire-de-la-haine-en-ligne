#ifndef TESTANALYSEUR_H
#define TESTANALYSEUR_H

#include <QObject>

class TestAnalyseur : public QObject
{
    Q_OBJECT
public:
    explicit TestAnalyseur(QObject *parent = nullptr);

signals:

public slots:
};

#endif // TESTANALYSEUR_H