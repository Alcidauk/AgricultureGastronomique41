#ifndef POINTTEST_H
#define POINTTEST_H

#include <iostream>
#include <fstream>
#include <vector>
#include <string.h>
//#include "secteur.h"
#include "site.h"
#include "environement.h"
#include "random.h"
#include "gfile.h"

class Propagation;
class secteur;
class site;

using namespace std;

typedef struct _tab_sites {
	_tab_sites():no_site(-1),dist_site_m(INFINITY){}
	int no_site;
	double dist_site_m;
} tab_sites;

typedef struct _tabSinr {
	_tabSinr():sinr_dB(-INFINITY),sinr_su(0){}
    double sinr_dB;
    double sinr_su;
} tabSinr;

typedef struct _signal {
	double  puissRecue_dBm;
    tabSinr* sinr;
    //vector<tabSinr> sinr;
	//double  sinr_dB;
    //double  sinr_su;
	secteur* sect;
} *signal;

typedef struct _scenario {
    _scenario():serveur(NULL),bestServer(NULL),sinr_dB(-INFINITY),sinr_su(-1),nb_clients(0),ds_client_kbps(NULL),
                puissanceRecue_dBm(Environement::get_puissanceNulle_dBm()),largeurNecessaire_kHz(0){}
    secteur* serveur;
    secteur* bestServer;
	double sinr_dB;
    double sinr_su;
    int nb_clients;
    int *ds_client_kbps;
    double puissanceRecue_dBm;
    double largeurNecessaire_kHz;
} scenario;

signal new_signal(double pR, secteur* _s);
signal new_signal(double pR, secteur* _s, int nb_scen);
//scenario new_scenario(secteur* _serveur, int _nb_clients, double * _ds_client, tabSinr* _sinr, double _largeurNecessaire_kHz);

class pointTest
{
    private:
		int no;
        int pi;
        int pj;
        double x;
        double y;
        double z;
        bool have_client;
        //secteur* secteurBestSinr;

        //secteur* plusProcheSecteur;
//        double puissanceRecue_dBm;
//        int nb_clients;
//        double sinr_dB;
//        double sinr_su;
        vector<signal> signaux;
        scenario** scenarii;
        //double sommeInterferences;
//        double largeurNecessaire_kHz;
        int i_plusProcheSite;
        int j_plusProcheSite;
        int plusProcheSite;
        tab_sites* nearest_sites;
        int nb_nearest_sites;

    public:
        pointTest();
        ~pointTest();
        pointTest(int _no, int _i, int _j, double _x, double _y, double _z);

        /// tableau des points de test
        static pointTest** lesPTPlats;
        /// matrice des points de test
        static pointTest*** lesPT;
		/// taille horizontale de la matrice des points de test
        static int size_x;
		/// taille verticale de la matrice des points de test
        static int size_y;
        static int nb_tp;
        static int pdm;
        static int nb_scenarii;
        static int xMin;
        static int yMin;

        static void load_test_point_simple(int nb_tp_x, int nb_tp_y, int delta_tp, int z_tp);
        static void load_test_point_file(char * tp_file_name);
        static void load_scenarii_file(char * scenarii_file_name, int nb_scen, int* no_col, char* nom);

        int get_no(){return no;};
        int get_pi(){return pi;};
        int get_pj(){return pj;};
        double get_x(){return x;};
        double get_y(){return y;};
        double get_z(){return z;};


        int get_nb_clients(int no_scenario){return scenarii[no_scenario]->nb_clients;};
        secteur* get_serveur(int no_scenario){return scenarii[no_scenario]->serveur;};
        secteur* get_bestServer(int no_scenario){return scenarii[no_scenario]->bestServer;};
        double get_puissanceRecue_dBm(int no_scenario){return scenarii[no_scenario]->puissanceRecue_dBm;};
        //int get_nb_scenarii(){return nb_scenarii;};
        scenario* get_scenario(int i){return scenarii[i];};
		int get_ds_client(int no_scen, int no_client){return scenarii[no_scen]->ds_client_kbps[no_client];};

        secteur* get_secteur(int i){return signaux.at(i)->sect;};
        int get_nbSignaux(){return signaux.size();};
        double get_puissanceSignal_dBm(int signali){return signaux.at(signali)->puissRecue_dBm;};
        bool get_have_client(){return have_client;};
        void add_client(int no_scen, int debit);

        double get_sinr_dB(int no_scenario){return scenarii[no_scenario]->sinr_dB;};
        //double get_sinr_dB(int no_scenario, int i){return signaux.at(i)->sinr[no_scenario].sinr_dB;};

        double get_largeurNecessaire_kHz(int no_scenario){return scenarii[no_scenario]->largeurNecessaire_kHz;};
        void set_largeurNecessaire_kHz();

        void set_scenario(int no_scen, int _nb_clients, int debit_client);

        void set_signaux_voisins(site*** lesS);
        void set_signaux_voisins_2(site*** lesS);
        void set_signaux_total(site*** lesS);
        void set_signaux_nearest_sites();
        void add_signal(int i, int j, site*** lesS);
        void add_signal(tab_sites s);
/*        void set_serveur(secteur* _s){serveur = _s;};

        //void set_signal(int k, secteur* anEnb, double anPuissanceRecue_dB);
        void set_nb_clients(int nbC){nb_clients = nbC;};

        //void changement_secteur(secteur* nvEnb, int k);
*/
        double calculPuissanceRecue_dB(secteur* theEnb);
        double calculAffaiblissementV_dB(secteur* theEnb);
        double calculAffaiblissementH_dB(secteur* theEnb);
        void calculSinr(secteur *, int);
        void calculSinr(secteur *);
		void calculTousSinr();

        void association_geographique(int coteHexag, site*** lesS);
        void association_best_power();
        void association_best_sinr();
		void new_serveur(secteur* new_secteur, int no_scenario){scenarii[no_scenario]->serveur = new_secteur;};
		void change_cellule(int no_scenario, secteur* new_secteur);

        void find_nearest_site();
        void find_nearest_sites(int nb);
        void find_nearest_sites2(int nb, tab_sites* &nearest_sites);
        //void find_site_ij_hexagonal(int a, site*** s, int &i_best, int &j_best);
        void find_site_ij_hexagonal(int coteHexagonal, site*** s);
		void set_circle_of_influence(double radius_of_influence);
		void update_signal(secteur* s);
        secteur* get_secteurBestSinr();
        //void add_client(int i){nb_clients += i;};

        static void save_map_as(char* filename, int thetype, int no_scen);
        static void save_map_TM_as(char* filename, int thetype, int no_scen, int nb_tp_a=0);
        static void calcul_sinr(pointTest ** pt, int taille);
		static void calcul_sinr(vector<pointTest*> pt);

        void free();


        void display();
        void display_simple();
        void display_detail();
        void display_detail2();




};

#endif
