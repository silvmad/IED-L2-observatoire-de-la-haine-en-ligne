#ifndef TESTANALYSEUR_H
#define TESTANALYSEUR_H

#include <QObject>

class TestAnalyseur : public QObject
{
    Q_OBJECT
/*public:
    explicit TestAnalyseur(QObject *parent = nullptr);*/

private slots:
    void test_load_patterns();
};

#endif // TESTANALYSEUR_H
