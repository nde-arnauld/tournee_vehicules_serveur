#ifndef AELEMENT_H_INCLUDED
#define AELEMENT_H_INCLUDED

#include <sstream>

class AElement {
public:
    int clef;

    AElement(const int id);
    operator std::string() const;
    friend std::ostream& operator <<(std::ostream& o, const AElement& element);
};

#endif // AELEMENT_H_INCLUDED
