#include "pointTest.h"
#include "propagation.h"
#include "secteur.h"


using namespace std;

void pointTest::load_test_point_simple(int nb_tp_x, int nb_tp_y, int delta_tp, int z_tp) {
    // Création des points de test
    pointTest::lesPT = new pointTest**[nb_tp_x];
    for (int i=0; i<nb_tp_x; i++) {
        pointTest::lesPT[i] = new pointTest*[nb_tp_y];
        for (int j=0; j<nb_tp_y; j++) {
            pointTest::lesPT[i][j] = new pointTest(i*nb_tp_y+j, i, j, i*delta_tp, j*delta_tp, z_tp);
            //cout << i << " " << j << " " << i*delta_tp << " " << j*delta_tp << " " << endl;
        }
    }
}

void pointTest::load_test_point_file(char* fname)
{
    GInputFile infile(fname) ;
    char* buf ;
    char *tok ;
    int xMax, yMax;

    infile.open() ;
    cout<<"\n>> Chargement des informations de cellules\n";
    // Lecture de l'en-tete
    for( int i=0; i<7; i++ ){
         buf=infile.readLine();
         tok=infile.getNextToken();
         if( tok && strcmp(tok, "ndMailleX")==0 ){
             size_x=infile.getNextIntToken();
         }
         else if( tok && strcmp(tok, "ndMailleY")==0 ){
              size_y=infile.getNextIntToken();
              }
         else if( tok && strcmp(tok, "Xmin")==0 ){
              xMin=infile.getNextIntToken();
         }
         else if( tok && strcmp(tok, "Ymin")==0 ){
              yMin=infile.getNextIntToken();
         }
         else if( tok && strcmp(tok, "Xmax")==0 ){
              xMax=infile.getNextIntToken();
         }
         else if( tok && strcmp(tok, "Ymax")==0 ){
              yMax=infile.getNextIntToken();
         }
         else if( tok && strcmp(tok, "Pdm")==0 ){
              pdm=infile.getNextIntToken();
         }
    }
    nb_tp = size_x*size_y;
    //int theno = 0;
    int thex,they,thez;
    lesPT = new pointTest**[size_x];
    for (int i = 0; i < size_x; i++) {
        lesPT[i] = new pointTest*[size_y];
        for (int j = 0; j < size_y; j++) {
            buf=infile.readUncommentedLine();
        //while ((buf=infile.readUncommentedLine())!=NULL)
            thex = atoi(infile.getNextToken('\t'));
            they = atoi(infile.getNextToken('\t'));
            thez = atoi(infile.getNextToken('\t'));
            lesPT[i][j] = new pointTest(i*size_y+j, i, j, thex, they, thez);
            //lesPTPlats[theno] = new pointTest(theno, (thex-xMin)/pdm, (they-yMin)/pdm, thex, they, thez);
            //cout << i*size_y+j << " " << thez << endl;
            //char typT = (*infile.getNextToken('\t'));
            infile.getNextToken('\t');
        }
    }
    infile.close();
}

void pointTest::load_scenarii_file(char * scenarii_file_name, int nb_sce, int* no_col, char * nom) {
    GInputFile infile(scenarii_file_name);
    char* buf ;
    //char *tok ;
    infile.open() ;
    cout<<"\n>> Chargement des informations des scenarii\n";
    buf=infile.readLine();
    infile.getNextToken();
    int size_tp = infile.getNextIntToken();
    int **repartition_func;
    int *thex = new int[size_tp];
    int *they = new int[size_tp];

    int nb_scenarii_total = nb_sce;
	pointTest::nb_scenarii = nb_scenarii_total;
    repartition_func = new int*[nb_scenarii_total];
    for (int i=0; i<nb_scenarii_total; i++) {
        repartition_func[i] = new int[size_tp+1];
        for(int j=0; j<size_tp+1; j++)
            repartition_func[i][j] = 0;
    }

    int **nb_clients_scen = new int*[nb_scenarii_total];
    for (int i=0; i<nb_scenarii_total; i++) {
        nb_clients_scen[i] = (int*)malloc((size_tp)*sizeof(int));
        for (int j = 0; j<size_tp; j++)
            nb_clients_scen[i][j] = 0;
    }

    int add = 0;
    for (int j=1; j<size_tp+1; j++) {
        buf=infile.readUncommentedLine();
        thex[j-1] = infile.getNextIntToken();
        they[j-1] = 2000000 + infile.getNextIntToken();
        int k = 0;
        int nb_heure = 18;
        int no_col_max = 4*nb_heure;
        for(int i=0; i<no_col_max; i++) {
            add = infile.getNextIntToken();
            if (i==no_col[k]) {
                repartition_func[k][j] = repartition_func[k][j-1] + add;
                k++;
            }
        }
    }

    int val=-1, select_c=-1, i_c=-1, j_c=-1;
    for(int i=0; i<nb_scenarii_total; i++) {
        for (int j = 0; j < repartition_func[i][size_tp]/1000/5; j++) {
            val = Random::aleatoire(repartition_func[i][size_tp]);
            select_c = MathB::find_dichotomie(val, repartition_func[i], size_tp+1) -1;
            nb_clients_scen[i][select_c]++;
        }
    }
    GOutputFile file_sortie(nom);

     //fsortie.setFileName("sortie.txt");

     file_sortie.open();

     file_sortie << "le nombre de clients tires" << repartition_func[0][size_tp]/1000/5 << "\n";
     //file_sortie << "le nombre de clients tires" << repartition_func[1][size_tp]/1000/5 << "\n";
     //file_sortie << "le nombre de clients tires" << repartition_func[2][size_tp]/1000/5 << "\n";

     file_sortie.close();


    //cout << "le nombre de clients tires" << repartition_func[0][size_tp]/1000/5 << endl;
    //cout << "le nombre de clients tires" << repartition_func[1][size_tp]/1000/5 << endl;
    //cout << "le nombre de clients tires" << repartition_func[2][size_tp]/1000/5 << endl;


    int debit = 0;
    /* if (Random::random01() < Environement::get_proportionVoixData())
		debit = Environement::get_debitNecessaireVoix_kbps();
    else*/
		debit = Environement::get_debitNecessaireData_kbps();

    for (int j = 0; j < size_tp; j++) {
        i_c = (thex[j]-pointTest::lesPT[0][0]->get_x())/pointTest::pdm;
        j_c = (they[j]-pointTest::lesPT[0][0]->get_y())/pointTest::pdm;
        //pointTest::lesPT[i_c][j_c]->nb_scenarii = nb_scenarii_total;
        pointTest::lesPT[i_c][j_c]->scenarii = new scenario*[nb_scenarii_total];
        for(int i=0; i<nb_scenarii_total; i++) {
            pointTest::lesPT[i_c][j_c]->scenarii[i] = new scenario();
            //pointTest::lesPT[i_c][j_c]->exist = true;
                if (nb_clients_scen[i][j] != 0) {
                pointTest::lesPT[i_c][j_c]->set_scenario(i, nb_clients_scen[i][j], debit);
                pointTest::lesPT[i_c][j_c]->have_client = true;
            }
        }
    }
    infile.close();

    for (int i=0; i<nb_scenarii_total; i++){
        delete repartition_func[i];
        delete nb_clients_scen[i];
    }
    delete nb_clients_scen;
    delete repartition_func;
    delete thex;
    delete they;
}

