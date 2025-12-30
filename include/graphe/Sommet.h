#ifndef GRAPHE_SOMMET_H
#define GRAPHE_SOMMET_H

#include "GElement.h"

template <class T>
class Sommet : public GElement<T> {
public:
    int degre;
    Sommet(const int clef, const T &v) : GElement<T>(clef, v), degre(0) {}

    operator string () const {
        ostringstream os;
        os << "Sommet{clef=" << this->clef << ", v=" << this->v << ", degre=" << degre << "}";
        return os.str();
    }
};

template <class T>
ostream & operator<<(ostream &os, const Sommet<T> &obj) {
    return os << string(obj);
}

#endif //GRAPHE_SOMMET_H
