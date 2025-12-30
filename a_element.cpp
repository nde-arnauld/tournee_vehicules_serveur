#include "a_element.h"
#include <iostream>
#include <sstream>

using namespace std;

AElement::AElement(const int id): clef(id) { }

AElement::operator string() const
{
    ostringstream os;
    os << "clef: " << clef;
    return os.str();
}

ostream& operator <<(ostream& o, const AElement& element)
{
    return o << (string) element;
}
