#ifndef SOLFREQUENCE_H
#define SOLFREQUENCE_H

using namespace std;

typedef struct _FreqStruct{
    _FreqStruct(): porteuz1(-1),porteuz2(-1),porteuz3(-1){}
    int porteuz1;
    int porteuz2;
    int porteuz3;
    }FreqStruct;

class SolFrequence{

private:
int nbrSites;
FreqStruct *solution;

public:
//Constructeur
SolFrequence(int nbSite){
    nbrSites = nbSite;
    solution = new FreqStruct[nbrSites];
}
//Destructeur
~SolFrequence();
FreqStruct* get_solution(){return solution;}
// Afficher la solution
void DisplaySolution(){
for (int i=0; i<nbrSites; i++){
cout << "la porteuse 1 : " << solution[i].porteuz1 << endl;
cout << "la porteuse 2 : " << solution[i].porteuz1 << endl;
cout << "la porteuse 3 : " << solution[i].porteuz1 << endl;
}
}

};
#endif
