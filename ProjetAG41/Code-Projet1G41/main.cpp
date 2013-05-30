#include <iostream>
#include <fstream>
#include <time.h>
#include <limits>

#include "pointTest.h"
#include "site.h"
#include "secteur.h"
#include "random.h"
#include "math_base.h"
#include "propagation.h"
#include "unite.h"
#include "version.h"
#include "fitness.h"
#include "optimisation.h"
#include "Optimaleatoire.h"
#include "gfile.h"
#include "SolFrequence.h"

using namespace std;


/** Affiche dans gnuplot la carte du sinr
    @param filename nom du fichier a lire par Gnuplot
    @param gplot fichier Gplot
*/
void gplot_diplay(FILE* gplot, char* filename) {
    fprintf(gplot,"load \"plot_matrix.txt\"\n");
    fprintf(gplot,"splot \"%s\" matrix\n", filename);
    fflush(gplot);
}


/** Utilise un fichier de configuration pour piloter le programme
*/
void load(const char* fileConfig){
	cout << ">> Chargement du module Twizy defini dans : "<<fileConfig<<"\n";
	GInputFile infile(fileConfig) ;

	infile.open();

	int tilt=-1;
	string outfilename;
	char* buf ;
    char *tok ;
    while ((buf=infile.readUncommentedLine())!=NULL)
    {
        if( (tok=infile.getNextToken())!=NULL ){
			if( strcmp(tok, "outfile")==0 ) {
					if( (tok=infile.getNextToken())!=NULL ){
						outfilename=tok;
					}
			}

			else if( strcmp(tok, "tilt")==0 ) {
				if( (tok=infile.getNextToken())!=NULL ){
					tilt = atoi(tok);
				}
			}
		}
	}

	infile.close();
	cout << "Les parametres sont : outfilename "<<outfilename<<" et tilt = "<<tilt<<"\n";
}
void load_stations_file(char* starios_File_name, int nb_sce, int nb_stationaOpt){

    GInputFile infile(starios_File_name);
    char* buf;
    int nb_stations = nb_stationaOpt;
    cout << "Chargement des stations et crenaux a optimiser" << endl;
    buf = infile.readLine();
    infile.getNextToken();
    int *stations = new int[7];
    buf = infile.readUncommentedLine();
    bool aOptimiser = false;
    for (int i=0;i<nb_stations; i++){
     buf = infile.readUncommentedLine();
     stations[i] = infile.getNextIntToken();
     //aOptimier = infile.getNextIntToken();
    }
}
//Solution initiale des frequences

// Varibles statiques d'Environement
int Environement::nbProteuses = 0;
//int Environement::pattern_reutilisation;
//initialisation du setcteur fictive des points de test non couverts

// Varibles statiques des secteurs
//secteur** secteur::secteurNonCouvert = NULL;
secteur* secteur::secteurNonCouvert = new secteur(-1,-1, NULL, -1, -1, -1, -1, -1, -1, -1, -1);

// Varibles statiques des pointTest
pointTest*** pointTest::lesPT = NULL;
pointTest** pointTest::lesPTPlats = NULL;
int pointTest::nb_tp = 0;
int pointTest::pdm = 0;
int pointTest::size_x = 0;
int pointTest::size_y = 0;
int pointTest::xMin = 0;
int pointTest::yMin = 0;
int pointTest::nb_scenarii = 0;
int secteur::nb_sectarii = 0;

// Varibles statiques des sites
site*** site::lesSitesM = NULL;
site** site::lesSites = NULL;
int site::size_x = 0;
int site::size_y = 0;
int site::size = 0;
//GOutputFile outfile("sortie2.txt");

