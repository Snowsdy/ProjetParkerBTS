#include "personne.h"

Personne::Personne()
{
    this->status = 'P';
    this->permissions.append("USE");
}

Personne::~Personne()
{
    qWarning("Passage destructeur Personne");
}

void Personne::setId(int val)
{
    this->id = val;
}

QString Personne::getNom()
{
    return this->nom;
}

QString Personne::getPrenom()
{
    return this->prenom;
}

QString Personne::getLogin()
{
    return this->login;
}

QString Personne::getMdp()
{
    return this->mdp;
}

QStringList Personne::getPermissions()
{
    return this->permissions;
}

char Personne::getStatus()
{
    return this->status;
}

void Personne::toString()
{
    qWarning("Nom : %s", (strcmp(this->nom.toLatin1().data(), "") ? this->nom.toLatin1().data() : ""));
    qWarning("Prénom : %s", (strcmp(this->prenom.toLatin1().data(), "") ? this->prenom.toLatin1().data() : ""));
    qWarning("Login : %s", (strcmp(this->login.toLatin1().data(), "") ? this->login.toLatin1().data() : ""));
    qWarning("Mdp : %s", (strcmp(this->mdp.toLatin1().data(), "") ? this->mdp.toLatin1().data() : ""));
    qWarning("Status : %c", this->status);
    qWarning("Permissions :");
    for (QString p : this->permissions)
        qWarning("%s", p.toLatin1().data());
}

QString Personne::md5(QString val)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QByteArray input;
    input.append(val.toLatin1());
    hash.addData(input);

    return hash.result().toHex();
}

void Personne::encodeMd5(QString val)
{
    QCryptographicHash hash(QCryptographicHash::Md5);
    QByteArray input;
    input.append(val.toLatin1());
    hash.addData(input);
    this->mdp = hash.result().toHex();
}

void Personne::setNom(QString val)
{
    this->nom = val;
}

void Personne::setPrenom(QString val)
{
    this->prenom = val;
}

void Personne::setLogin(QString val)
{
    this->login = val;
}

void Personne::setMdp(QString val)
{
    encodeMd5(val);
}

void Personne::setEncodedMdp(QString val)
{
    this->mdp = val;
}

void Personne::setPerms(QString perms)
{
    this->permissions.clear();
    this->permissions = perms.split(',');
}

void Personne::setStatus(char val)
{
    this->status = val;
}

int Personne::getId()
{
    return this->id;
}
