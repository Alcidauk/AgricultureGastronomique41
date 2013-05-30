#ifndef _ENVIRONEMENT_H
#define _ENVIRONEMENT_H
#include "gfile.h"

//GOutputFile fsortie;
//void setFSortie(char *fnom) {fsortie.setFileName(fnom);}

class Environement
{
    private:
        static double densiteBruitBlanc;
        static const double Am_dB = 25;
        static const double SLAv_dB = 20;
        static const double constanteBoltzmann = 1.3806504e-23;
        static const double temperature_K = 290.;
        static const double puissanceNulle_dBm = -200.;
        static const double puissanceNegligeable_dBm = -160.;

        //static const double debitNecessaireVoix_kbps = 2000.;
        static const double debitNecessaireData_kbps = 2000.;
       // static const double proportionVoixData = 3/4;

        //static const double sur_sol_default_m = 400;
        static const int height_mobile_station_default_m = 1;
        //static const double largeurProteuseClient_MHz = 0.40;

        /// bande de fréquence d'émisson des eNB
        static const int freqE_MHz = 1800;
        static const double largeurBande_kHz = 20000;

    public:
        Environement(){}

        static int nbProteuses;
        static int get_height_mobile_station_default_m(){return height_mobile_station_default_m;};
        //static double get_sur_sol_default_m(){return sur_sol_default_m;};
        static double get_puissanceNegligeable_dBm(){return puissanceNegligeable_dBm;};
        static double get_puissanceNulle_dBm(){return puissanceNulle_dBm;};
        static double get_densiteBruitBlanc(){return constanteBoltzmann*temperature_K;};
//        static double get_debitNecessaireVoix_kbps(){ return debitNecessaireVoix_kbps;};
        static double get_debitNecessaireData_kbps(){ return debitNecessaireData_kbps;};
       // static double get_proportionVoixData(){ return proportionVoixData;};
        static double get_largeurBandeClient_kHz(){return debitNecessaireData_kbps/efficacite_spectrale(1.);};
        static double get_largeurBande_kHz(){return largeurBande_kHz;};
        static int get_freqE_MHz(){return freqE_MHz;};
		static double get_Am_dB(){return Am_dB;};
		static double get_SLAv_dB(){return SLAv_dB;};



        static double efficacite_spectrale(double sinr_dB) {
            if (sinr_dB < 0.9)  return 0.;
            if (sinr_dB < 2.1)  return 0.75;
            if (sinr_dB < 3.8)  return 1.;
            if (sinr_dB < 7.7)  return 1.25;
            if (sinr_dB < 9.8)  return 2.;
            if (sinr_dB < 12.6) return 2.75;
            if (sinr_dB < 15)   return 3.25;
            if (sinr_dB < 18.2) return 4.;
            return 5.;
        };

		static const double largeurBlock_kHz = 180.;
        static int nb_block_utile(double demande_kbps, double efficacite_kbpspkHz) {
        	return (int)ceil(demande_kbps/largeurBlock_kHz/efficacite_kbpspkHz);
        };

        static const int nb_neighbor_sites = 5;
		static const double radius_of_influence_m = 1000.0;

		static const int pattern_reutilisation = 2;
		static void set_nbPrteuse() {
			switch (pattern_reutilisation) {
				case 0 : //schema 1x1x1
					Environement::nbProteuses = 1;
					break;
				case 1 : //schema 1x3x1
					Environement::nbProteuses = 1;
					break;
				case 2 : //schema 1x3x3
					Environement::nbProteuses = 3;
					break;
			}
		};
		/////////



};

# endif
