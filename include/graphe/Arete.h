#ifndef GRAPHE_ARETE_H
#define GRAPHE_ARETE_H

#include "Sommet.h"

template <class S, class T>
class Arete : public GElement<S> {
public:
    Sommet<T>* debut;
    Sommet<T>* fin;
    /**
     * met à jour le degré des sommets que cette nouvelle arête va connecter
     */
    Arete(const int clef, const S & v, Sommet<T> * debut, Sommet<T> * fin)
        : GElement<S>(clef, v), debut(debut), fin(fin) {};

    ~Arete() {
        --debut->degre;
        --fin->degre;
    };

    operator string() const {
        ostringstream os;
        os << "Arete{clef=" << this->clef << ", v=" << this->v
           << ", debut=" << debut->clef << ", fin=" << fin->clef << "}";
        return os.str();
    }

    /**
     * vérifie si *this s'appuie sur s1 et s2
     * DONNEES : *this, s1, s2
     * RESULTATS : true si *this s'appuie sur s1 et s2 c'est-à-dire si (début == s1 et fin == s2) ou (début == s2 et fin == s1), false sinon
     */
    bool estEgal(const Sommet<T> * s1, const Sommet<T> * s2) const {
        return (debut == s1 && fin == s2) || (debut == s2 && fin == s1);
    }
};

template <class S, class T>
ostream & operator<<(ostream &os, const Arete<S, T> &obj) {
    return os << string(obj);
}

#endif //GRAPHE_ARETE_H
