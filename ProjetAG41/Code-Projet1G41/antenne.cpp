#include "antenne.h"

using namespace std;

/// Constructeur vide
antenne::antenne() {}
/// Destructor
antenne::~antenne() {}

antenne::antenne(int _type_antenne, double g_dB, double ouvertureV_degre, double ouvertureH_degre) {
    type_antenne = _type_antenne;
    gain_dBi = g_dB;
    ouvertureMiPuissanceV_radian = Unite::degre2radian(ouvertureV_degre);
    ouvertureMiPuissanceH_radian = Unite::degre2radian(ouvertureH_degre);
}

antenne::antenne(int _type_antenne, double g_dB, double ouvertureV_degre) {
    type_antenne = _type_antenne;
    gain_dBi = g_dB;
    ouvertureMiPuissanceV_radian = Unite::degre2radian(ouvertureV_degre);
}

double antenne::get_diagH_radian2dB(double angle_radian) {
	switch (type_antenne) {
		case 0 : // antenne omnidirectionnelle
			return 0.;
			break;
		case 1 : // antenne directive
			//cout << "phi3= " << Unite::radian2degre(angle_radian)  << endl;
			//cout << "phi4= " << Unite::radian2degre(MathB::radian_PI_PI(angle_radian))  << endl;
			return min(12.*pow(MathB::radian_PI_PI(angle_radian),2)/pow(ouvertureMiPuissanceH_radian,2),Environement::get_Am_dB());
			break;
		default :
			return 0.;
    }
}

double antenne::get_diagV_radian2dB(double angle_radian) {
	double angle_max_radian = Unite::degre2radian(5.);
	angle_radian = min(angle_radian, angle_max_radian);
    return min(12.*pow(MathB::radian_PI_PI(angle_radian),2)/pow(ouvertureMiPuissanceV_radian,2),Environement::get_SLAv_dB());
}

void antenne::display() {
    cout << "gain_dB = " << gain_dBi << endl;
}

