#ifndef __ENTREESORTIE_h__
#define __ENTREESORTIE_h__

#include "gfile.h"
class EntreeSortie {
    public :
    GOutputFile fsortie;
    void SetFSortie(char * fnom) {fsortie.setFileName(fnom);}
};
#endif
