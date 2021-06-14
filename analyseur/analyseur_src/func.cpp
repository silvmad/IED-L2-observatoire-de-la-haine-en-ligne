#include <main.h>

/* Remplit le dictionnaire patterns avec le contenu du fichier patterns.
 * Renvoie -1 si l'ouverture du fichier échoue et 0 sinon.
 * patterns est le dictionnaire qui contiendra les patrons. Il associe un
 * entier à une liste de QString. Chaque int correspond à l'id d'un type et la
 * liste de QString aux patrons correspondant à ce type.
 * file_name le nom du fichier qui contient les patrons. */
int load_patterns(QMap<int, QStringList>& patterns, QString file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return -1;
    }
    QTextStream stream(&file);
    QString line;
    QStringList list;
    while (!stream.atEnd())
    {
        line = stream.readLine();
        list = line.split('\t');
        /* Précaution au cas où le fichier des patrons aurait été modifié sur
         * le disque et contiendrai de mauvaises valeurs. */
        if (list.size() == 2 && is_int(list[1]))
        {
            add_pattern(patterns, list[0], list[1].toInt());
        }
    }
    return 0;
}

/* Renvoie true si s est la représentation d'un entier sous forme de chaine de
 * caractères et false sinon.
 * Trouvé ici : https://stackoverflow.com/questions/2844817/how-do-i-check-if-a-c-string-is-an-int */
bool is_int(QString s)
{
    char *p;
    strtol(s.toStdString().c_str(), &p, 10);
    return (*p == 0);
}

/* Ajoute un patron au dictionnaire patterns.
 * Si le type du patron est déjà présent, le patron est ajouté à la liste
 * des patrons correspondant à ce type.
 * Sinon une nouvelle entrée est ajoutée au dictionnaire avec le nouveau
 * type comme clé et comme valeur une liste contenant le patron.
 * patterns est le dictionnaire qui contient les patrons.
 * regex est le patron à ajouter.
 * type est l'id du type du patron à ajouter. */
int add_pattern(QMap<int, QStringList>& patterns, QString regex, int type)
{
    if (!patterns.contains(type))
    {
        patterns.insert(type, QStringList());
    }
    patterns[type] << regex;
    return 0;
}

/* Remplit la liste keywords avec le contenu du fichier keywords.
 * Renvoie -1 si l'ouverture du fichier échoue et 0 sinon.
 * L'id des type est récupérée dans la base de données.
 * keywords est la liste des mot-clés.
 * file_name est le nom du fichier qui contient les mots-clés. */
int load_keywords(QList<keyword>& keywords, QString file_name)
{
    QFile file(file_name);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        return -1;
    }
    QTextStream stream(&file);
    QString word;
    QStringList list;
    QSqlQuery q;
    while (!stream.atEnd())
    {
        word = stream.readLine();
        keyword kw;
        kw.word = word;
        q.prepare("select id_mot_clé from mot_clé where mot = ?");
        q.addBindValue(word);
        q.exec();
        q.next();
        kw.id = q.value(0).toInt();
        keywords << kw;
    }
    return 0;
}

/* Crée la base de données avec les informations contenues dans le fichier de
 * configuration (obtenues à travers un ConfList) et l'ouvre.
 * c est le ConfList qui contient les informations de configuration. */
QSqlDatabase open_db(ConfList c)
{
    QSqlDatabase db = QSqlDatabase::addDatabase("QPSQL");
    db.setHostName(c.getValue(DB_HOST));
    db.setUserName(c.getValue(DB_USERNAME));
    db.setPassword(c.getValue(DB_USER_PW));
    db.setDatabaseName(c.getValue(DB_DBNAME));
    db.open();
    return db;
}

