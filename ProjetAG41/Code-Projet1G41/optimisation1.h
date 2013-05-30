
#ifndef _OPTIMISATION1_H
#define _OPTIMISATION1_H

#include <stdlib.h>
#include <limits.h>
#include "pointTest.h"
#include "secteur.h"
#include "fitness.h"
#include "site.h"
#include "environement.h"

class pointTest;
class secteur;
class site;

class optimisation1
{
    public:
    static void descente_aleatoire_frequence(int stable,
											 pointTest** lesPTA, int nb_tp_a,
											 secteur** lesSecteurA, int nb_secteur_a, int no_scen=-1);

};

# endif
