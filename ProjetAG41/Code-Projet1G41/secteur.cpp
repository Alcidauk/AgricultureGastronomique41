#include "secteur.h"
#include "site.h"
#include "pointTest.h"

using namespace std;

/// Constructeur vide
secteur::secteur() {}
/// Destructor
secteur::~secteur() {}

/// Constructeur
/** Constructeur de l'objet secteur \n
	puissanceEmision=46dBm
	@param _no numéro du secteur dans le site
	@param _s adresse du site sur lequel est installé le sercteur
	@param type_antenne =0 pour une antenne omnidirectionelle gain=2dBi, theta3dB=10° \n
						=1 pour une antenne directionelle gain=17dBi, theta3dB=10°, phi3dB=70°
	@param a_radian azimut en radian
	@param p numéro de la porteuse (ou du canal)
*/
secteur::secteur(int _no, site *_s, int type_antenne, double a_radian, int p) {
    no = _no;
    s = _s;
//    nb_clients = 0;
    azimut_radian = a_radian;
    porteuse = p;
    largeurDisponible_kHz = Environement::get_largeurBande_kHz()/Environement::nbProteuses;
    puissance_dBm = 46.;
    //puissance_dBm = Unite::watt2dBm(16.); // ~42 dBm
    switch (type_antenne) {
		case 0 : // antenne omnidirectionnelle
			ant = new antenne(type_antenne, 2, 10);
			break;
		case 1 : // antenne directive
			ant = new antenne(type_antenne, 17, 10, 70);
			break;
    }
    tiltE_radian = Unite::degre2radian(10);
    tiltM_radian = Unite::degre2radian(20);
//    largeurProteuseClient_kHz = Environement::get_largeurBandeClient_kHz();
//	largeurNecessaire_kHz = 0;
//	largeurUtilisee_kHz = 0;
}

/// Constructeur
/** Constructeur de l'objet secteur \n
	puissanceEmision=46dBm
	@param _no numéro du secteur dans le site
	@param _s adresse du site sur lequel est installé le sercteur
	@param type_antenne =0 pour une antenne omnidirectionelle gain=2dBi, theta3dB=10° \n
						=1 pour une antenne directionelle gain=17dBi, theta3dB=10°, phi3dB=70°
	@param a_radian azimut en radian
	@param p numéro de la porteuse (ou du canal)
*/
secteur::secteur(int _no_abs, int _no, site *_s, int type_antenne, double _perte_cable, double _gain_dBi, double _power_dBm, double _azimut_radian,
                int _tilt_m, int _tilt_e, int _p) {
    no = _no;
    no_absolu = _no_abs;
    s = _s;
//    nb_clients = 0;
    azimut_radian = _azimut_radian;
    porteuse = _p;
    largeurDisponible_kHz = Environement::get_largeurBande_kHz()/Environement::nbProteuses;
    puissance_dBm = _power_dBm;
    //puissance_dBm = Unite::watt2dBm(16.); // ~42 dBm
    perte_cable_dBm = _perte_cable;
    switch (type_antenne) {
		case 0 : // antenne omnidirectionnelle
			ant = new antenne(type_antenne, 2, 10);
            ant = new antenne(type_antenne, _gain_dBi, 10.);
			break;
		case 1 : // antenne directive
            ant = new antenne(type_antenne, _gain_dBi, 10., 70.);
			break;
    }
    tiltE_radian = Unite::degre2radian(_tilt_e);
    tiltM_radian = Unite::degre2radian(_tilt_m);
//    largeurProteuseClient_kHz = Environement::get_largeurBandeClient_kHz();
//	largeurNecessaire_kHz = 0;
//	largeurUtilisee_kHz = 0;
}

void secteur::initialize_scenarii(int nb_scen) {
    nb_sectarii = nb_scen;
    sectarii = new sectario*[nb_sectarii];
    for (int i=0; i<nb_sectarii; i++) {
        sectarii[i] = new sectario();
    }
}

