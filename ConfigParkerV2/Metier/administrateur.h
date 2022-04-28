#ifndef ADMINISTRATEUR_H
#define ADMINISTRATEUR_H

#include "personne.h"


class Administrateur : public Personne
{
public:
    Administrateur();
    ~Administrateur() override;
};

#endif // ADMINISTRATEUR_H
