
#include "optimisation1.h"
#include "random.h"

using namespace std;

void optimisation::descente_aleatoire_frequence(int stable,
											 pointTest** lesPTA, int nb_tp_a,
											 secteur** lesSecteurA, int nb_secteur_a, int no_scen) {
	double nb_clients_non_couvert = 0.0;
	double best_nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
	double best_nb_clients_non_couvert2 = best_nb_clients_non_couvert+1;
    int nbIteration = 10;
	//while (best_nb_clients_non_couvert != best_nb_clients_non_couvert2) {
		//best_nb_clients_non_couvert2 = best_nb_clients_non_couvert;
		int nb = 0;
      while (nb <= nbIteration){
          	srand (time (NULL));
          	int aleaSite = rand()/35;
		for (int i=0; i<site::size; i++) {
			cout << "site " << i << " nb sect = " << site::lesSites[i]->get_nb_secteur() << endl;
			if ((aleaSite== i)&&(site::lesSites[i]->get_nb_secteur() == 1)) {
				int old_p = site::lesSites[i]->get_secteur(0)->get_porteuse();
				int best_p = old_p;
	//			best_nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a);
				for (int p=0; p<Environement::nbProteuses; p++) {
					if (p != old_p) {
					    site::lesSites[i]->get_secteur(0)->set_porteuse(p);
//						nb_clients_non_couvert = Fitness::eval(stable,lesSecteurA, nb_secteur_a, site::lesSites[i]->get_secteur(0));
                        nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
						if (best_nb_clients_non_couvert>nb_clients_non_couvert) {
							cout << best_nb_clients_non_couvert << "Nbre clients non couverts -> " << nb_clients_non_couvert << endl;
							best_nb_clients_non_couvert=nb_clients_non_couvert;
							best_p = p;
						}
					}
				}
				if (best_p != Environement::nbProteuses-1)
					site::lesSites[i]->get_secteur(0)->set_porteuse(best_p);
			}
			else ((aleaSite== i)&&(site::lesSites[i]->get_nb_secteur() <> 1)) {
				int k = 0, l = 1;
				int permutation = 3;
				int nb_permutations = MathB::fact(permutation)-1;
				int *best_p = (int*)malloc(permutation*sizeof(int));
				int *les_p = (int*)malloc(permutation*sizeof(int));
				for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++)
					les_p[j] = site::lesSites[i]->get_secteur(j)->get_porteuse();
				if (site::lesSites[i]->get_nb_secteur()==2) les_p[2] = 2;
				for (int j=0; j<permutation; j++)
					best_p[j] = les_p[j];
	//			best_nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a);
				for (int ii=0; ii<nb_permutations; ii++) {
	//				cout << "indice : " << k << " " << l << endl;
					//cout << "freque : " << les_p[k] << " " << les_p[l] << endl;
					int tmp = les_p[k];
					les_p[k] = les_p[l];
					les_p[l] = tmp;
					if (site::lesSites[i]->get_nb_secteur()>k) site::lesSites[i]->get_secteur(k)->set_porteuse(les_p[k]);
					if (site::lesSites[i]->get_nb_secteur()>l) site::lesSites[i]->get_secteur(l)->set_porteuse(les_p[l]);
//					nb_clients_non_couvert = Fitness::eval(stable, lesSecteurA, nb_secteur_a, site::lesSites[i]->get_secteur(k));
					nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
//				cout << " -> " << nb_clients_non_couvert << endl;
					if (best_nb_clients_non_couvert>nb_clients_non_couvert) {
						cout << best_nb_clients_non_couvert << " --> " << nb_clients_non_couvert << endl;
						cout << "freq=" << les_p[0] << " " << les_p[1] << " " << les_p[2] << endl;
						best_nb_clients_non_couvert=nb_clients_non_couvert;
						for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++)
							best_p[j] = les_p[j];
					}
					// inverse k et l
					k = (k+1)%permutation;
					l = (l+1)%permutation;
				}
				for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++)
					site::lesSites[i]->get_secteur(j)->set_porteuse(best_p[j]);
	//			cout << "valid = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << endl;
	//			cout << "f=" << best_p[0] << " " << best_p[1] << " " << best_p[2] << endl;
				free(best_p);
				free(les_p);
			}
		}
//		cout << "valid = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << endl;
      for (int i=0; i<site::size; i++) {
      for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++){
					cout << "site " << i << " nb sect = " << site::lesSites[i]->get_nb_secteur() <<
					"freq" <<site::lesSites[i]->get_secteur(j)->get_porteuse()<< endl;

      }
      }
      nb ++;
	}

}
