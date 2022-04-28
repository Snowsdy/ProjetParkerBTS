#include "technicien.h"

Technicien::Technicien()
{
    qWarning("Création Technicien");
    this->status = 'T';
    this->permissions.append("MANAGE");
    this->permissions.append("USE");
    for (QString perm : this->permissions) {
        qWarning("Permissions : %s", perm.toLatin1().data());
    }
    qWarning("\n");
}

Technicien::~Technicien(){}
