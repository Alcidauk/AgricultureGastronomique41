#include "site.h"
#include "secteur.h"

using namespace std;

/// Constructeur vide
site::site() {}
/// Destructor
site::~site() {}


/// Constructeur
/** Constructeur simple d'un objet site
	@param (_x,_y,_z) coordonnées en métre du site
*/
site::site(int _no, char* _name, double _x, double _y, double _z) {
    no = _no;
    //name =  (char*)malloc(80*sizeof(char));
    strcpy (name,_name);
    x = _x;
    y = _y;
    z = _z;
}

/// Constructeur
/** Constructeur d'un objet site
	@param (_x,_y,_z) coordonnées en métre du site
	@param (_si,_sj) indice du site dans la grille hexagonale des sites
	@param p patern de réutilisation des fréquence \n
		si p=0 alors schema 1x1x1 \n
		si p=1 alors schema 1x3x1 \n
		si p=2 alors schema 1x3x3
*/
site::site(double _x, double _y, double _z, int _si, int _sj, int p) {
    si = _si;
    sj = _sj;
    x = _x;
    y = _y;
    z = _z;
    patern = p;
    int nb_porteuses, type_antenne;

	switch (patern) {
		case 0 : //schema 1x1x1
			nb_secteur = 1;
			nb_porteuses = 1;
			type_antenne = 0; // antenne omnidirectionnelle
			//secteur::nbClientsMaxPorteuse = secteur::nbClientsMaxBande/nb_porteuses;
			//sect = new secteur*[nb_secteur];
			for (int i=0; i<nb_secteur; i++)
				//sect[i] = new secteur(i, this, type_antenne, 0, 0);
                sect.push_back(new secteur(i, this, type_antenne, 0, 0));
                break;
        case 1 : //schema 1x3x1
			nb_secteur = 3;
			nb_porteuses = 1;
			type_antenne = 1; // antenne directive
			//secteur::nbClientsMaxPorteuse = secteur::nbClientsMaxBande/nb_porteuses;
			//sect = new secteur*[nb_secteur];
			for (int i=0; i<nb_secteur; i++)
                sect.push_back(new secteur(i, this, type_antenne, Unite::degre2radian(60+120.*i), 0));
                //sect[i] = new secteur(i, this, type_antenne, Unite::degre2radian(60+120.*i), 0);
			break;
		case 2 : //schema 1x3x3
			nb_secteur = 3;
	        nb_porteuses = 3;
			type_antenne = 1; // antenne directive
			//secteur::nbClientsMaxPorteuse = secteur::nbClientsMaxBande/nb_porteuses;
			//sect = new secteur*[nb_secteur];
			for (int i=0; i<nb_secteur; i++)
				sect.push_back(new secteur(i, this, type_antenne, Unite::degre2radian(60+120.*i), i));
				//sect[i] = new secteur(i, this, type_antenne, Unite::degre2radian(60+120.*i), i);
			break;
	}
}

void site::load_sites_simple(double cote_hexag, double haut_hexag, int z_site, int pattern_reutilisation)
{
    // Création des sites et des secteurs
    lesSitesM = new site**[size_x];
    for (int i=0; i<size_x; i++) {
        lesSitesM[i] = new site*[size_y];
        for (int j=0; j<size_y; j++) {
            site::lesSitesM[i][j] = new site(i*3.*cote_hexag/2., (2.*j+(i%2==1))*haut_hexag, z_site, i, j, pattern_reutilisation);
            //cout << i << " " << j << " " << i*3.*cote_hexag/2. << " " << (2.*j+(i%2==1))*haut_hexag << " " << endl;
        }
    }
}
/// chargement des informations des sites et affectation des fréquences de départ déterministe

