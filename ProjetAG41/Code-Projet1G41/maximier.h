#include <stdlib.h>
#include <stdio.h>
#include <assert.h>

struct h {
    int elt;
    struct h *g, *d; /* fils gauche et droit */
};
typedef struct h* maximier;

#define vide (maximier)0
maximier alloue(int elt, maximier g, maximier d) {
    maximier m = (maximier)malloc(sizeof(struct h));
    m->elt = elt; m->g = g; m->d = d; return m;
}

void insertion(int comp(int, int), int x, maximier * m) {
    if (*m) { /* Si le maximier n’est pas vide */
        int r = (*m)->elt;
        maximier g = (*m)->g; /* On  ́change les 2 fils */
        (*m)->g = (*m)->d;
        (*m)->d = g;
        if (comp(x, r)) { /* nouvel  ́l ́ment plus petit que la racine */
            (*m)->elt = x; /* on le place ` la racine */
            insertion(comp, r, &((*m)->g)); /*on ins`re la racine ` gauche */
        } else {           /* sinon on ins`re le nouvel  ́l ́ment ` gauche */
            insertion(comp, x, &((*m)->g));
        }
    } else { /* Maximier vide, on en alloue un nouveau */
        *m = alloue(x, vide, vide);
    }
}

void ote_sommet(int comp(int, int), maximier * m) {
    if (*m) { /* si le maximier n’est pas vide (sinon rien) */
        maximier x = *m;
        if (! (x->g)) { /* si le fils gauche est vide */
            (*m) = x->d; /* on remonte le fils droit */
            free(x);         /* et on lib`re l’ex racine */
            return;
        }
        if (! (x->d)) { /* si le fils droit est vide: sym ́triquement */
            (*m) = x->g; free(x);
            return;
        }
        else { /* Sinon on remonte la plus petite racine des 2 */
            int eltg = (x->g)->elt;
            int eltd = (x->d)->elt;
            if (comp(eltg,eltd)) {
                x->elt = eltg;
                ote_sommet(comp, &(x->g));
            } else {
                x->elt = eltd;
                ote_sommet(comp, &(x->d));
            }
        }
    }
}

