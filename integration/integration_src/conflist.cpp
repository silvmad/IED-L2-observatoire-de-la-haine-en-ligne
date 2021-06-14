#include "conflist.h"

using namespace std;

/* Constructeur de ConfList vide. */
ConfList::ConfList()
{
    loaded = false;
}

/* Constructeur de ConfList chargée avec les informations du fichier finelame.
 * Si le chargement est impossible, loaded est mis à false. */
ConfList::ConfList(const QString filename)
{
    if(load(filename) == 0)
    {
        loaded = true;
    }
    else
    {
        loaded = false;
    }
}

/* Charge les informations de configuration contenues dans le fichier filename.
 * Renvoie 0 si tout s'est bien passé et -1 si le fichier n'a pu être ouvert.
 */
int ConfList::load(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return -1;
    }
    QTextStream stream(&file);
    QString line;
    QStringList list;
    entry e;
    for(int i = 0; !stream.atEnd(); i++)
    {
        line = stream.readLine();
        list = line.split('\t');
        if (list.size() == 2)
        {
            e.name = list[0];
            e.value = list[1];
            entries << e;
        }
    }
    return 0;
}

/* Écrit les information contenues dans la ConfList dans un fichier.
 * filename est le chemin vers ce fichier.
 * Renvoie 0 si tout s'est bien passé et -1 si le fichier n'a pu être ouvert.
 * Le fichier doit exister, aucun fichier ne sera créé. */
int ConfList::write(const QString filename)
{
    QFile file(filename);
    if (!file.open(QIODevice::WriteOnly | QIODevice::Text | QIODevice::ExistingOnly))
    {
        return -1;
    }
    QTextStream stream(&file);
    for (int i = 0; i < entries.size(); i++)
    {
        stream << this->entries[i].name << "\t" << this->entries[i].value << endl;
    }
    return 0;
}

/* Renvoie la valeur correspondant à un champ donné en argument.
 * name est le nom du champ.
 * Si un champ avec le nom fourni en argument existe, la valeur correspondante
 * est renvoyée.
 * Sinon nullptr est renvoyé. */
QString ConfList::getValue(QString name)
{
    for(int i = 0; i < entries.size(); i++)
    {
        if (this->entries[i].name == name)
        {
            return this->entries[i].value;
        }
    }
    return nullptr;
}

/* Écrit une valeur pour un champ donné.
 * name est le nom du champ.
 * value est la valeur à écrire.
 * Renvoie 0 si tout s'est bien passé et -1 si aucun champ ne correspond à
 * celui donné en argument. */
int ConfList::setValue(QString name, QString value)
{

    for(int i = 0; i < entries.size(); i++)
    {
        if (this->entries[i].name == name)
        {
            this->entries[i].value = value;
            return 0;
        }
    }
    return -1;
}

/* Renvoie l'attribut loaded. */
bool ConfList::is_loaded()
{
    return loaded;
}