/*
void pointTest::add_client(int no_scen, int debit) {
    scenarii[no_scen]->nb_clients++;
    scenarii[no_scen]->ds_client = (int*) realloc (scenarii[no_scen]->ds_client, scenarii[no_scen]->nb_clients * sizeof(int));
    if (scenarii[no_scen]->ds_client!=NULL)
        scenarii[no_scen]->ds_client[scenarii[no_scen]->nb_clients-1] = debit;
    else {
       cout << "Error (re)allocating memory";
       exit (1);
    }
}
*/

void pointTest::set_scenario(int no_scen, int _nb_clients, int debit_client_kbps) {
    //scenario sortie = (scenario)malloc(sizeof(struct _scenario));
    if (_nb_clients> 10) cout << "big pb " << _nb_clients << endl;
    scenarii[no_scen]->nb_clients = _nb_clients;
    scenarii[no_scen]->ds_client_kbps = new int[_nb_clients];
    for (int i = 0; i < _nb_clients; i++)
        scenarii[no_scen]->ds_client_kbps[i] = debit_client_kbps;
    //scenarii[no_scen]->serveur;
    //scenarii[no_scen]->sinr = NULL;
    scenarii[no_scen]->largeurNecessaire_kHz = -1;
    //return sortie;
}

/*
scenario new_scenario(int _nb_clients, int debit_client) {
    //scenario sortie = (scenario)malloc(sizeof(struct _scenario));
    scenario->nb_clients = _nb_clients;
    scenario->ds_client = new int[_nb_clients];
    for (int i = 0; i < _nb_clients; i++)
        scenario->ds_client[i] = debit_client;
    scenario->serveur;
    scenario->sinr = NULL;
    scenario->largeurNecessaire_kHz = -1;
    //return sortie;
}
*/
/*
scenario new_scenario(secteur* _serveur, int _nb_clients, double * _ds_client, tabSinr* _sinr, double _largeurNecessaire_kHz) {
    scenario sortie = (scenario)malloc(sizeof(struct _scenario));;
    scenario->nb_clients;
    scenario->largeurNecessaire_kHz;
    scenario->ds_client;

    scenario->nb_clients;
    scenario->nb_clients;

        secteur* serveur;
    int
    int *ds_client;
    tabSinr* sinr;
    double largeurNecessaire_kHz;
    return sortie;
}
*/

/** Fonction ajoutant un signal à la structure 'signaux' recus
	@param pR puissance du signal à ajouter
	@param _e secteur dont provient le signal à ajouter
*/
signal new_signal(double pR, secteur* _e) {
    //interferent sortie = new interferent();
    signal sortie = (signal)malloc(sizeof(struct _signal));;
    sortie->puissRecue_dBm = pR;
    sortie->sect = _e;
    sortie->sinr = NULL;
//cout << "-----------------------------" << endl << pointTest::nb_scenarii  << "-----------------------------" << endl;
    return sortie;
}

signal new_signal(double pR, secteur* _e, int nb_sce){
    //interferent sortie = new interferent();
    signal sortie = (signal)malloc(sizeof(struct _signal));;
    sortie->puissRecue_dBm = pR;
    sortie->sect = _e;
//cout << "------------2----------------" << endl << pointTest::nb_scenarii  << endl << "---------------2-------------" << endl;
    sortie->sinr = new tabSinr[nb_sce];
//    sortie->sinr = (tabSinr*)malloc(nb_sce*sizeof(struct _tabSinr));;
    return sortie;
}

/// Constructeur vide
pointTest::pointTest() {}
/// Destructor
pointTest::~pointTest() {}

