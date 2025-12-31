#ifndef GRAPHE_H_INCLUDED
#define GRAPHE_H_INCLUDED

#include <iostream>
#include <sstream>
#include <string>
#include <utility>

#include "p_element.h"
#include "sommet.h"
#include "arete.h"

template<typename S, typename T>
class Graphe
{
public:
    PElement<Sommet<T>>* lSommets;
    PElement<Arete<S, T>>* lAretes;
    int prochaineClef;

    Graphe();
    Graphe(const Graphe<S, T>& graphe);
    ~Graphe();

    Sommet<T>* creeSommet(const T& info);
    Arete<S, T>* creeArete(const S& info, Sommet<T>* debut, Sommet<T>* fin);

    int nombreSommets() const;
    int nombreAretes() const;

    operator std::string() const;

    template<typename A, typename B>
    friend std::ostream& operator << (std::ostream& os, const Graphe<A, B>& graphe);

    PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* adjacences(const Sommet<T>* sommet) const;
    PElement<Arete<S, T>>* aretesAdjacentes(const Sommet<T>* sommet) const;
    PElement<Sommet<T>>* voisins(const Sommet<T>* sommet) const;

    Arete<S, T>* getAreteParSommets(const Sommet<T>* s1, const Sommet<T>* s2) const;

    // Ajouter un constructeur de copie
    const Graphe<S, T>& operator = (const Graphe<S, T>& graphe);
    
};

template<typename S, typename T>
Graphe<S, T>::Graphe(): lSommets(nullptr), lAretes(nullptr), prochaineClef(1) {}

template<typename S, typename T>
Graphe<S, T>::Graphe(const Graphe<S, T>& graphe)
    : lSommets(nullptr), lAretes(nullptr), prochaineClef(graphe.prochaineClef)
{
    // Copie des sommets
    lSommets = PElement<Sommet<T>>::copie2(graphe.lSommets);

    // Copie des arêtes
    lAretes = PElement<Arete<S, T>>::copie2(graphe.lAretes);
}

template<typename S, typename T>
const Graphe<S, T>& Graphe<S, T>::operator=(const Graphe<S, T>& graphe)
{
    if (this != &graphe)
    {
        // Nettoyage de l'existant
        PElement<Arete<S, T>>::efface2(lAretes);
        PElement<Sommet<T>>::efface2(lSommets);

        // Copie
        lSommets = PElement<Sommet<T>>::copie2(graphe.lSommets);
        lAretes  = PElement<Arete<S, T>>::copie2(graphe.lAretes);
        prochaineClef = graphe.prochaineClef;
    }
    return *this;
}

template<typename S, typename T>
Graphe<S, T>::~Graphe()
{
    PElement<Arete<S, T>>::efface2(lAretes);
    PElement<Sommet<T>>::efface2(lSommets);

    lAretes = nullptr;
    lSommets = nullptr;
}

template<typename S, typename T>
Sommet<T>* Graphe<S, T>::creeSommet(const T& info)
{
    Sommet<T> *nouveau = new Sommet<T>(prochaineClef++, info);
    lSommets = new PElement<Sommet<T>>(lSommets, nouveau);
    return nouveau;
}

template<typename S, typename T>
Arete<S, T>* Graphe<S, T>::creeArete(const S& info, Sommet<T>* debut, Sommet<T>* fin)
{
    Arete<S, T>* nouvelle = new Arete<S, T>(prochaineClef++, info, debut, fin);
    lAretes = new PElement<Arete<S, T>>(lAretes, nouvelle);
    return nouvelle;
}

template<typename S, typename T>
int Graphe<S, T>::nombreSommets() const
{
    return PElement<Sommet<T>>::taille(lSommets);
}

template<typename S, typename T>
int Graphe<S, T>::nombreAretes() const
{
    return PElement<Arete<S, T>>::taille(lAretes);
}

template<typename S, typename T>
Graphe<S, T>::operator std::string() const
{
    std::ostringstream oss;

    oss << "Graphe {\n";
    oss << "  Sommets : ";

    if (lSommets == nullptr)
        oss << "()";
    else
        oss << PElement<Sommet<T>>::toString(lSommets);

    oss << "\n  Aretes  : ";

    if (lAretes == nullptr)
        oss << "()";
    else
        oss << PElement<Arete<S, T>>::toString(lAretes);

    oss << "\n}";

    return oss.str();
}

template<typename A, typename B>
std::ostream& operator << (std::ostream& os, const Graphe<A, B>& graphe)
{
    return os << (std::string) graphe;
}

template<typename S, typename T>
PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* Graphe<S, T>::adjacences(const Sommet<T>* sommet) const
{
    // liste d'adjacences
    PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* resultat = nullptr;

    // liste des arêtes
    PElement<Arete<S, T>>* courant = lAretes;

    while(courant != nullptr)
    {
        Arete<S, T>* arete = courant->v;
        std::pair<Sommet<T>*, Arete<S, T>*>* selectionne = nullptr;

        if(sommet == arete->debut)
        {
            selectionne = new std::pair<Sommet<T>*, Arete<S, T>*>(arete->fin, arete);
        } 
        else if(sommet == arete->fin)
        {
            selectionne = new std::pair<Sommet<T>*, Arete<S, T>*>(arete->debut, arete);
        }

        if(selectionne != nullptr) resultat = new PElement<std::pair<Sommet<T>*, Arete<S, T>*>>(resultat, selectionne);

        courant = courant->s;
    }
    return resultat;
}

template<typename S, typename T>
PElement<Arete<S, T>>* Graphe<S, T>::aretesAdjacentes(const Sommet<T>* sommet) const
{
    // liste des arêtes finale
    PElement<Arete<S, T>>* resultat = nullptr;

    PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* courant = adjacences(sommet);

    while(courant != nullptr)
    {
        /**
         * Car la liste d'adjacence est formée ainsi:
         * 
         * template<class T1, class T2>
         * struct {
         *      T1 first;
         *      T2 second;
         * } pair;
        */
        Arete<S, T>* arete = courant->v->second;

        resultat = new PElement<Arete<S, T>>(resultat, arete);
        courant = courant->s;
    }
    return resultat;
}

template<typename S, typename T>
PElement<Sommet<T>>* Graphe<S, T>::voisins(const Sommet<T>* sommet) const
{
    PElement<Sommet<T>>* resultat = nullptr;

    PElement<std::pair<Sommet<T>*, Arete<S, T>*>>* courant = adjacences(sommet);

    while (courant != nullptr)
    {
        Sommet<T>* voisin = courant->v->first;
        resultat = new PElement<Sommet<T>>(resultat, voisin);
        courant = courant->s;
    }
    return resultat;
}

template<typename S, typename T>
Arete<S, T>* Graphe<S, T>::getAreteParSommets(const Sommet<T>* s1, const Sommet<T>* s2) const
{
    PElement<Arete<S, T>>* courant = lAretes;
    Arete<S, T>* arete = nullptr;

    while(courant != nullptr)
    {
        arete = courant->v;
        if((arete->debut == s1 && arete->fin == s2) || (arete->debut == s2 && arete->fin == s1))
        {
            return arete;
        }
        courant = courant->s;
    }
    return arete;
}
#endif // GRAPHE_H_INCLUDED
