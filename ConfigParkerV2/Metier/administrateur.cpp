#include "administrateur.h"

Administrateur::Administrateur()
{
    qWarning("Création Administrateur");
    this->status = 'A';
    this->permissions.append("MANAGE");
    this->permissions.append("CREATE");
    this->permissions.append("DELETE");
    this->permissions.append("USE");
    for (QString perm : this->permissions) {
        qWarning("Permissions : %s", perm.toLatin1().data());
    }
    qWarning("\n");
}

Administrateur::~Administrateur(){}