/// Constructeur
/** Constructeur de l'objet pointTest
	@param _no numéro du point de test
	@param (_i,_j) indices du point de test dans la grille des point de test
	@param (_x,_y,_z) coordonnées en mètres du point de test
*/
pointTest::pointTest(int _no, int _i, int _j, double _x, double _y, double _z) {
    no = _no;
    pi = _i;
    pj = _j;
    x = _x;
    y = _y;
    z = _z;
    have_client = false;
//    nb_clients = 0;
//    puissanceRecue_dBm = Environement::get_puissanceNulle_dBm();
    i_plusProcheSite = -1;
    j_plusProcheSite = -1;
//    serveur = NULL;
//    sinr_dB = Environement::get_puissanceNulle_dBm();
//    sinr_su = -1;
//    largeurNecessaire_kHz = -1;
}

/** Libère le point de test de ces clients et de siganux recus
*/
void pointTest::free() {
//    nb_clients = 0;
//    puissanceRecue_dBm = Environement::get_puissanceNulle_dBm();
//    serveur = NULL;
//    sinr_dB = Environement::get_puissanceNulle_dBm();
//    sinr_su = -1;
//    largeurNecessaire_kHz = -1;
	signaux.clear();
}

/** Ajoute aux 'signaux' recus du point de test les signaux provenant des secteurs
    de la première couronne de voisins du site (i_plusProcheSite, j_plusProcheSite)
    @param lesS tableau des sites de la zone
*/
void pointTest::set_signaux_voisins(site*** lesS) {
    int i = i_plusProcheSite;
    int j = j_plusProcheSite;

    add_signal(i  , j  , lesS);
    add_signal(i-1, j-1, lesS);
    add_signal(i  , j-1, lesS);
    add_signal(i+1, j-1, lesS);
    add_signal(i-1, j  , lesS);
    add_signal(i+1, j  , lesS);
    add_signal(i  , j+1, lesS);
}

/** Ajoute aux 'signaux' recus du point de test les signaux provenant des secteurs
    des deux premières couronnes de voisins du site (i_plusProcheSite, j_plusProcheSite)
    @param lesS tableau des sites de la zone
*/
void pointTest::set_signaux_voisins_2(site*** lesS) {
    int i = i_plusProcheSite;
    int j = j_plusProcheSite;

    add_signal(i-1, j-1, lesS);
    add_signal(i-1, j  , lesS);
    add_signal(i-1, j+1, lesS);

    add_signal(i  , j-1, lesS);
    add_signal(i  , j  , lesS);
    add_signal(i  , j+1, lesS);
    add_signal(i  , j+2, lesS);

    add_signal(i+1, j-1, lesS);
    add_signal(i+1, j  , lesS);
    add_signal(i+1, j+1, lesS);
    add_signal(i+1, j+2, lesS);

    add_signal(i+2, j  , lesS);
    add_signal(i+2, j+1, lesS);
    add_signal(i+2, j+2, lesS);
}

/** Ajoute aux 'signaux' recus du point de test tout les signaux du réseaux
    @param lesS tableau des sites de la zone
*/
void pointTest::set_signaux_total(site*** lesS) {
    for (int i=0; i<site::size_x; i++)
        for (int j=0; j<site::size_y; j++)
            add_signal(i, j, lesS);
}

/** Ajoute aux 'signaux' recus du point de test les signaux des sites voisins
    @param lesS tableau des sites de la zone
*/
void pointTest::set_signaux_nearest_sites() {
    for (int i=0; i<nb_nearest_sites; i++)
        add_signal(nearest_sites[i]);
//	vector<signal>::iterator it;
//	for ( it=signaux.begin() ; it < signaux.end(); it++ )
//		(*it)->sinr = new tabSinr[nb_scenarii];
}

/** Ajoute aux signaux reçus du point de test les signaux provenant des secteurs du site (i,j)
	@param (i,j) identifiant du site
	@param lesS matrice des sites
*/
void pointTest::add_signal(tab_sites s) {
    double p;
    for (int k=0; k<site::lesSites[s.no_site]->get_nb_secteur(); k++) {
        p = calculPuissanceRecue_dB(site::lesSites[s.no_site]->get_secteur(k));
        if (p>Environement::get_puissanceNegligeable_dBm())
            signaux.push_back(new_signal(p, site::lesSites[s.no_site]->get_secteur(k),nb_scenarii));
    }
}

void pointTest::update_signal(secteur* s) {
    vector<signal>::iterator it;
    for ( it=signaux.begin() ; it < signaux.end(); it++ ) {
		if (s==(*it)->sect) {
		//cout << (*it)->puissRecue_dBm << " -> ";
			(*it)->puissRecue_dBm = calculPuissanceRecue_dB(s);
		//cout << (*it)->puissRecue_dBm << endl;
		}
    }
}

/** Ajoute aux signaux reçus du point de test les signaux provenant des secteurs du site (i,j)
	@param (i,j) identifiant du site
	@param lesS matrice des sites
*/
void pointTest::add_signal(int i, int j, site*** lesS) {
    double p;
    if ((i>=0)&&(j>=0)&&(i<site::size_x)&&(j<site::size_y))
        for (int k=0; k<lesS[i][j]->get_nb_secteur(); k++) {
            p = calculPuissanceRecue_dB(lesS[i][j]->get_secteur(k));
            if (p>Environement::get_puissanceNegligeable_dBm())
                signaux.push_back(new_signal(p, lesS[i][j]->get_secteur(k)));
        }
}