/*
void secteur::initialize_scenarii_non_couvert(int nb_scen) {
    secteur::secteurNonCouvert->sectarii = new sectario*[nb_scen];
    for (int i=0; i<nb_scen; i++) {
        secteur::secteurNonCouvert->sectarii[i] = new sectario();
    }
}
*/
/** Libère le secteur des ses points de test et de la largueur de bande nécessaire
*/
/*
void secteur::free() {
    nb_clients = 0;
    largeurDisponible_kHz = Environement::get_largeurBande_kHz()/Environement::nbProteuses;
    puissance_dBm = 46.;
    tiltE_radian = Unite::degre2radian(10);
    tiltM_radian = Unite::degre2radian(20);
    largeurProteuseClient_kHz = Environement::get_largeurBandeClient_kHz();
	largeurNecessaire_kHz = 0;
	largeurUtilisee_kHz = 0;
	cellule.clear();
}
*/

/** retourne le premier indice de la grille des site dans un pavage hexagonale
*/
int secteur::get_si() {
    return s->get_si();
}

/** retourne le second indice de la grille des site dans un pavage hexagonale
*/
int secteur::get_sj() {
    return s->get_sj();
}

/** retourne le coordonnée x du secteur (=celui du site)
*/
double secteur::get_x() {
    return s->get_x();
}

/** retourne le coordonnée y du secteur (=celui du site)
*/
double secteur::get_y() {
    return s->get_y();
}

/** retourne le coordonnée z du secteur (=celui du site)
*/
double secteur::get_z() {
    return s->get_z();
}

/*/** retourne le numéro absolu du secteur (=numéro du site * nombre de secteurs + numéro du secteur)
int secteur::get_no_absolue() {
	if (this==secteurNonCouvert) return -1;
    return s->get_no()*s->get_nb_secteur()+no;
}
*/

void secteur::calcul_sinr() {
	pointTest::calcul_sinr(get_site()->circle_of_influence);
}

void secteur::update_signal() {
    vector<pointTest*>::iterator it;
    for ( it=get_site()->circle_of_influence.begin() ; it < get_site()->circle_of_influence.end(); it++ )
		(*it)->update_signal(this);
}

/** indique si le secteur a au moins un client dans l'un des scénarii
*/
bool secteur::have_client() {
	for (int k=0; k<nb_sectarii; k++)
		if (get_clients(k) != 0)
			return true;
	return false;
}

/** Calcul et fixe la largeur de bande passante nécessaire pour écouler le débit des clients du secteur \n
	largeurNecessaire_kHz = somme (largeurNecessaire_kHz des clients)
*/
void secteur::set_largeurNecessaire_kHz(int i) {
    sectarii[i]->largeurNecessaire_kHz = 0;
    vector<pointTest*>::iterator it;
    for ( it=sectarii[i]->cellule.begin() ; it < sectarii[i]->cellule.end(); it++ )
        sectarii[i]->largeurNecessaire_kHz += (*it)->get_largeurNecessaire_kHz(i);
}

/** Recalcule et fixe le nombre des clients du secteur
*/
void secteur::set_nb_clients(int i) {
    sectarii[i]->nb_clients = 0;
    vector<pointTest*>::iterator it;
    for ( it=sectarii[i]->cellule.begin() ; it < sectarii[i]->cellule.end(); it++ )
        sectarii[i]->nb_clients += (*it)->get_nb_clients(i);
}

/** Ajoute un point de test dans la 'cellule' du secteur pour le scenario 'i'
	@param i numéro du sectario
	@param t adresse du point de test
*/
void secteur::add_cellule(int i, pointTest *t) {
    sectarii[i]->cellule.push_back(t);
    sectarii[i]->nb_clients += t->get_nb_clients(i);
	t->new_serveur(this, i);
}

/** Supprime un point de test de la 'cellule' du secteur pour le scenario 'i'
	@param i numéro du sectario
	@param t adresse du point de test
*/
void secteur::delete_cellule(int i, pointTest *t) {
    vector<pointTest*>::iterator it;
    for (it=sectarii[i]->cellule.begin() ; it < sectarii[i]->cellule.end(); it++ )
        if ((*it) == t) sectarii[i]->cellule.erase(it);
	sectarii[i]->nb_clients -= t->get_nb_clients(i);
}

/** Affiche les paramètres du secteur
*/
void secteur::display() {
	display_simple();
	for (int k=0; k<nb_sectarii; k++)
		cout << "scenario " << k << " -> nb clients=" << get_clients(k) << " tx_occup=" << get_txOccP(k) << " largeurN(kHz)=" << get_largeurNecessaire_kHz(k) << " / " << get_largeurDisponible_kHz() <<  endl;
//    cout << "secteur " << get_si() << " , " << get_sj() << " azimut=" << get_azimut_degre()
//    << " -> position=" << get_x() << " "  << get_y() << " "  << get_z()
    //<< " : nb clients=" << nb_clients << " tx_occup=" << get_txOccP()
//    << endl;
}

