#ifndef _RANDOM_H
#define _RANDOM_H

#include <stdlib.h>
#include <time.h>

class Random
{
 public:
  /** Initialize the random variable generator */
  static void randomize()
    {
      srand ( time(NULL) );
    };

  /** return a random interger between 0 and (borne-1) */
  /*static long aleatoire(long borne)
    {
      return(rand()%borne);
    };
*/
  /** return a random interger between 0 and (borne-1) */
  static long aleatoire(long borne)
    {
        if ((borne>0) && (borne<RAND_MAX))
            return(rand()%borne);
        else {
            int exposant = 0;
            while (borne/pow(10.,exposant*1.) > 1)
                exposant++;
            exposant = exposant+2;
            double sortie = 0;
            for (int i=0; i<exposant; i++)
                sortie += pow(10.,i*1.)*(rand()%10);
            return(((int)floor(sortie))%borne);
        }
    };

  /** return a random value between 0 and 1 */
  static double random01()
    {
      return(rand()*1./RAND_MAX);
    };

  /** tirer aléatoirement uniformément sur l'intervalle [-coeff, coeff] */
  static double random(double coeff)
    {
      return ((1.-rand()*2./RAND_MAX)*coeff);
    };
 static int randDureeTabu(int min, int max)
    {
       return (min + (rand() % (max - min)));
    };
};

# endif