/** Calcul la puissance recue par le point de test et provenant du secteur 'theSect'
	@param theSect
	@return la puissance recue
*/
double pointTest::calculPuissanceRecue_dB(secteur* theSect) {
    double pR =   theSect->get_puissance_dBm()
                  + theSect->get_gain_dBi()
                  - theSect->get_perte_cable_dBm()
                  - Propagation::pathLossHataMediumCity(this, theSect, Environement::get_freqE_MHz())
                  - calculAffaiblissementV_dB(theSect)
                  - calculAffaiblissementH_dB(theSect);
/*if(this->have_client){
    cout << theSect->get_puissance_dBm() << endl;
    cout << theSect->get_gain_dBi() << endl;
    cout << Propagation::pathLossHataMediumCity(this, theSect, Environement::get_freqE_MHz()) << endl;
    cout << calculAffaiblissementV_dB(theSect) << endl;
    cout << calculAffaiblissementH_dB(theSect) << endl;
    cout << pR << endl;
    exit(0);
}*/
    return pR;
}

/** Calcul l'affaiblissement verticale (en dB) du diagramme de rayonnement
	de l'antenne placé sur le secteur 'theSect'
	@param theSect
	@return l'affaiblissement en dB (positif)
*/
double pointTest::calculAffaiblissementV_dB(secteur* theSect) {
    double d_m = MathB::dist(x, y, theSect->get_x(), theSect->get_y());
    double theta_radian;
    if (d_m==0) theta_radian = M_PI_2;
    else theta_radian = atan((theSect->get_z()-z)/d_m);
    //else theta_radian = atan2((theSect->get_z()-z),d_m);

	double dtheta_radian = theta_radian - theSect->get_tiltE_radian() - theSect->get_tiltM_radian();
    return theSect->get_diagrammeV_radian2dB( dtheta_radian );
}

/** Calcul l'affaiblissement horizontale (en dB) du diagramme de rayonnement
	de l'antenne placé sur le secteur 'theSect'
	@param theSect
	@return l'affaiblissement en dB (positif)
*/
double pointTest::calculAffaiblissementH_dB(secteur* theSect) {
    double phi_radian;
    if ((x==theSect->get_x())&&(y==theSect->get_y())) phi_radian = 0;
    else phi_radian = MathB::angle(theSect->get_x(), theSect->get_y(), x, y);
    return theSect->get_diagrammeH_radian2dB( (phi_radian-MathB::azimut2trigo_radian(theSect->get_azimut_radiant())) );
}

/** Calcul le SINR DL au niveau du point de test avec comme porteuse un secteur donné et pour un scénarion donné\n
	Note le résulat dans la structure 'tabSinr' sinr des scenarii
	@param s secteur porteur de l'information
	@param scen numéro du scénario
*/
void pointTest::calculSinr(secteur *s, int scen) {
    double tx= s->get_txOccP(scen);
    double puissancePorteur_mW;
    double sommeI_mW = 0;
    vector<signal>::iterator it, it1;
    int porteuse = s->get_porteuse();
    for ( it=signaux.begin() ; it < signaux.end(); it++ ) {
		if (s==(*it)->sect) {
            puissancePorteur_mW = Unite::dBm2mWatt((*it)->puissRecue_dBm);
//            tx = (*it)->sect->get_txOccP(scen);
//            sommeI_mW += max(0., (tx-1.)*puissancePorteur_mW);// pourquoi (t-1)
            it1 = it;
        } else if (porteuse == (*it)->sect->get_porteuse()){
			sommeI_mW += (*it)->sect->get_txOccP(scen)*tx*Unite::dBm2mWatt((*it)->puissRecue_dBm);
		}
    }

    sommeI_mW += s->get_bruitBlanc_mW();
    (*it1)->sinr[scen].sinr_su = puissancePorteur_mW/sommeI_mW;
    (*it1)->sinr[scen].sinr_dB = Unite::sansUnite2dB((*it1)->sinr[scen].sinr_su);
//    (*it1)->sinr_su = puissancePorteur_mW/sommeI_mW;
//    (*it1)->sinr_dB = Unite::sansUnite2dB((*it1)->sinr_su);
}

/** Calcul les SINR DL pour un secteur donné et pour tous les scenarii
*/
void pointTest::calculSinr(secteur *s) {
    for (int k=0; k<nb_scenarii; k++)
		calculSinr(s, k);
}

/** Calcul les SINR DL de tout les secteurs et tous les scenarii
*/
void pointTest::calculTousSinr() {
	for (unsigned int j=0; j<signaux.size(); j++)
		for (int k=0; k<nb_scenarii; k++)
			calculSinr(signaux[j]->sect,k);
}

/** Calul le SINR de tout les points de test de la liste 'pt'
	@param pt liste des points de test
	@param taille de la liste
*/
void pointTest::calcul_sinr(pointTest ** pt, int taille) {
    for (int i=0; i<taille; i++) {
        for (int j=0; j<pt[i]->get_nbSignaux(); j++) {
            pt[i]->calculSinr(pt[i]->get_secteur(j));
        }
        pt[i]->association_best_sinr();
        pt[i]->set_largeurNecessaire_kHz();
    }
}

/** idem que calcul_sinr(pointTest ** pt, int taille) mais avec un vecteur au lieu d'un tableau
*/
void pointTest::calcul_sinr(vector<pointTest*> pt) {
    vector<pointTest*>::iterator it;
    for ( it=pt.begin() ; it < pt.end(); it++ ) {
        for (int j=0; j<(*it)->get_nbSignaux(); j++)
            (*it)->calculSinr((*it)->get_secteur(j));
        (*it)->association_best_sinr();
        (*it)->set_largeurNecessaire_kHz();
    }
}

