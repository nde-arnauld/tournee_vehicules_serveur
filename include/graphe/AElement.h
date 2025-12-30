#ifndef GRAPHE_AELEMENT_H
#define GRAPHE_AELEMENT_H
#include <ostream>
#include <sstream>
#include <string>

using namespace std;

class AElement {
public:
    int clef;
    AElement(const int clef) : clef(clef) {};

    operator string () const {
        ostringstream os;
        os << "AElement{clef=" << clef << "}";
        return os.str();
    }
};

inline ostream & operator<<(ostream &os, const AElement &obj) {
    return os << string(obj);
}

#endif //GRAPHE_AELEMENT_H