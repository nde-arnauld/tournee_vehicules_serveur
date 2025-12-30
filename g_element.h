#ifndef GELEMENT_H_INCLUDED
#define GELEMENT_H_INCLUDED

#include "a_element.h"
#include <iostream>
#include <string>
#include <sstream>

template <typename T>
class GElement : public AElement
{
public:
    T v;
    GElement(const int id);
    GElement(const int id, T t);

    operator std::string() const;

    template<typename U>
    friend std::ostream& operator <<(std::ostream& os, const GElement<U>& g_element);
};

template <typename T>
GElement<T>::GElement(const int id): AElement(id), v(0) {}

template <typename T>
GElement<T>::GElement(const int id, T t): AElement(id), v(t) {}

template <typename T>
GElement<T>::operator std::string() const
{
    std::ostringstream oss;
    oss << std::string(AElement::operator std::string()) << ", v: " << v;
    return oss.str();
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const GElement<T>& g_element)
{
    os << (std::string) g_element;
    return os;
}

#endif // GELEMENT_H_INCLUDED