/** Retourne les 4 hexagones les plus proche du point (x,y).
    L'espace étant un tapis d'hexagones réguliers de coté 'a'
    et le premier hexagone (i=0,j=0) est centré à (x=0,y=0)
    On retourne l'indice (i,j) pour identifier les 4 hexagones
    centré en (i,j) (i+1,j) (i,j+1) (i+1,j+1)
    @param coteHexagonal coté des hexagones
    @param s matrice des sites
*/
void pointTest::find_site_ij_hexagonal(int coteHexagonal, site*** s) {
    int i, j;
    int i_max = site::size_x;
    int j_max = site::size_y;
    double h = coteHexagonal*sqrt(3.)/2.;

    i = floor(2.*x/(3.*coteHexagonal));
    j = floor((y*1./h-(i%2==1))/2.);

    if (i<0) i=0;
    if (i>i_max-2) i=i_max-2;
    if (j<0) j=0;
    if (j>j_max-2) j=j_max-2;

    double d, d_min;
    d_min = MathB::dist(x,y,s[i][j]->get_x(),s[i][j]->get_y());
    i_plusProcheSite = i;
    j_plusProcheSite = j;

    d = MathB::dist(x,y,s[i+1][j]->get_x(),s[i+1][j]->get_y());
    if (d<d_min) {
        d_min = d;
        i_plusProcheSite = i+1;
        j_plusProcheSite = j;
    }
    d = MathB::dist(x,y,s[i][j+1]->get_x(),s[i][j+1]->get_y());
    if (d<d_min) {
        d_min = d;
        i_plusProcheSite = i;
        j_plusProcheSite = j+1;
    }
    d = MathB::dist(x,y,s[i+1][j+1]->get_x(),s[i+1][j+1]->get_y());
    if (d<d_min) {
        d_min = d;
        i_plusProcheSite = i+1;
        j_plusProcheSite = j+1;
    }
}

void pointTest::find_nearest_site() {
    int besti = 0;
    double _dist=0, dist_min = INFINITY;
    for (int i=0; i < site::size; i++) {
        _dist = MathB::dist(site::lesSites[i]->get_x(), site::lesSites[i]->get_y(), this->get_x(),  this->get_y());
        if (_dist<dist_min) {
            dist_min = _dist;
            besti = i;
        }
    }
    plusProcheSite = besti;
}

void pointTest::find_nearest_sites(int nb) {
    nb_nearest_sites = nb;
    nearest_sites = new tab_sites[nb];
    double _dist=0;//, dist_min = INFINITY;
    for (int i=0; i < site::size; i++) {
        _dist = MathB::dist(site::lesSites[i]->get_x(), site::lesSites[i]->get_y(), this->get_x(),  this->get_y());
        int j=nb;
        while ((j>0) && (_dist < nearest_sites[j-1].dist_site_m)) j--;
        if (j<nb) {
            for (int k=nb-1; k>j; k--){
                nearest_sites[k].dist_site_m = nearest_sites[k-1].dist_site_m;
                nearest_sites[k].no_site = nearest_sites[k-1].no_site;
            }
            nearest_sites[j].dist_site_m = _dist;
            nearest_sites[j].no_site = i;
        }
    }
}
// calcul nearest sites et les stocke dans un vecteur
void pointTest::find_nearest_sites2(int nb, tab_sites* &nearest_sites){
    nb_nearest_sites = nb;
    nearest_sites = new tab_sites[nb];
    double _dist=0;//, dist_min = INFINITY;
    for (int i=0; i < site::size; i++) {
        _dist = MathB::dist(site::lesSites[i]->get_x(), site::lesSites[i]->get_y(), this->get_x(),  this->get_y());
        int j=nb;
        while ((j>0) && (_dist < nearest_sites[j-1].dist_site_m)) j--;
        if (j<nb) {
            for (int k=nb-1; k>j; k--){
                nearest_sites[k].dist_site_m = nearest_sites[k-1].dist_site_m;
                nearest_sites[k].no_site = nearest_sites[k-1].no_site;
            }
            nearest_sites[j].dist_site_m = _dist;
            nearest_sites[j].no_site = i;
        }
    }
}


void pointTest::set_circle_of_influence(double radius_of_influence) {
	for (int i=0; i<nb_nearest_sites; i++) {
		if (nearest_sites[i].dist_site_m < radius_of_influence)
			site::lesSites[nearest_sites[i].no_site]->circle_of_influence.push_back(this);
	}
}

/** 1. Localise ces points de test au secteur le plus proche sur un pavage hexagonale \n
    2. Associe le point test géographiquement au secteur le plus proche
    @param coteHexag taille du coté de l'hégagone
    @param lesS martice des sites hexagonales de zone
*/
/*void pointTest::association_geographique(int coteHexag, site*** lesS) {
    if (this->have_client) {
        // localisation du point de test dans le pavage hexagonale des sites
        int no_secteur;
        if ((i_plusProcheSite == -1) || (j_plusProcheSite == -1))
			find_site_ij_hexagonal(coteHexag, lesS);
        no_secteur = lesS[i_plusProcheSite][j_plusProcheSite]->find_secteur(x, y);
        //find_secteur(x, y, coteHexag, lesS, i, j, no_secteur);
        // assiciation des points de test avec le serveur le plus proche
        serveur = lesS[i_plusProcheSite][j_plusProcheSite]->get_secteur(no_secteur);
        // incrémentation du secteur d'un client suplémentaire
        //serveur->add_client(1);
        // ajouter le point de test à la cellule du nouveau serveur
        serveur->add_cellule(this);
    }
}*/


