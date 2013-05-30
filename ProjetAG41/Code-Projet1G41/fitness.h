#ifndef _FITNESS_H
#define _FITNESS_H

#include <stdlib.h>
#include <limits.h>
#include "pointTest.h"
#include "secteur.h"

//class pointTest;
//class secteur;

class Fitness
{
    public:

/*
    static double eval(int nb_stable, secteur** lesSecteurA, int nb_secteur_a, secteur* s) {
    	double fit=0.0;
		for (int j=0; j<nb_stable; j++){
			// Calcul completement le SINR et réalise une association au best SINR
			s->calcul_sinr();
			// mise à jour des secteurs : bande necessaire, taux d'occupation du canal
			secteur::miseAjour(lesSecteurA, nb_secteur_a);
			fit += secteur::secteurNonCouvert->get_nb_clients_total();
		}
		return fit/nb_stable;
    };
*/
/*
    static double eval(int nb_stable, pointTest** lesPTA, int nb_tp_a, secteur** lesSecteurA, int nb_secteur_a) {
    	double fit=0.0;
		for (int j=0; j<nb_stable; j++){
			// Calcul completement le SINR et réalise une association au best SINR
			pointTest::calcul_sinr(lesPTA, nb_tp_a);
			// mise à jour des secteurs : bande necessaire, taux d'occupation du canal
			secteur::miseAjour(lesSecteurA, nb_secteur_a);
			fit += secteur::secteurNonCouvert->get_nb_clients_total();
		}
		return fit/nb_stable;
    };
*/

    static double eval(int nb_stable, pointTest** lesPTA, int nb_tp_a, secteur** lesSecteurA, int nb_secteur_a, int no_scen=-1) {
    	double fit=0.0;
		for (int j=0; j<nb_stable; j++){
			// Calcul completement le SINR et réalise une association au best SINR
			pointTest::calcul_sinr(lesPTA, nb_tp_a);
			// mise à jour des secteurs : bande necessaire, taux d'occupation du canal
			secteur::miseAjour(lesSecteurA, nb_secteur_a);
            if (no_scen==-1)
                fit += secteur::secteurNonCouvert->get_nb_clients_total();
            else
                fit += secteur::secteurNonCouvert->get_clients(no_scen);
		}

		if (no_scen==-1) return (fit/nb_stable);
		else return fit/nb_stable;
    };

/// Evaluation de la robustesse moyenne

    static double eval_mean(int nb_stable, pointTest** lesPTA, int nb_tp_a, secteur** lesSecteurA, int nb_secteur_a, int no_scen=-1) {
        double fit=0.0;
        double proba = 1/3;
		for (int j=0; j<nb_stable; j++){
			// Calcul completement le SINR et réalise une association au best SINR
			pointTest::calcul_sinr(lesPTA, nb_tp_a);
			// mise à jour des secteurs : bande necessaire, taux d'occupation du canal
			secteur::miseAjour(lesSecteurA, nb_secteur_a);
            if (no_scen==-1)
                fit += secteur::secteurNonCouvert->get_nb_clients_total();
            else
                fit += secteur::secteurNonCouvert->get_clients(no_scen);
		}
		return (fit/nb_stable)* proba;
    };

/// Evaluation de la robustesse absolue

static double eval_absolute(int nb_stable, pointTest** lesPTA, int nb_tp_a, secteur** lesSecteurA, int nb_secteur_a, int no_scen=-1) {
                double fit=0.0;
                double proba = 1/3;
                fit += secteur::secteurNonCouvert->get_clients(no_scen);
		return ((fit/nb_stable)* proba);
    };

/// Evaluation deviation absolue
 static double eval_absoluteDeviation(int nb_stable, pointTest** lesPTA, int nb_tp_a, secteur** lesSecteurA, int nb_secteur_a, int no_scen=-1) {
        double fit=0.0;
        double proba = 1/3;
		for (int j=0; j<nb_stable; j++){
			// Calcul completement le SINR et réalise une association au best SINR
			pointTest::calcul_sinr(lesPTA, nb_tp_a);
			// mise à jour des secteurs : bande necessaire, taux d'occupation du canal
			secteur::miseAjour(lesSecteurA, nb_secteur_a);
            if (no_scen==-1)
                fit += secteur::secteurNonCouvert->get_nb_clients_total();
            else
                fit += secteur::secteurNonCouvert->get_clients(no_scen);
		}
		return (fit/nb_stable)* proba;
    };

/*
    static int eval_2(int nb_stable, pointTest** lesPTA, int nb_tp_a, secteur** lesSecteurA, int nb_secteur_a) {
    	int fit, fmax=INT_MIN;
		for (int j=0; j<nb_stable; j++){
			// Calcul completement le SINR et réalise une association au best SINR
			pointTest::calcul_sinr(lesPTA, nb_tp_a);
			// mise à jour des secteurs : bande necessaire, taux d'occupation du canal
			secteur::miseAjour(lesSecteurA, nb_secteur_a);
			fit = secteur::secteurNonCouvert->get_nb_clients_total();
//cout << j << " " << fit << endl;
			if (fmax<fit) fmax=fit;
		}
		return fmax;
    };
    static int eval(int nb_stable, pointTest** lesPTA, int nb_tp_a, secteur** lesSecteurA, int nb_secteur_a) {
		eval_2(nb_stable, lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a);
		return eval_2(3, lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a);
//		return eval_3(nb_stable, lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a);
    };
    */
};

# endif

