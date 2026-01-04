#ifndef SOMMET_H_INCLUDED
#define SOMMET_H_INCLUDED

#include <iostream>
#include <sstream>
#include "g_element.h"

template<typename T>
class Sommet : public GElement<T>
{
public:
    int degre;

    // Constructeurs
    Sommet(const int id, T t);
    Sommet(const int id, T t, int deg);

    // Op�ratareur de conversion en string
    operator std::string() const;

    // Op�rateur d'�criture sur un flux
    template<typename U>
    friend std::ostream& operator <<(std::ostream& os, const Sommet<U>& sommet);
};

// Constructeurs
template<typename T>
Sommet<T>::Sommet(const int id, T t): GElement<T>(id, t), degre(0) {}

template<typename T>
Sommet<T>::Sommet(const int id, T t, int deg): GElement<T>(id, t), degre(deg) {}

// Op�ratareur de conversion en string
template <typename T>
Sommet<T>::operator std::string() const
{
    std::ostringstream oss;
    oss << std::string(GElement<T>::operator std::string()) << ", degre: " << degre;
    return oss.str();
}

// Op�rateur d'�criture sur un flux
template<typename U>
std::ostream& operator <<(std::ostream& os, const Sommet<U>& sommet)
{
    return os << (std::string) sommet;
}

#endif // SOMMET_H_INCLUDED
