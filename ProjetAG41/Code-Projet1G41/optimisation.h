#ifndef _OPTIMISATION_H
#define _OPTIMISATION_H

#include <stdlib.h>
#include <limits.h>
#include <vector>
#include "pointTest.h"
#include "secteur.h"
#include "fitness.h"
#include "Random.h"
#include "site.h"
#include "environement.h"


class pointTest;
class secteur;
class site;

class optimisation
{
    public:
    static void descente_simple_frequence(char* nom, int stable,
											 pointTest** lesPTA, int nb_tp_a,
											 secteur** lesSecteurA, int nb_secteur_a, int no_scen=-1);

   static void frequencyOptimization (char *nom, int stable,
										 pointTest** lesPTA, int nb_tp_a,
										 secteur** lesSecteurA, int nb_secteur_a, int no_scen=-1);
};

# endif
