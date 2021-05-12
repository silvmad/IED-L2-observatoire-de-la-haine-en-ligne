#include "confarray.h"

using namespace std;

ConfArray::ConfArray()
{

}

ConfArray::ConfArray(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        cout << "Erreur, impossible d'ouvrir le fichier." << endl;
    }
    QTextStream stream(&file);
    QString line;
    QStringList list;
    for(int i = 0; i < CONF_LINE_COUNT && !stream.atEnd(); i++)
    {
        line = stream.readLine();
        list = line.split('\t');
        this->entries[i].name = list[0];
        this->entries[i].value = list[1];
    }
}

int ConfArray::write(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        cout << "Erreur, impossible d'ouvrir le fichier." << endl;
        return 1;
    }
    QTextStream stream(&file);
    for (int i = 0; i < CONF_LINE_COUNT; i++)
    {
        stream << this->entries[i].name << "\t" << this->entries[i].value << endl;
    }
    return 0;
}

QString ConfArray::getValue(QString name)
{
    for(int i = 0; i < CONF_LINE_COUNT; i++)
    {
        if (this->entries[i].name == name)
        {
            return this->entries[i].value;
        }
    }
    return nullptr;
}

int ConfArray::setValue(QString name, QString value)
{

    for(int i = 0; i < CONF_LINE_COUNT; i++)
    {
        if (this->entries[i].name == name)
        {
            this->entries[i].value = value;
            return 0;
        }
    }
    return 1;
}
