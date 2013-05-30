#ifndef SITE_H
#define SITE_H

#include <iostream>
#include <vector>
#include <string.h>
#include "gfile.h"
#include "pointTest.h"

using namespace std;

class secteur;
class pointTest;
/*
typedef struct _table_sites {
	_table_sites():no_site(-1),dist_site_m(INFINITY){}
	int no_site;
	double dist_site_m;
} table_sites;

struct TabuItem{
    TabuItem():no_site(-1),dureeTabu(0){}
    int no_site;
    int dureeTabu;
};*/
typedef struct Table_TP {
	Table_TP():no_TP(-1),dist_site_m(INFINITY){}
	int no_TP;
	double dist_site_m;
    }Tab_TP;

class site
{
    private:
        int no;
        int nb_secteur;
        int si;
        int sj;
        int si_max;
        int sj_max;
        double x;
        double y;
        double z;
        int patern;
        char name[80];
        //secteur** sect;
        vector <secteur*> sect;
       // table_sites* closest_sites;
        int nb_closest_sites;
        int nb_closest_TP;
        Tab_TP* nearest_TP;

    public:
        site();
        ~site();
        site(double _x, double _y, double _z, int _si, int _sj, int t);
        site(int _no, char* _name, double _x, double _y, double _z);

        vector <pointTest*> circle_of_influence;
        //vector <TabuItem*> tabuList;
   		/// matrice des sites
        static site*** lesSitesM;
   		/// tableau des sites
        static site** lesSites;
   		/// nombre de sites
        static int size;
   		/// taille horizontale de la matrice des sites
        static int size_x;
   		/// taille verticale de la matrice de sites
        static int size_y;

        static void load_sites_simple(double cote_hexag, double haut_hexag, int z_site, int pattern_reutilisation);
        static void load_sites_file(char* file_name);
        static void load_sites_file_freqaleatoire(char* file_name);
        static void load_sites_file_freqmanuelle(char* file_name);
        static void load_sites_file_freqmanuelle2(char* file_name);

        //static void find_ij_hexagonal(double x, double y, int a, int i_max, int j_max, int &i, int &j);
        //static void find_site_hexagonal(double x, double y, int a, site*** s, int &i_best, int &j_best);
        int find_secteur(double lex, double ley);

        int get_si(){return si;};
        int get_sj(){return sj;};
        int get_no(){return no;};
        //int get_si_max(){return si_max;};
        //int get_sj_max(){return sj_max;};
        double get_x(){return x;};
        double get_y(){return y;};
        double get_z(){return z;};
        int get_nb_secteur(){return nb_secteur;};
        secteur* get_secteur(int i){return sect.at(i);};
        void find_closest_sites(int nb);
        void find_closest_TP(int num_site, int nb, Table_TP* &closest_TP);
        void set_closest_sites();
        void display();
        void display_total();
};

#endif
