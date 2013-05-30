#ifndef SECTEUR_H
#define SECTEUR_H

#include <math.h>
#include <iostream>
#include <vector>

#include "antenne.h"
#include "environement.h"
//#include "pointTest.h"

using namespace std;

class pointTest;
class site;


typedef struct _sectario {
    _sectario():nb_clients(0),cellule(),
                largeurNecessaire_kHz(0),
                largeurUtilisee_kHz(0),
                largeurProteuseClient_kHz(Environement::get_largeurBandeClient_kHz()){}
    /// nombre de client associé au secteur (de la cellule)
    int nb_clients;
    /// ensemble des points de test du secteur c-à-d cellule du secteur
    vector< pointTest* > cellule;
    /// largeur de la porteuse en MHz nécessaire au secteur pour écouler le débit demandé par ces clients (modèle : w^_t)
    double largeurNecessaire_kHz;
    /// largeur de la porteuse en MHz utilisée par le secteur = min(porteuse nécessaire, porteuse diponible)
    double largeurUtilisee_kHz;
    /// largeur de la porteuse en MHz utilisée en moyenne par un client
    double largeurProteuseClient_kHz;
} sectario;

class secteur
{
    private:
		/// numéro du secteur du site
        int no;
		/// numéro du secteur dans l'absolu
        int no_absolu;
        /// adresse du site sur lequel est installé le secteur
        site *s;
        /// puisance d'émission du secteur en dBm
        double puissance_dBm;
        /// azimut de l'antenne en radian
        double azimut_radian;
        /// indice de la porteuse
        int porteuse;
        /// perte des cables
        double perte_cable_dBm;
        /// tilt éléctrique en radian
        double tiltE_radian;
        /// tilt mécanique en radian
        double tiltM_radian;
        /// adresse de l'antenne installé sur ce secteur
        antenne* ant;
        /// largeur de la porteuse disponible en MHz (modèle : w)
        double largeurDisponible_kHz;
        sectario** sectarii;

    public:
        secteur();
        ~secteur();
        secteur(int _no, site *_s, int type_antenne, double a_radian, int p);
        secteur(int _no_abs, int _no, site *_s, int type_antenne, double _perte_cable, double _gain_dBi, double _power_dBm, double _azimut_radian,
                int _tilt_m, int _tilt_e, int _p);
		void free();

		/// Secteur des points de test non couverts
        static secteur* secteurNonCouvert;
        static int nb_sectarii;

        int get_no(){return no;};
        int get_no_absolue(){return no_absolu;};
        int get_si();
        int get_sj();
        double get_x();
        double get_y();
        double get_z();
        site* get_site(){return s;};

        double get_puissance_dBm(){return puissance_dBm;};
        double get_azimut_radiant(){return azimut_radian;};
        double get_azimut_degre(){return Unite::radian2degre(azimut_radian);};
        double get_perte_cable_dBm(){return perte_cable_dBm;};
        double get_tiltE_radian(){return tiltE_radian;};
        double get_tiltM_radian(){return tiltM_radian;};
        double get_tiltE_degre(){return Unite::radian2degre(tiltE_radian);};
        double get_tiltM_degre(){return Unite::radian2degre(tiltM_radian);};
        void set_tiltM_degre(double t_degre){tiltM_radian = Unite::degre2radian(t_degre);};
        void set_tiltE_degre(double t_degre){tiltE_radian = Unite::degre2radian(t_degre);};
        void set_puissance(double p_puissance){puissance_dBm = p_puissance;};
        double get_tiltT_degre(){return Unite::radian2degre(tiltM_radian)+Unite::radian2degre(tiltE_radian);};
        double get_gain_dBi(){return ant->get_gain_dBi();};
        double get_diagrammeV_radian2dB(double angle_radian){return ant->get_diagV_radian2dB(angle_radian);};
        double get_diagrammeH_radian2dB(double angle_radian){return ant->get_diagH_radian2dB(angle_radian);};
        int get_porteuse(){return porteuse;};
        void set_porteuse(int p){porteuse=p;};
        bool have_client();

        double get_largeurDisponible_kHz(){return largeurDisponible_kHz;};
        double get_largeurNecessaire_kHz(int i){return sectarii[i]->largeurNecessaire_kHz;};
        double get_manqueBande_kHz(int i){return max(0.,sectarii[i]->largeurNecessaire_kHz-largeurDisponible_kHz);};
        double get_bruitBlanc_mW(){return Unite::kilo(Environement::get_densiteBruitBlanc()*Unite::kilo(largeurDisponible_kHz));};
        double get_bruitBlanc_dBm(){return Unite::mWatt2dBm(get_bruitBlanc_mW());};
        int get_clients(int i){return sectarii[i]->nb_clients;};
        int get_nb_clients_total();

        //double get_txOccP(){return nb_clients*1./nbClientsMaxPorteuse;};
        double get_txOccP(int i){return sectarii[i]->largeurUtilisee_kHz/largeurDisponible_kHz;};
        //double get_txOccP(){return largeurNecessaire_kHz/largeurDisponible_kHz;};
        double get_largeurUtilisee_kHz(int i){return sectarii[i]->largeurUtilisee_kHz;};
        vector<pointTest*> get_cellule(int i){return sectarii[i]->cellule;};

        void initialize_scenarii(int nb_scen);
        void set_nb_clients(int nb_scen);
        void set_nb_clients(int nb_scen, int nbC){sectarii[nb_scen]->nb_clients = nbC;};
        void set_largeurUtiliseeInitiale_kHz(int nb_scen){sectarii[nb_scen]->largeurUtilisee_kHz = sectarii[nb_scen]->nb_clients*sectarii[nb_scen]->largeurProteuseClient_kHz;};
        void set_largeurUtilisee_kHz(int i){sectarii[i]->largeurUtilisee_kHz = min(sectarii[i]->largeurNecessaire_kHz, largeurDisponible_kHz);};
        void set_largeurNecessaire_kHz(int i);
        void set_largeurDisponible_kHz(double l){largeurDisponible_kHz = l;};
        // pour fixer les taux d'occupation
		void set_txOccP(int i, double tx){sectarii[i]->largeurUtilisee_kHz = tx*largeurDisponible_kHz;};

        void add_cellule(int i, pointTest *t);
        void delete_cellule(int i, pointTest *t);
		void sur_capacite();
		void update_signal();
		void calcul_sinr();

//        void add_client(int i){
//            nb_clients += i;
//            set_largeurUtiliseeInitiale_kHz();
//        };
        void display();
        void display_simple();
        static void miseAjour(secteur** lesS, int taille);
		static void initialisation(secteur** lesS, int taille);
		static void fixe_txOccP(secteur** lesS, int taille, double tx);

};

#endif