void site::load_sites_file(char* file_name)
{
    GInputFile infile(file_name) ;
    char* buf ;
    //char *tok ;
    infile.open() ;
    cout<<"\n>> Chargement des informations des sites et secteurs\n";
    buf=infile.readLine();
    size = infile.getNextIntToken();
    //int size_secteur = infile.getNextIntToken();
    infile.getNextIntToken();
    buf=infile.readLine();

    lesSites = new site*[size];
    int _no = -1, no_sect = 0, type_antenne, patern=2, _porteurse, no_sect_absolu=0;
    //char * name1 = "rien";
    int thex1 = -1, they1 = -1, s_sol = -1;
    int id, azimut, tilt_m, tilt_e, tilt_t, i_tp, j_tp;
    char name_s[80], name_a[100], dbm[10];
    double gain, power, thex, they, thez, h, perte_a;

    while ((buf=infile.readUncommentedLine())!=NULL) {
        id = infile.getNextIntToken();
        strcpy(name_s,infile.getNextToken());
        //cout << name_s << endl;
        thex = infile.getNextFloatToken();
        they = infile.getNextFloatToken();
        s_sol = infile.getNextFloatToken();
        strcpy(name_a,infile.getNextToken());
        //cout << name_a << endl;
        gain = infile.getNextFloatToken();
        h = infile.getNextFloatToken();
        azimut = infile.getNextIntToken();
        tilt_m = infile.getNextIntToken();
        tilt_e = infile.getNextIntToken();
        tilt_t = infile.getNextIntToken();
        perte_a = infile.getNextFloatToken();
        power = infile.getNextFloatToken();
        strcpy(dbm,infile.getNextToken());
        //cout << dbm << endl;
        //if(strcmp(name1, name)==0 ){
        if((thex1==thex)&&(they1==they)){
            // création d'un nouveau secteur
            no_sect ++;
            type_antenne = 1; // antenne directive
            if (patern==1) _porteurse = 0; //schema 1x3x1
            if (patern==2) _porteurse = no_sect; //schema 1x3x3
            secteur* the_sect = new secteur(no_sect_absolu,no_sect, lesSites[_no], type_antenne, perte_a, gain, power, Unite::degre2radian(azimut),
                                            tilt_m, tilt_e, _porteurse);
            lesSites[_no]->sect.push_back(the_sect);
            lesSites[_no]->nb_secteur++;
        }
       else {
            _no++;
            // création d'un nouveau site
            i_tp = (int)floor(0.5+((thex-pointTest::lesPT[0][0]->get_x())*1./pointTest::pdm));
            j_tp = (int)floor(0.5+((they-pointTest::lesPT[0][0]->get_y())*1./pointTest::pdm));
            /*if ((i_tp < 0)||(i_tp>pointTest::size_x)||(j_tp < 0)||(j_tp>pointTest::size_y)) {
                cout << endl << "hors departement : site no " << _no << " " <<  name_s << " " << thex << " " << they << endl;
                thez = s_sol + h;
            }*/
            thez = s_sol + h;
            lesSites[_no] = new site(_no, name_s, thex, they, thez);
            thex1 = thex;
            they1 = they;
            // création d'un nouveau secteur
            no_sect = 0;
            type_antenne = 1; // antenne directive
            if (patern==1) _porteurse = 0; //schema 1x3x1
            if (patern==2) _porteurse = no_sect; //schema 1x3x3
            secteur* the_sect = new secteur(no_sect_absolu,no_sect, lesSites[_no], type_antenne, perte_a, gain, power, Unite::degre2radian(azimut),
                                            tilt_m, tilt_e, _porteurse);
            lesSites[_no]->sect.push_back(the_sect);
            lesSites[_no]->nb_secteur = 1;
        }
		no_sect_absolu ++;
    }
    infile.close();
cout << "ssd" << endl;
}
/**Affectation aléatoire des frequeces initiales lors de la lecture du fichier site
*/