// main
int main(int argc, char* argv[]) {
	srand ( time(NULL) );
	srand ( 0 );
    FILE* gplot; /* pour la manipulation de gnuplot */

    cout << "version " << AutoVersion::MAJOR << "." << AutoVersion::MINOR << endl;
    //int taille_x, taille_y, nb_clients;
    //double delta_tp, cote_hexag, haut_hexag;
    //int nb_tp, nb_tp_x, nb_tp_y;
    //int nb_sites, nb_sites_x, nb_sites_y;
    int k;
	Environement::set_nbPrteuse();
    // Création des points de test
    char * tp_file = (char*)"dataFT/DataXYZ.txt";
    pointTest::load_test_point_file(tp_file);
    char * site_file = (char*)"dataFT/ListeStations_V1_900Mhz.txt";
     site::load_sites_file(site_file);

    for (int i=0; i < site::size; i++)
        site::lesSites[i]->display_total();

    char * demand_file = (char*)"dataFT/lundi.distrib";
    ///Lecture des scenarios a optimiser apres l etape d'analyse
    int nb_sce = 1;
    //24=12h, 48=18h,64=22h,8=8h,36=15h, 10h = 16
     int no_col[] = {24};

    char nom[200];
    //Test_TMV_Frequences_Robustesse_Moyenne_SC-CritiquesALL/Test5_meanTMV
    sprintf(nom, "fichier_de_sortie.txt");//Optimglobale18_22_27
    GOutputFile file_sortie;
    file_sortie.setFileName(nom);
    pointTest::load_scenarii_file(demand_file, nb_sce, no_col, nom);
    //pointTest::load_scenarii_file(demand_file, nb_sce, no_col);
    //*******************
	secteur::secteurNonCouvert->initialize_scenarii(nb_sce);

    int nb_clients_scen1 = 0;
    int nb_tp_a = 0;
    for (int i=0; i<pointTest::size_x; i++) {
        for (int j=0; j<pointTest::size_y; j++) {
            //if ((pointTest::lesPT[i][j]->get_have_client()) && (pointTest::lesPT[i][j]->get_scenario(0)->nb_clients != 0)) {
            if (pointTest::lesPT[i][j]->get_have_client()) {
                //cout << "aaaaaaaa" <<endl;
                //if ((pointTest::lesPT[i][j]->get_x() == 944775) && (pointTest::lesPT[i][j]->get_y() == 2290500) )
                //pointTest::lesPT[i][j]->display_detail();
                nb_clients_scen1 = nb_clients_scen1 + pointTest::lesPT[i][j]->get_scenario(0)->nb_clients;
                nb_tp_a++;
            }
        }
    }
    cout << "le nombre de clients dans le scenario 1 =" << nb_clients_scen1 << endl;
    cout << "nombre des tp actifs =" << nb_tp_a << endl;
    // Comptage des points de test actifs

    for (int i=0; i < site::size; i++){
        for (int j=0; j < site::lesSites[i]->get_nb_secteur(); j++){
            site::lesSites[i]->get_secteur(j)->initialize_scenarii(nb_sce);
        }
    }

 cout << "grrrrrrrrrrrrrrrr" << endl;
  // Indexation des points de test actifs (i.e. ayant au moins 1 clients)
    k = 0;
    pointTest** lesPTA = new pointTest*[nb_tp_a];
    for (int i=0; i<pointTest::size_x; i++) {
        for (int j=0; j<pointTest::size_y; j++) {
            if (pointTest::lesPT[i][j]->get_have_client()){
                lesPTA[k] = pointTest::lesPT[i][j];
                k++;
                // Identifie le site le plus proche géographiquement du point de test
                pointTest::lesPT[i][j]->find_nearest_sites(Environement::nb_neighbor_sites);
                // Calcul les puissances des signaux reçus par le point de test provenant des secteurs voisins
                pointTest::lesPT[i][j]->set_signaux_nearest_sites();
                pointTest::lesPT[i][j]->set_circle_of_influence(Environement::radius_of_influence_m);
			}
		}
    }

//lesPTA[300]->display_detail2();

	// Association des points test avec les secteurs géographiquement au secteur le plus proche
	for (int i=0; i<nb_tp_a; i++) {
		// Association des points test au secteur géographiquement le plus proche
		//lesPTA[i]->association_geographique(cote_hexag, site::lesSites);
		// Association des points test au secteur offrant le plus fort signal
		lesPTA[i]->association_best_power();
		// Association des points test au secteur offrant le meilleur (+ fort) SINR // impossible à l'initialisation
		lesPTA[i]->association_best_sinr();
		// Calcul des SINR des différents secteurs au niveau du point de test
		lesPTA[i]->calculTousSinr();
	}


	bool gplot_output = false;
	if (gplot_output) {
		if ((gplot=popen("gnuplot","w"))==NULL) {
			printf("gnuplot n est pas installe sur votre systeme\n");
			exit(-1);
		}
	}

	// On effectue des sorties graphiques, le choix de la sortie est
	char file_name_puissances[] = "map_puissances.txt";
	char file_name_sinr[] = "map_sinr.txt";
	char file_name_secteur[] = "map_secteurs.txt";
	char file_name_site[] = "map_sites.txt";
	char file_name_couvert[] = "map_couvert01.txt";

	secteur** lesSecteurA;
	int nb_secteur_a = 0;
	for (int i=0; i<site::size; i++)
		for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++){//cout << "nb secteur =" << site::lesSites[i]->get_nb_secteur() << endl;
			if (site::lesSites[i]->get_secteur(j)->have_client())
				nb_secteur_a++;
	}
	cout << "nb secteurs actifs =" << nb_secteur_a << endl;
	lesSecteurA = new secteur*[nb_secteur_a];
	k = 0;
	for (int i=0; i<site::size; i++) {
		for (int j=0; j<site::lesSites[i]->get_nb_secteur(); j++) {
			if (site::lesSites[i]->get_secteur(j)->have_client()) {
				lesSecteurA[k] = site::lesSites[i]->get_secteur(j);
				k++;
			}
		}
	}
	secteur::initialisation(lesSecteurA, nb_secteur_a);