/* Analyse les résultats de la requète query et remplit les tables de la base
 * données en conséquence.
 * Chaque messages est d'abord comparé à chaque liste de patrons correspondant
 * à un type de haine pour en déterminer le caractère haineux avec a fonction
 * match.
 * Si un message matche un type, le message est marqué comme haineux et le
 * type correspondant est ajouté aux types du message (avec les fonctions
 * set_hateful et set_type).
 * Ensuite, si le message a été détécté comme haineux, les mots-clés sont
 * ensuite recherchés dans le message. Chaque mot-clé retrouvé dans le message
 * est ajouté à la liste des mots-clés de celui-ci (avec la fonction
 * set_keyword).
 * Si le message n'a pas été détecté comme haineux, il est marqué comme non
 * haineux avec la fonction set_hateful.
 * query est la requète dont les résultats sont à analyser.
 * patterns est le dicitonnaire des patrons.
 * keywords est la liste des mots-clés. */
void parse_result(QSqlQuery query, const QMap<int,QStringList>& patterns,
                  const QList<keyword>& keywords)
{
    bool hateful;
    QString text, id;
    QMap<int, QStringList>::const_iterator i;
    while (query.next())
    {
        hateful = false;
        id = query.value(0).toString();
        text = query.value(1).toString();
        for (i = patterns.begin(); i != patterns.end(); i++)
        {
            if (match(text, i.value()))
            {
                if (!hateful)
                {
                    hateful = true;
                    set_hateful(id, "true");
                }
                set_type(id, i.key());
            }
        }
        if (hateful)
        {
            for (int i = 0; i < keywords.size(); i++)
            {
                if (text.contains(keywords[i].word, Qt::CaseInsensitive))
                {
                    set_keyword(id, keywords[i].id);
                }
            }
        }
        else
        {
            set_hateful(id, "false");
        }
    }
}

/* Détermine si un message correspond à un type de haine donné.
 * text est le texte du message.
 * list est la liste des patrons du type concerné.
 * Le message est comparé à chaque patron, dès qu'un patron correspond au
 * message, la fonction renvoie le numéro du patron dans la liste (la
 * numérotation commence à 1).
 * Si aucun patron ne correspond elle renvoie 0. */
int match(QString text, QStringList list)
{
    QRegExp regex;
    for (int i = 0; i < list.size(); i++)
    {
        regex.setPattern(list[i]);
        regex.setCaseSensitivity(Qt::CaseInsensitive);
        if (text.contains(regex))
        {
            return i + 1;
        }
    }
    return 0;
}

/* Edite la base de données pour ajouter le caractère haineux d'un message.
 * id est l'id du message dans la table messages.
 * val est la valeur (true ou false) à donner au champ haineux de la table
 * pour ce message. */
void set_hateful(QString id, QString val)
{
    QSqlQuery update_hateful;
    update_hateful.prepare("update messages set haineux=:b where id=:id;");
    update_hateful.bindValue(":b", val);
    update_hateful.bindValue(":id", id);
    update_hateful.exec();
}

/* Edite la base de données pour ajouter un type de haine à un message.
 * Ajoute une entrée à la table possede qui fait correspondre l'id d'un
 * message avec l'id d'un type de haine.
 * type_id est l'id du message.
 * type est l'id du type */
void set_type(QString id, int type_id)
{
    QSqlQuery update_type;
    update_type.prepare("insert into possede values (:id, :type);");
    update_type.bindValue(":id", id);
    update_type.bindValue(":type", type_id);
    update_type.exec();
}

/* Edite la base de données pour ajouter un mot-clé à un message.
 * Ajoute une entrée à la table contient qui fait correspondre l'id d'un
 * message avec l'id d'un mot-clé.
 * id est l'id du message.
 * kw_id est l'id du mot-clé.
 */
void set_keyword(QString id, int kw_id)
{
    QSqlQuery update_kw;
    update_kw.prepare("insert into contient values (:id, :type);");
    update_kw.bindValue(":id", id);
    update_kw.bindValue(":type", kw_id);
    update_kw.exec();
}

/* Fonction qui gère la réception du signal pour sortir le programme
 * de sa pause.
 * Elle affiche juste un message et permet au programme de reprendre. */
void signal_handler(int sig)
{
    printf("Remise en route de l'analyse (reçu signal %d).\n", sig);
}
