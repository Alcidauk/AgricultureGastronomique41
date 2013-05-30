#ifndef _MATH_BASE_H
#define _MATH_BASE_H

#include <stdlib.h>
#include <math.h>


class MathB
{
    public:
    static double dist(int x1, int y1, int x2, int y2) {
        return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    };

    static double dist(int x1, int y1, int z1, int x2, int y2, int z2) {
        return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
    };

    static double dist(double x1, double y1, double x2, double y2) {
        return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2));
    };

    static double dist(double x1, double y1, double z1, double x2, double y2, double z2) {
        return sqrt((x1-x2)*(x1-x2)+(y1-y2)*(y1-y2)+(z1-z2)*(z1-z2));
    };

    static void cartesien2polar(double x, double y, double &r, double &theta) {  // dans [0,+2pi[
        r = sqrt(x*x+y*y);
        if (r!=0) theta = radian_0_2PI(atan2(y,x)); // car atan2 est dans [-pi,+pi]
        else theta = 0;
    };

    static void polar2cartesien(double r, double theta, double &x, double &y) {
        x = r*cos(theta);
        y = r*sin(theta);
    };

	/** Détermine l'angle (Ox, u) (en radian compris entre [0,+2pi[) u étant le vecteur allant de (x1,y1) à (x2,y2)
		@param (x1,y1) coordonnée de départ du vecteur u
		@param (x2,y2) coordonnée d'arrivé du vecteur u
	*/
    static double angle(double x1, double y1, double x2, double y2) { // dans [0,+2pi[
        if ((y1==y2)&&(x1==x2)) return 0;
        else return radian_0_2PI(atan2(y2-y1, x2-x1)); // car atan2 est dans [-pi,+pi]
    };

    static double radian_0_2PI(double a) {
        if (a<0) return radian_0_2PI(a+2*M_PI);
        if (a>=2*M_PI) return radian_0_2PI(a-2*M_PI);
        return a;
    };

    static double radian_PI_PI(double a) {
        if (a<-M_PI) return radian_PI_PI(a+2*M_PI);
        if (a>=M_PI) return radian_PI_PI(a-2*M_PI);
        return a;
    };

	/** Transforme l'angle azimut en aangle trigonométrique standard \n
		l'azimut à pour origine le Nord c-à-d l'axe (Oy) et tourne dans le sens horaire (anti trigronométrique) \n
		tandis que l'angle trigonométrique a pour origine l'axe (Ox) et tourne dans le sens trigronométrique
		@param angle azimut en radian
		@return angle trigonométrique en radian dans [0, 2PI[
	*/
    static double azimut2trigo_radian(double a) {
        return radian_0_2PI(5.*M_PI/2. - a);
    };
    static double trigo2azimut_radian(double a) {
        return radian_0_2PI(5.*M_PI/2. - a);
    };

    static double tilt2trigo_radian(double a) {
        return radian_0_2PI(-a);
    };

    static int find_dichotomie(int valeur, int* tab, int size_tab) {
        return find_dichotomie(valeur, tab, 0, size_tab);};

    static int find_dichotomie(int valeur, int* tab, int i_min, int i_max) {
        if (i_max - i_min <=1) return i_min;
        else{
            int i_milieu =  i_min + (i_max - i_min)/2;
            if (valeur <= tab[i_milieu])
                return find_dichotomie(valeur, tab, i_min, i_milieu);
            else
                return find_dichotomie(valeur, tab, i_milieu, i_max);
        }
    };

    static int fact_rec(int n, int acc) {
    	if (n==1) return acc;
    	else return fact_rec(n-1, n*acc);
    };
    static int fact(int n) {return fact_rec(n,1.);};

};

# endif