int stable = 10;
int no_scenar = 1;
//char* logFichier("");
Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a);

srand (time (NULL));
clock_t startTime=clock();
//double elapsedTime=0;
int nb = 0;
//cout << "resultats optimisation du scenario 1" << endl;
//string public const monFile ("Sortie");
    file_sortie.open();
   double elapsedTime=0;

	 optimisation::descente_simple_frequence(nom, stable,lesPTA, nb_tp_a,lesSecteurA, nb_secteur_a, -1);

     //optimisation::optimisation_FrequencyTabuSearch(nom,stable, lesPTA, nb_tp_a,lesSecteurA, nb_secteur_a,tilt_delta,delta_power, indiceMin);


	//elapsedTime=(clock()-startTime) / (double)CLOCKS_PER_SEC;
   // }

//fin optimisation
file_sortie << "<<<<<<<<<<<<<<" << "\n";
file_sortie << "resultats de sortie" << "\n";
//file_sortie << "le temps d execution: "<< timeOfRun_sec << "\n";
    //file_sortie << "non couverts 8 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 7) << "\n";
    //file_sortie << "non couverts 9 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 8) << "\n";
    //file_sortie << "non couverts 10 = " << (float)Fitness::eval(stable,lesPTA, nb_tp_a, lesSecteurA, nb_secteur_a, 9) << "\n";*/

     file_sortie << "------------------------------------------------------0-----------------------------------------------"<< "\n";

file_sortie.close();

	// afficher des resultats
	int no_scen_aff = 0;
	pointTest::save_map_TM_as(file_name_puissances, 0, no_scen_aff, nb_tp_a);
	pointTest::save_map_TM_as(file_name_sinr, 1, no_scen_aff, nb_tp_a);
	pointTest::save_map_TM_as(file_name_secteur, 2, no_scen_aff);
	pointTest::save_map_TM_as(file_name_site, 3, no_scen_aff);
	pointTest::save_map_TM_as(file_name_couvert, 4, no_scen_aff);
	if (gplot_output) gplot_diplay(gplot,file_name_sinr);
	//if (gplot_output) gplot_diplay(gplot, file_mane_puissances);

return 0;

}