void site::load_sites_file_freqaleatoire(char* file_name)
{
   // insérer la procédure ici (Projet AG41)

       GInputFile infile(file_name) ;
    char* buf ;
    //char *tok ;
    infile.open() ;
    cout<<"\n>> Chargement des informations des sites et secteurs\n";
    buf=infile.readLine();
    size = infile.getNextIntToken();
    //int size_secteur = infile.getNextIntToken();
    infile.getNextIntToken();
    buf=infile.readLine();

    lesSites = new site*[size];
    int _no = -1, no_sect = 0, type_antenne, patern=2, _porteurse, no_sect_absolu=0;
    //char * name1 = "rien";
    int thex1 = -1, they1 = -1, s_sol = -1;
    int id, azimut, tilt_m, tilt_e, tilt_t, i_tp, j_tp;
    char name_s[80], name_a[100], dbm[10];
    double gain, power, thex, they, thez, h, perte_a;

    int* tableFreq = NULL;

    while ((buf=infile.readUncommentedLine())!=NULL) {
        id = infile.getNextIntToken();
        strcpy(name_s,infile.getNextToken());
        //cout << name_s << endl;
        thex = infile.getNextFloatToken();
        they = infile.getNextFloatToken();
        s_sol = infile.getNextFloatToken();
        strcpy(name_a,infile.getNextToken());
        //cout << name_a << endl;
        gain = infile.getNextFloatToken();
        h = infile.getNextFloatToken();
        azimut = infile.getNextIntToken();
        tilt_m = infile.getNextIntToken();
        tilt_e = infile.getNextIntToken();
        tilt_t = infile.getNextIntToken();
        perte_a = infile.getNextFloatToken();
        power = infile.getNextFloatToken();
        strcpy(dbm,infile.getNextToken());
        //cout << dbm << endl;
        //if(strcmp(name1, name)==0 ){
        if((thex1==thex)&&(they1==they)){
            // création d'un nouveau secteur
            no_sect ++;
            type_antenne = 1; // antenne directive
            if (patern==1) _porteurse = 0; //schema 1x3x1
            if (patern==2) _porteurse = tableFreq[no_sect]; //schema 1x3x3
            secteur* the_sect = new secteur(no_sect_absolu,no_sect, lesSites[_no], type_antenne, perte_a, gain, power, Unite::degre2radian(azimut),
                                            tilt_m, tilt_e, _porteurse);
            lesSites[_no]->sect.push_back(the_sect);
            lesSites[_no]->nb_secteur++;
        }
       else {
            _no++;
            // création d'un nouveau site
            i_tp = (int)floor(0.5+((thex-pointTest::lesPT[0][0]->get_x())*1./pointTest::pdm));
            j_tp = (int)floor(0.5+((they-pointTest::lesPT[0][0]->get_y())*1./pointTest::pdm));
            /*if ((i_tp < 0)||(i_tp>pointTest::size_x)||(j_tp < 0)||(j_tp>pointTest::size_y)) {
                cout << endl << "hors departement : site no " << _no << " " <<  name_s << " " << thex << " " << they << endl;
                thez = s_sol + h;
            }*/
            thez = s_sol + h;
            lesSites[_no] = new site(_no, name_s, thex, they, thez);
            thex1 = thex;
            they1 = they;
            // création d'un nouveau secteur
            if(tableFreq)delete tableFreq;
            tableFreq = randomizeTableFreq();
            cout<<endl<<endl;
            for(int i=0; i<3; i++)cout<<tableFreq[i]<<" ";
                        cout<<endl<<endl;
            no_sect = 0;
            type_antenne = 1; // antenne directive
            if (patern==1) _porteurse = 0; //schema 1x3x1
            if (patern==2) _porteurse = tableFreq[no_sect]; //schema 1x3x3
            secteur* the_sect = new secteur(no_sect_absolu,no_sect, lesSites[_no], type_antenne, perte_a, gain, power, Unite::degre2radian(azimut),
                                            tilt_m, tilt_e, _porteurse);
            lesSites[_no]->sect.push_back(the_sect);
            lesSites[_no]->nb_secteur = 1;
        }
		no_sect_absolu ++;
    }
    infile.close();
cout << "ssd" << endl;

}

int* site::randomizeTableFreq(){
    int* tableFreq = new int[3];
    int* table = new int[3]; for(int i=0; i<3;i++)table[i]=i+1;
    int* tableTmp = NULL;
    int taille = 3;
    long random;
    bool shift;
    for(int i=0; i<3; i++)
    {
        shift = false;
        random = Random::aleatoire(taille);
        tableFreq[i]=table[random];
        tableTmp = new int[taille-1];
        for(int j=0; j<taille; j++)
        {
            if(j != random){
                if(shift) tableTmp[j-1] = table[j];
                else tableTmp[j] = table[j];
            }
            else{shift=true;}
        }
        delete table;
        table = tableTmp;
        taille--;
    }
    delete tableTmp;
    return tableFreq;
}

