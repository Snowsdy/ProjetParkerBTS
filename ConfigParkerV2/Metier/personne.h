#ifndef PERSONNE_H
#define PERSONNE_H

#include <QString>
#include <QStringList>
#include <QCryptographicHash>
#include <QDebug>

class Personne
{
public:
    Personne();
    virtual ~Personne();

    // SETTERS :
    void setId(int val);
    void setNom(QString val);
    void setPrenom(QString val);
    void setLogin(QString val);
    void setMdp(QString val);
    void setEncodedMdp(QString val);
    void setPerms(QString perms);
    void setStatus(char val);

    // GETTERS :
    int getId();
    QString getNom();
    QString getPrenom();
    QString getLogin();
    QString getMdp();
    QStringList getPermissions();
    char getStatus();

    // TOSTRING :
    void toString();

    // Verif correspondance mdp :
    static QString md5(QString val);

public:
    int id;
    QString nom;
    QString prenom;
    QString login;
    QString mdp;
    char status;
    QStringList permissions;

private:
    void encodeMd5(QString val);

};

#endif // PERSONNE_H