void secteur::display_simple() {
    cout << "secteur " << get_no_absolue() << " " << get_no()
    << " (" << get_x() << ";"  << get_y() << ";"  << get_z()
    << ") -> az=" << get_azimut_degre()<< " t_m=" << get_tiltM_degre() << " t_e=" << get_tiltE_degre()
    << " p(dBm)=" << get_puissance_dBm() << " g(dBi)=" << get_gain_dBi()
    << endl;
}

/** Initialise les secteurs avec comme largueur de bande utilisée
	@param lesS liste des secteurs
	@param taille de la liste
*/
void secteur::initialisation(secteur** lesS, int taille) {
	for (int i=0; i<taille; i++) {
		for (int k=0; k<nb_sectarii; k++) {
			lesS[i]->set_nb_clients(k);
			lesS[i]->set_largeurUtiliseeInitiale_kHz(k);
			//cout << lesSecteurA[i]->get_largeurUtilisee_kHz()  << endl;
		}
    }
}

/** fixe arbitrairement le taux d'occupation du canal utilisés par tous les secteurs à 'tx'
	@param lesS liste des secteurs
	@param taille de la liste
	@param tx valeur du taux d'occupation
*/
void secteur::fixe_txOccP(secteur** lesS, int taille, double tx) {
	for (int i=0; i<taille; i++)
		for (int k=0; k<nb_sectarii; k++)
			lesS[i]->set_txOccP(k, tx);
}

/** Mets à jour (calcul et fixe) les largueurs de bande nécessaire et utilisé par le secteur (-> taux d'occupation) \n
	ainsi que son nombre de clients
*/
void secteur::miseAjour(secteur** lesS, int taille) {
    for (int i=0; i<taille; i++) {
		for (int k=0; k<nb_sectarii; k++) {
			lesS[i]->set_nb_clients(k);
			lesS[i]->set_largeurNecessaire_kHz(k);
			//cout << lesSecteurA[i]->get_largeurUtilisee_kHz()  << endl;
			lesS[i]->set_largeurUtilisee_kHz(k);
			//cout << lesSecteurA[i]->get_largeurUtilisee_kHz()  << endl;


/*
if(i==taille/2+1) {
        cout << "secteur " << i << " : manque=" << lesS[i]->get_manqueBande_kHz()
        	 << "; nessecaire=" << lesS[i]->get_largeurNecessaire_kHz()
        	 << "/" << lesS[i]->get_largeurDisponible_kHz() << " -> " << lesS[i]->get_txOccP()
        	 << "; nbC = " << lesS[i]->get_clients() << endl;

    }
*/
		}
	}
}

/** Supprime aléatoirement de la cellule du secteur des points de test afin \n
	que la capacité en terme de bande passante soit respectée.
*/
void secteur::sur_capacite() {
	for (int k=0; k<nb_sectarii; k++) {
//		int gain = 0;
		//cout << "get_manqueBande_kHz(k)=" << get_manqueBande_kHz(k) << endl;
//		while (get_manqueBande_kHz(k) > gain)
		while ( sectarii[k]->largeurNecessaire_kHz > this->get_largeurDisponible_kHz() )
		{
			sectarii[k]->largeurNecessaire_kHz -= get_cellule(k).at(0)->get_largeurNecessaire_kHz(k);
//			gain += get_cellule(k).at(0)->get_largeurNecessaire_kHz(k);
			get_cellule(k).at(0)->change_cellule(k,secteur::secteurNonCouvert);
			// supprime le point de test à la cellule du nouveau serveur
			//delete_cellule(k, get_cellule(k).at(0));
			// ajouter le point de test à la cellule des non couverts
			//secteur::secteurNonCouvert->add_cellule(k,get_cellule(k).at(0));
		}
//		set_largeurNecessaire_kHz(k);
		//cout << "gain=" << gain << endl;
		//cout << "get_manqueBande_kHz(k)=" << get_manqueBande_kHz(k) << endl;
	}
}

int secteur::get_nb_clients_total() {
	int nb_clients_t = 0;
	for (int k=0; k<nb_sectarii; k++)
		nb_clients_t += get_clients(k);
	return nb_clients_t;
}
