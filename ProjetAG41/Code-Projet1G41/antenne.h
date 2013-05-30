#ifndef ANTENNE_H
#define ANTENNE_H

#include <iostream>
#include "unite.h"
#include "math_base.h"
#include "environement.h"

class antenne
{
    private:
		int type_antenne;
        double gain_dBi;
        double ouvertureMiPuissanceV_radian;
        double ouvertureMiPuissanceH_radian;
        int nbTiltElectrique;
        int nbAnglesDiagrammeV;
        double **diagrammeV_degre_degre_dB;
        int nbAnglesDiagrammeH;
        double *diagrammeH_degre_dB;
        int nbPuissances;
        double* listePuissances_dBm;

    public:
        antenne();
        ~antenne();
        antenne(int type_antenne, double g_dB, double ouvertureV_degre);
        antenne(int type_antenne, double g_dB, double ouvertureV_degre, double ouvertureH_degre);

        double get_gain_dBi(){return gain_dBi;};
        double get_diagV_radian2dB(double angle_radian);
        double get_diagH_radian2dB(double angle_radian);

        //double get_ouvertureMiPuissanceV_degre(){return ouvertureMiPuissanceV_degre;};
        //double get_ouvertureMiPuissanceH_degre(){return ouvertureMiPuissanceH_degre;};

        void display();

};

#endif