/** Associe le point de test au secteur lui offrant le plus fort signal \n
	et le desassocie (si nécessaire) de son ancien serveur
*/
/*
void pointTest::association_best_power() {
    secteur *ancien_serveur = serveur;
    if (signaux.size() != 0) {
        vector<signal>::iterator it, itServeur;
        itServeur = signaux.begin();
        double power_max = (*itServeur)->puissRecue_dBm;
        for ( it=signaux.begin() ; it < signaux.end(); it++ ) {
            if (power_max < (*it)->puissRecue_dBm) {
                power_max = (*it)->puissRecue_dBm;
                itServeur = it;
            }
        }
        serveur = (*itServeur)->sect;
        puissanceRecue_dBm = (*itServeur)->puissRecue_dBm;
        // mise à jour des cellules des serveurs anciens et nouveau
        if (ancien_serveur == NULL) {
            // ajouter le point de test à la cellule du nouveau serveur
            serveur->add_cellule(this);
        } else if (serveur != ancien_serveur) {
            // ajouter le point de test à la cellule du nouveau serveur
            serveur->add_cellule(this);
            // supprime le point de test à la cellule du nouveau serveur
            ancien_serveur->delete_cellule(this);
        }
    }
}
*/

/** Associe le point de test au secteur lui offrant le plus fort signal \n
	et le desassocie (si nécessaire) de son ancien serveur
*/
void pointTest::association_best_power() {
    for (int i=0; i<nb_scenarii; i++){
        secteur *ancien_serveur = scenarii[i]->serveur;
        if (signaux.size() != 0) {
            vector<signal>::iterator it, itServeur;
            itServeur = signaux.begin();
            double power_max = (*itServeur)->puissRecue_dBm;
            for ( it=signaux.begin() ; it < signaux.end(); it++ ) {
                if (power_max < (*it)->puissRecue_dBm) {
                    power_max = (*it)->puissRecue_dBm;
                    itServeur = it;
                }
            }
            scenarii[i]->serveur = (*itServeur)->sect;
            scenarii[i]->puissanceRecue_dBm = (*itServeur)->puissRecue_dBm;
            // mise à jour des cellules des serveurs anciens et nouveau
            if (ancien_serveur == NULL) {
                // ajouter le point de test à la cellule du nouveau serveur
                scenarii[i]->serveur->add_cellule(i,this);
            }
            else if (scenarii[i]->serveur != ancien_serveur) {
                // ajouter le point de test à la cellule du nouveau serveur
                scenarii[i]->serveur->add_cellule(i,this);
                // supprime le point de test à la cellule du nouveau serveur
                ancien_serveur->delete_cellule(i,this);
            }
        }
    }
}

/** Associe le point de test au secteur lui offrant le plus fort SINR DL \n
	et le desassocie (si nécessaire) de son ancien serveur
*/
void pointTest::association_best_sinr() {
    for (int i=0; i<nb_scenarii; i++){
		secteur *ancien_serveur = scenarii[i]->serveur;
		if (scenarii[i]->sinr_su == -1 ){
			calculTousSinr();
		}

		if (signaux.size() != 0) {
			vector<signal>::iterator it, itServeur;
			itServeur = signaux.begin();
			double sinr_max = (*itServeur)->sinr[i].sinr_dB;
			for ( it=signaux.begin() ; it < signaux.end(); it++ ) {
				if (sinr_max < (*it)->sinr[i].sinr_dB) {
					sinr_max = (*it)->sinr[i].sinr_dB;
					itServeur = it;
				}
			}
			scenarii[i]->serveur = (*itServeur)->sect;
			scenarii[i]->puissanceRecue_dBm = (*itServeur)->puissRecue_dBm;
			scenarii[i]->sinr_dB = (*itServeur)->sinr[i].sinr_dB;
			scenarii[i]->sinr_su = (*itServeur)->sinr[i].sinr_su;
			// mise à jour des cellules des serveurs anciens et nouveau
			if (ancien_serveur == NULL) {
				// ajouter le point de test à la cellule du nouveau serveur
				scenarii[i]->serveur->add_cellule(i,this);
			} else if (scenarii[i]->serveur != ancien_serveur) {
				// supprime le point de test à la cellule du nouveau serveur
				ancien_serveur->delete_cellule(i,this);
				// ajouter le point de test à la cellule du nouveau serveur
				scenarii[i]->serveur->add_cellule(i,this);
			}
		}
    }
}

