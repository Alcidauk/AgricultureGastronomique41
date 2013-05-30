#include<time.h>
#include "Optimaleatoire.h"
#include "random.h"
#include "fitness.h"

using namespace std;

void Optimaleatoire::descente_aleatoire_frequence(char* nom, int stable,
											 pointTest** lesPTA, int nb_tp_a,
											 secteur** lesSecteurA, int nb_secteur_a, int aleaSite1, int no_scen) {
	GOutputFile file_sortie(nom);
    file_sortie.open();
	//double best_nb_clients_non_couvert = 0.0;
	double best_nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
	double nb_clients_non_couvert = 1000;
    //int nbIteration = 5;
    //clock_t startTime=clock();
	//while (best_nb_clients_non_couvert != best_nb_clients_non_couvert2) {
		//best_nb_clients_non_couvert2 = best_nb_clients_non_couvert;
		//srand (time (NULL));
		//double elapsedTime=0;
		//int nb = 0;
      //while (elapsedTime<1200){
          	//int aleaSite1 = rand()%35;
          	//int aleaSite2 = rand()%35;

          	//cout << "le site a modifier " << aleaSite1 << " etttttttttttt "<< aleaSite2 << endl;
		for (int i=0; i<site::size; i++) {

			if ((aleaSite1== i)&&(site::lesSites[i]->get_nb_secteur() == 1)) {
				int old_p = site::lesSites[i]->get_secteur(0)->get_porteuse();
				int best_p = old_p;
	//			best_nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a);
				for (int p=0; p<Environement::nbProteuses; p++) {
					if (p != old_p) {
					    site::lesSites[i]->get_secteur(0)->set_porteuse(p);
//						nb_clients_non_couvert = Fitness::eval(stable,lesSecteurA, nb_secteur_a, site::lesSites[i]->get_secteur(0));
                        nb_clients_non_couvert = Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, no_scen);
						if (best_nb_clients_non_couvert>nb_clients_non_couvert) {
                        cout<< "site " << i << " nb sect = " << site::lesSites[i]->get_nb_secteur() << endl;
						cout<< best_nb_clients_non_couvert << " --> " << nb_clients_non_couvert << endl;
						cout<< "freq=" << p << endl;
                        file_sortie << "non couverts t = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << "\n";
                        file_sortie << "non couverts 1 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << "\n";
                        file_sortie << "non couverts 2 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << "\n";
                        file_sortie << "non couverts 3 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << "\n";

                        cout << "non couverts t = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << endl;
                        cout << "non couverts 1 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << endl;
                        cout << "non couverts 2 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << endl;
                        cout << "non couverts 3 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << endl;
					    file_sortie << "site " << i << " nb sect = " << site::lesSites[i]->get_nb_secteur() << "\n";
						file_sortie << (float)best_nb_clients_non_couvert << " --> " << (float)nb_clients_non_couvert << "\n";
						file_sortie << "freq=" << p << "\n";
							best_nb_clients_non_couvert=nb_clients_non_couvert;
							best_p = p;
						}
					}
				}
				if (best_p != Environement::nbProteuses-1)
					site::lesSites[i]->get_secteur(0)->set_porteuse(best_p);
			}
			else if ((aleaSite1== i)&&(site::lesSites[i]->get_nb_secteur() > 1)) {
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
					    cout<< "site " << i << " nb sect = " << site::lesSites[i]->get_nb_secteur() << endl;
						cout<< best_nb_clients_non_couvert << " --> " << nb_clients_non_couvert << endl;
						cout<< "freq=" << les_p[0] << " " << les_p[1] << " " << les_p[2] << endl;
                        file_sortie << "non couverts t = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << "\n";
                        file_sortie << "non couverts 1 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << "\n";
                        file_sortie << "non couverts 2 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << "\n";
                        file_sortie << "non couverts 3 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << "\n";

                        cout << "non couverts t = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a) << endl;
                        cout << "non couverts 1 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 0) << endl;
                        cout << "non couverts 2 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 1) << endl;
                        cout << "non couverts 3 = " << Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 2) << endl;
					    file_sortie << "site " << i << " nb sect = " << site::lesSites[i]->get_nb_secteur() << "\n";
						file_sortie << (float)best_nb_clients_non_couvert << " --> " << (float)nb_clients_non_couvert << "\n";
						file_sortie << "freq=" << les_p[0] << " " << les_p[1] << " " << les_p[2] << "\n";
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
      /*for (int i=0; i<site::size; i++) {
      for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++){
					cout << "site " << i << " nb sect = " << site::lesSites[i]->get_nb_secteur() <<
					"freq" <<site::lesSites[i]->get_secteur(j)->get_porteuse()<< endl;

      }
      }*/

      //elapsedTime=(clock()-startTime) / (double)CLOCKS_PER_SEC;
     // nb ++;
	//}

file_sortie.close();
    }



