#ifndef GRAPHE_GELEMENT_H
#define GRAPHE_GELEMENT_H

#include "AElement.h"

template <class T>
class GElement : public AElement {
public:
    T v;
    GElement(const int clef, const T &v) : AElement(clef), v(v) {};

    operator string () const {
        ostringstream os;
        os << "GElement{clef=" << clef << ", v=" << v << "}";
        return os.str();
    }
};

template <class T>
ostream & operator<<(ostream &os, const GElement<T> &obj) {
    return os << string(obj);
}

#endif //GRAPHE_GELEMENT_H