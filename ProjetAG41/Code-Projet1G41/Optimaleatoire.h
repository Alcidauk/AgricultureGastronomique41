

#ifndef _OPTIMALEATOIRE_H
#define _OPTIMALEATOIRE_H

#include <stdlib.h>
#include <limits.h>
#include <string.h>
#include "pointTest.h"
#include "secteur.h"
#include "fitness.h"
#include "site.h"
#include "environement.h"

class pointTest;
class secteur;
class site;

class Optimaleatoire
{
    public:
    static void descente_aleatoire_frequence(char* nom, int stable,
											 pointTest** lesPTA, int nb_tp_a,
											 secteur** lesSecteurA, int nb_secteur_a, int aleaSite1, int no_scen=-1);
};

# endif
