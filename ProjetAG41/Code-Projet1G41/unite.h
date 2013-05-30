#ifndef _UNITE_H
#define _UNITE_H

#include <math.h>

class Unite
{
    public:
    static double radian2degre(double a) {
        return (a*180./M_PI);
    };

    static double degre2radian(double a) {
        return (a*M_PI/180.);
    };

    static double mWatt2dBm(double p_mW) {
        return (10.*log10(p_mW));
    };

    static double watt2dBW(double p_W) {
        return (10.*log10(p_W));
    };

    static double watt2dBm(double p_W) {
        return (10.*log10(1000.*p_W));
    };

    static double sansUnite2dB(double p_sansUnite) {
        return (10.*log10(p_sansUnite));
    };

    static double dBW2watt(double p_dBW) {
        return (pow(10,(p_dBW/10.)));
    };

    static double dBm2mWatt(double p_dBm) {
        return (pow(10,(p_dBm/10.)));
    };

    static double mega(double a) {return (a*1000000.);};
    static double kilo(double a) {return (a*1000.);};

};

# endif
