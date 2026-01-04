#ifndef ARETE_H_INCLUDED
#define ARETE_H_INCLUDED

#include <iostream>
#include <sstream>
#include <memory>
#include "g_element.h"
#include "sommet.h"

template <typename S, typename T>
class Arete : public GElement<S>
{
public:
    Sommet<T>* debut;
    Sommet<T>* fin;

    // Met � jour le degr� des sommets que cette nouvelle ar�te va connecter
    Arete(const int clef, const S& v, Sommet<T>* debut, Sommet<T>* fin);
    ~Arete();

    operator std::string() const;

    template<typename A, typename B>
    friend std::ostream& operator <<(std::ostream& os, const Arete<A, B>& arete);

    bool estEgal(const Sommet<T>* s1, const Sommet<T>* s2) const;
};

template <typename S, typename T>
Arete<S, T>::Arete(const int clef, const S& v, Sommet<T>* debut, Sommet<T>* fin):
    GElement<S>(clef, v), debut(debut), fin(fin)
{
    debut->degre ++;
    fin->degre ++;
}

template <typename S, typename T>
Arete<S, T>::~Arete()
{
    debut->degre --;
    fin->degre --;
}

template <typename U>
void print(std::ostream& os, const U& value) { os << value; }

template <typename U>
void print(std::ostream& os, const std::shared_ptr<U>& ptr)
{
    if (ptr)
        os << *ptr;
    else
        os << "null";
}

template <typename S, typename T>
Arete<S, T>::operator std::string() const
{
    std::ostringstream oss;
    oss << debut->v << ")-- ";
    print(oss, this->v);
    oss <<" --(" << fin->v;
    return oss.str();
}

template<typename A, typename B>
std::ostream& operator <<(std::ostream& os, const Arete<A, B>& arete)
{
    return os << (std::string) arete;
}

template <typename S, typename T>
bool Arete<S, T>::estEgal(const Sommet<T>* s1, const Sommet<T>* s2) const
{
    return (debut == s1 && fin == s2) || (debut == s2 && fin == s1);
}
#endif // ARETE_H_INCLUDED
