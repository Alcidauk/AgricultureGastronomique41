#ifndef _PROPAGATION_H
#define _PROPAGATION_H

#include <stdlib.h>
#include "secteur.h"
#include "pointTest.h"
#include "math_base.h"

using namespace std;

class pointTest;

class Propagation
{
    public:
    /**
        @return l'affaiblissament en dB
    */
    static double pathLossHataMediumCity(pointTest *p, secteur* s, int f_MHz) {
        //double dist_p_s_metre = MathB::dist(p->get_x(), p->get_y(), s->get_x(), s->get_y());
        double dist_p_s_metre = MathB::dist(p->get_x(), p->get_y(), p->get_z(), s->get_x(), s->get_y(), s->get_z());
        double dist_p_s_km = dist_p_s_metre/1000.;
        //int zp_m = p->get_z();
        //int zs_m = s->get_z();
        int zp_m = Environement::get_height_mobile_station_default_m();
        int zs_m = min(100., max(1., fabs(s->get_z()-p->get_z())));
        //if( zs_m < 0 ) return (-Environement::get_puissanceNulle_dBm());
        if( (zs_m==0) || (zp_m==0) || (f_MHz==0)) {
            cout << "------------- Arret du programme ----------------" << endl;
            cout << "------------- Arret du programme ----------------" << endl;
            cout << "Probleme fatal dans le modele de propagation HATA" << endl;
            cout << "------------- Arret du programme ----------------" << endl;
            cout << "------------- Arret du programme ----------------" << endl;
            cout << "Affichage du point de test" << endl;
            p->display_detail2();
            cout << "Affichage du secteur" << endl;
            s->display_simple();
            cout << "------------- Arret du programme ----------------" << endl;
            cout << "------------- Arret du programme ----------------" << endl;
            cout << "Probleme fatal dans le modele de propagation HATA" << endl;
            cout << "------------- Arret du programme ----------------" << endl;
            cout << "------------- Arret du programme ----------------" << endl;
            exit(0);
        }

        if (dist_p_s_km==0)
            return 0;
        else {
            double az = 0.8 + (1.1*log10(f_MHz)-0.7)*zp_m - 1.56*log10(f_MHz);
            double PL_dB = 69.55 + 26.16*log10(f_MHz) - 13.82*log10(zs_m) - az + (44.9-6.55*log10(zs_m))*log10(dist_p_s_km);
/*
    cout << dist_p_s_metre << endl;
    cout << dist_p_s_km << endl;
    cout << zp_m << endl;
    cout << zs_m << endl;
    cout << az << endl;
    cout << 1.56*log10(f_MHz) << endl;
    cout << (1.1*log10(f_MHz)-0.7)*zp_m << endl;
    cout << "69.55" << endl;
    cout << 26.16*log10(f_MHz) << endl;
    cout << 13.82*log10(zs_m) << endl;
    cout << (44.9-6.55*log10(zs_m))*log10(dist_p_s_km) << endl;
    cout << PL_dB << endl;
    exit(0);
    */
            return PL_dB;
        }
    };

};

# endif