/** Calcul et fixe la largeur de bande nécessaire au point de test correspondant à son SINR DL
*/
void pointTest::set_largeurNecessaire_kHz() {
    for (int i=0; i<nb_scenarii; i++){
		//cout << "sinr db=" << scenarii[i]->sinr_dB << endl;
		double d = Environement::efficacite_spectrale(scenarii[i]->sinr_dB);
		//cout << "bande =" << d << endl;
		scenarii[i]->largeurNecessaire_kHz = 0;
		if (d!=0) {
			for (int j=0; j < get_nb_clients(i); j++)
				scenarii[i]->largeurNecessaire_kHz += Environement::nb_block_utile(get_ds_client(i,j),d)*Environement::largeurBlock_kHz;
			//:get_largeurBandeClient_kHz get_debitNecessaireVoix_kbps()/d;
			//cout << "largeur =" << scenarii[i]->largeurNecessaire_kHz << endl;
		}
		else {
			// supprime le point de test à la cellule du nouveau serveur
			//cout << "le numero du secteur qui presente meilleur sinr " << scenarii[i]->serveur->get_no_absolue();
			scenarii[i]->serveur->delete_cellule(i,this);
			// ajouter le point de test à la cellule du nouveau serveur
			secteur::secteurNonCouvert->add_cellule(i,this);
			scenarii[i]->serveur = secteur::secteurNonCouvert;
			scenarii[i]->largeurNecessaire_kHz = 0;
		}
    }
}
/** Retourne le secteur ayant le meilleur SINR lorsque le point test n'est pas couvert
*/
secteur* pointTest::get_secteurBestSinr(){
for (int i=0; i<nb_scenarii; i++){
    double d = Environement::efficacite_spectrale(scenarii[i]->sinr_dB);
    if(d==0){
    secteur* secteurBestSinr = scenarii[i]->serveur;
    return secteurBestSinr;
    //cout << "le secteur best Sinr du point test non couvert est : " << secteurBestSinr->get_no_absolue;
    }
    }
}

/** Change le point de test de cellule (nouveau serveur) pour un scenario donné
	@param no_scenario numéro du scenario
	@param new_secteur adresse du nouveau serveur
*/
void pointTest::change_cellule(int no_scenario, secteur* new_secteur) {
	get_serveur(no_scenario)->delete_cellule(no_scenario, this);
	new_secteur->add_cellule(no_scenario, this);
}

/* fonction plus utilisée
void pointTest::changement_secteur(secteur* nvSect, int k) {
    secteur* anSect = serveur;
    double anPuissanceRecue_dBm = puissanceRecue_dBm;
    anSect->set_nb_clients(anSect->get_clients()-nb_clients);
    serveur = nvSect;
    nvSect->set_nb_clients(nvSect->get_clients()+nb_clients);
    puissanceRecue_dBm = get_puissanceSignal_dBm(k);
    set_signal(k, anSect, anPuissanceRecue_dBm);
    calculSinr(nvSect);
}
*/

/* fonction plus utilisée
void pointTest::set_signal(int k, secteur* anSect, double anPuissanceRecue_dBm) {
    signaux.at(k)->puissRecue_dBm = anPuissanceRecue_dBm;
    signaux.at(k)->sect = anSect;
}
*/

/** Enregistre la carte
	\li des puissances recues
	\li des niveau de SINR
	\li les secteurs
	\li des sites
	@param filename nom du fichier dans lequel on enregistre les données
	@param thetype indique quelle carte on enregistre \n
			si thetype = 0 => carte des puissances recues \n
			si thetype = 1 => carte des SINRs \n
			si thetype = 2 => carte des secteurs \n
			si thetype = 3 => carte des sites \n
	@param no_scen numéro du scenario de trafic\n
*/
void pointTest::save_map_as(char* filename, int thetype, int no_scen) {
    // si thetype = 0 => puissance recue
    // si thetype = 1 => sinr
    // si thetype = 2 => secteur
    // si thetype = 3 => site
    fstream filestr;
    cout << "Enregistement du fichier " << filename << endl;
    filestr.open (filename, fstream::out);
    for (int j=0; j<size_y; j++) {
        for (int i=0; i<size_x; i++) {
            if ((lesPT[i][j]->get_have_client())&&(lesPT[i][j]->get_nb_clients(no_scen) != 0))
            {
                switch (thetype) {
                case 0 :
                    filestr << lesPT[i][j]->get_puissanceRecue_dBm(no_scen) << " ";
                    break;
                case 1 :
                    //filestr << "salut didinchik" ;
                    filestr << lesPT[i][j]->get_sinr_dB(no_scen) << " ";
                    break;
                case 2 :
                    filestr << lesPT[i][j]->get_serveur(no_scen)->get_no()*1.+0*lesPT[i][j]->get_serveur(no_scen)->get_no_absolue() << " ";
                    break;
                case 3 :
					if (lesPT[i][j]->get_serveur(no_scen)==secteur::secteurNonCouvert) filestr << "-1**";
                    else filestr << lesPT[i][j]->get_serveur(no_scen)->get_site()->get_no() << " ";
                    break;
                }
            }
            else filestr << "-10 ";
        }
        filestr << endl;
    }
    filestr.close();
}