/** Retourne le numéro du secteur (du site) le plus proche de '(lex,ley)'
	@param (lex,ley) coordonnées du point en mètre
	@return numéro du secteur
*/
int site::find_secteur(double lex, double ley) {
    int no = 0;
    double ecart;
    double t = MathB::angle(x, y, lex, ley);
    //t = MathB::radian_0_2PI(t-M_PI_2);
    t = MathB::radian_0_2PI(t);
    //t = atan2(ley-y,lex-x);
    double a = MathB::azimut2trigo_radian(sect[0]->get_azimut_radiant());
    double ecart_min = min(min(fabs(t-a),fabs(2*M_PI+t-a)),fabs(-2*M_PI+t-a));
    //cout << "0 : " << Unite::radian2degre(t) << " " << Unite::radian2degre(a)  << " " << Unite::radian2degre(ecart_min) << endl;

    for (int i=1; i<nb_secteur; i++) {
        a = MathB::azimut2trigo_radian(sect[i]->get_azimut_radiant());
        ecart = min(min(fabs(t-a),fabs(2*M_PI+t-a)),fabs(-2*M_PI+t-a));
        //cout << i << " : " << Unite::radian2degre(t) << " " << Unite::radian2degre(MathB::azimut2trigo_radian(sect[i]->get_azimut_radiant()))  << " " << Unite::radian2degre(ecart) << endl;
        if (ecart < ecart_min) {
            ecart_min = ecart;
            no = i;
        }
    }
    //cout << no << endl;
    //if (t<2.*M_PI/3) no=0;
    //else if (t<4.*M_PI/3) no=1;
    //else no=2;
    return no;
}
/** retourne les nb premiers sites du site (this)
@param (nb) le nombre des sites voisins
@return le tableau des sites voisins et les distances associées
*/
/*
void site::find_closest_sites(int nb) {
    nb_closest_sites = nb;
    closest_sites = new table_sites[nb];
    double _dist=0;//, dist_min = INFINITY;
    for (int i=0; i < site::size; i++) {
        if (((lesSites[i]->get_x())!= (this->get_x()))||((lesSites[i]->get_y())!=(this->get_y()))){
        _dist = MathB::dist(site::lesSites[i]->get_x(), site::lesSites[i]->get_y(), this->get_x(),  this->get_y());
        int j=nb;
        while ((j>0) && (_dist < closest_sites[j-1].dist_site_m)) j--;
        if (j<nb) {
            for (int k=nb-1; k>j; k--){
                closest_sites[k].dist_site_m = closest_sites[k-1].dist_site_m;
                closest_sites[k].no_site = closest_sites[k-1].no_site;
            }
            closest_sites[j].dist_site_m = _dist;
            closest_sites[j].no_site = i;
        }
        }
    }
   //for (int i=0; i<nb_closest_sites; i++) {
     //site::lesSites[closest_sites[i].no_site]->tabuList.push_back(closest_sites[i]);
	//}
}
/** rajoutes le site le plus proche au vecteur des sites voisins

void site::set_closest_sites() {
	for (int i=0; i<nb_closest_sites; i++) {
     site::lesSites[closest_sites[i]->no_site]->tabuList.push_back(closest_sites[i]);
	}
}
/*
/** Affiche les paramètres du site
*/
void site::display() {
    cout << "position = " << x << " "  << y << " "  << z << " -> " << si << " "  << sj << " "   << " : nb secteur = " << nb_secteur << endl;
}

void site::display_total() {
    cout << "site " << no << " : " << name << " -> position = " << x << " "  << y << " "  << z << " : nb secteur = " << nb_secteur << endl;
    vector<secteur*>::iterator it;
    for ( it=sect.begin() ; it < sect.end(); it++ )
		(*it)->display_simple();
}