void pointTest::save_map_TM_as(char* filename, int thetype, int no_scen, int nb_tp_a) {
    // si thetype = 0 => best puissance recue
    // si thetype = 1 => sinr
    // si thetype = 2 => secteur
    // si thetype = 3 => site
    // si thetype = 4 => couvert/pas couvert
    fstream filestr;
	vector<signal>::iterator it;
	cout << "Enregistement du fichier " << filename << " pour Territoire Mobile." << endl;
    filestr.open (filename, fstream::out);
	if (thetype>1) {
		filestr << "ndMailleX;" << size_x << endl;
		filestr << "ndMailleY;" << size_y << endl;
		filestr << "Xmin;" << xMin << endl;
		filestr << "Ymin;" << yMin << endl;
		filestr << "Pdm;" << pdm << endl << endl;
		filestr << "IndiceMailleY;IndiceMailleX;Station1;PPC1;Station2;PPC2;Station3;PPC3;Station4;PPC4;...;" << endl;
	}
	else
		filestr << "nbLignes\t" << nb_tp_a << endl;
    for (int j=0; j<size_y; j++) {
        for (int i=0; i<size_x; i++) {
            if ((lesPT[i][j]->get_have_client())&&(lesPT[i][j]->get_nb_clients(no_scen) != 0))
            {
                switch (thetype) {
                case 0 :
					filestr << lesPT[i][j]->get_x() << "\t" << lesPT[i][j]->get_y()-2000000;
					for (int k =0; k < nb_scenarii; k++)
						filestr << "\t" << lesPT[i][j]->get_puissanceRecue_dBm(k)+100;
                    break;
                case 1 :
					filestr << lesPT[i][j]->get_x() << "\t" << lesPT[i][j]->get_y()-2000000;
					for (int k =0; k < nb_scenarii; k++)
						filestr << "\t" << lesPT[i][j]->get_sinr_dB(k);
                    break;
                case 2 :
					filestr << j << ";" << i;
					if (lesPT[i][j]->get_serveur(no_scen)==secteur::secteurNonCouvert)
						filestr << ";1000;0";
					else
						filestr << ";" << lesPT[i][j]->get_serveur(no_scen)->get_no_absolue() << ";0";
                    break;
                case 3 :
					filestr << j << ";" << i;
					if (lesPT[i][j]->get_serveur(no_scen)==secteur::secteurNonCouvert)
						filestr << lesPT[i][j]->get_serveur(no_scen)->get_no_absolue() <<";;;0";
                    else
						filestr << ";" << lesPT[i][j]->get_serveur(no_scen)->get_site()->get_no() << ";0";
                    break;
                case 4 :
					filestr << j << ";" << i;
					if (lesPT[i][j]->get_serveur(no_scen)==secteur::secteurNonCouvert)
						filestr << ";0;0";
                    else
						filestr << ";1;0";
                    break;
                case 5:
                    filestr << lesPT[i][j]->get_x() << "\t" << lesPT[i][j]->get_y()-2000000;
					for (int k =0; k < nb_scenarii; k++){
					  //if (lesPT[i][j]->get_serveur(no_scen)==secteur::secteurNonCouvert){
					    //filestr << lesPT[i][j]->get_x() << "\t" << lesPT[i][j]->get_y()-2000000;
						filestr << "\t " << lesPT[i][j]->get_sinr_dB(k) << "\t ";
						filestr << "\t " << lesPT[i][j]->get_serveur(no_scen)->get_no_absolue()<< "\t";
						filestr << "\t " << lesPT[i][j]->get_nb_clients(no_scen) << "\t";
					}
					//}
                    break;

                }
                filestr << endl;
            }
        }
    }
    filestr.close();
}


/** Affiche les paramètres du point de test
*/
void pointTest::display_simple() {
    cout << endl << "point test " << no << " : " << pi << "," << pj <<  " -> position : " << x << " "  << y << " "  << z << endl;
}

void pointTest::display_detail() {
    cout << endl << "point test " << no << " : " << pi << "," << pj <<  " -> position : " << x << " "  << y << " "  << z << endl;
    cout << "nb_scenarii = " << nb_scenarii << endl;
    for (int i=0; i<nb_scenarii; i++) {
        cout << "scenaro " << i << " : nb_clients = " << scenarii[i]->nb_clients << endl;
        for (int j = 0; j<scenarii[i]->nb_clients; j++)
            cout << "debit client " << j << " : " << scenarii[i]->ds_client_kbps[j] << endl;
    }
}

void pointTest::display_detail2() {
    cout << endl << "point test " << no << " : " << pi << "," << pj <<  " -> position : " << x << " "  << y << " "  << z << endl;
    cout << "nb_scenarii = " << nb_scenarii << endl;
    for (int i=0; i<nb_scenarii; i++) {
        cout << "scenaro " << i << " : nb_clients = " << scenarii[i]->nb_clients << endl;
        for (int j = 0; j<scenarii[i]->nb_clients; j++)
            cout << "debit client " << j << " : " << scenarii[i]->ds_client_kbps[j] << endl;
    }
    cout << "Nearest_sites" << endl;
    for (int i=0; i<nb_nearest_sites; i++)
        cout << nearest_sites[i].no_site << " " << nearest_sites[i].dist_site_m << endl;

    vector<signal>::iterator it;
    for ( it=signaux.begin() ; it < signaux.end(); it++ ) {
        cout << "signaux :   " ;
        (*it)->sect->display_simple();
        cout << "---> "  << (*it)->puissRecue_dBm << endl;
        //cout << "---> "  << MathB::dist((*it)->sect->get_x(), (*it)->sect->get_y(), x, y) << endl;
    }
}

/*
void pointTest::display() {
    cout << endl << "point test : " << pi << "," << pj <<  " -> position : " << x << " "  << y << " "  << z << " : nb clients = " << nb_clients << endl;
    vector<signal>::iterator it;
    cout << "secteur porteurse : ";
    get_serveur()->display();
    cout  << "---> " << puissanceRecue_dBm << " sinr=" << sinr_dB << endl;
    cout << "---> "  << MathB::dist(get_serveur()->get_x(), get_serveur()->get_y(), x, y) << endl;
    for ( it=signaux.begin() ; it < signaux.end(); it++ ) {
        cout << "signaux :   " ;
        (*it)->sect->display();
        cout << "---> "  << (*it)->puissRecue_dBm << endl;
        cout << "---> "  << MathB::dist((*it)->sect->get_x(), (*it)->sect->get_y(), x, y) << endl;
    }
}
*/

