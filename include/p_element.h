#ifndef P_ELEMENT_H_INCLUDED
#define P_ELEMENT_H_INCLUDED


#include <iostream>
#include <string>
#include <sstream>

template<typename T>
class PElement
{
public:
    PElement* s;
    T* v;

    PElement(PElement<T>* suivant, T* valeur);
    static int taille(const PElement<T>* liste);
    static const std::string toString(const PElement<T>* liste,
                                 const char* debut = "(",
                                 const char* separateur = ", ",
                                 const char* fin = ")");
    static void insertionOrdonnee(T* a,
                                  PElement<T>* &liste,
                                  bool (*estPlusPetitOuEgal)(const T* a1, const T* a2));
    static T* depiler(PElement<T>* &liste);
    static bool retire(const T* a, PElement<T>* &liste);
    static void efface1(PElement<T>* &liste);
    static void efface2(PElement<T>* &liste);
    static PElement<T>* copie1(PElement<T>* liste);
    static PElement<T>* copie2(PElement<T>* liste);
    static PElement<T>* copie3(PElement<T>* liste);
    static PElement<T>* appartient(const T* a, PElement<T>* liste);

    template<typename FONCTEUR>
    static PElement<T>* appartient(PElement<T>* l, const FONCTEUR &condition);
    // Avec:  bool condition(const T* a);
};

template<typename T>
PElement<T>::PElement(PElement<T>* suivant, T* valeur): s(suivant), v(valeur) {}

template<typename T>
int PElement<T>::taille(const PElement<T>* liste)
{
    if (liste == nullptr) return 0;

    const PElement<T>* courant = liste;
    int compteur = 0;

    while(courant != nullptr)
    {
        compteur ++;
        courant = courant->s;
    }
    return compteur;
}

template<typename T>
const std::string PElement<T>::toString(const PElement<T>* liste, const char* debut,
                                   const char* separateur, const char* fin)
{
    std::ostringstream os;
    os << debut;
    if (liste == nullptr)
    {
        os << fin;
        return os.str();
    }

    const PElement<T>* courant = liste;
    int nbElements = PElement::taille(liste);

    for(int i = 0; i < nbElements; i++)
    {
        T* valeur = courant->v;
        os << *valeur;
        courant = courant->s;
        if (i < nbElements - 1) os << separateur;
    }
    os << fin;
    return os.str();
}

template<typename T>
std::ostream& operator <<(std::ostream& os, const PElement<T>* liste)
{
    os << liste->toString(liste);
    return os;
}

template<typename T>
bool estPlusPetitOuEgal(T* a1, T* a2)
{
    return *a1 < *a2;
}

template<>
bool estPlusPetitOuEgal(const double* a1, const double* a2)
{
    return *a1 < *a2;
}

template<>
bool estPlusPetitOuEgal(const std::string* a1, const std::string* a2)
{
    std::string valeur_a1 = *a1;
    std::string valeur_a2 = *a2;
    return valeur_a1.compare(valeur_a2);
}

template<typename T>
void PElement<T>::insertionOrdonnee(T* a, PElement<T>* &liste, bool (*estPlusPetitOuEgal)(const T* a1, const T* a2))
{
    // Liste vide
    if (liste == nullptr)
    {
        liste = new PElement<T>(nullptr, a);
        return;
    }

    // Insertion en t�te
    if (estPlusPetitOuEgal(a, liste->v))
    {
        liste = new PElement<T>(liste, a);
        return;
    }

    PElement<T>* courant = liste;

    while(courant->s != nullptr && !estPlusPetitOuEgal(a, courant->s->v))
    {
        courant = courant->s;
    }
    courant->s = new PElement<T>(courant->s, a);
}

template<typename T>
T* PElement<T>::depiler(PElement<T>* &liste)
{
    T* element = nullptr;
    if(liste == nullptr) throw "Liste vide";

    element = liste->v;
    liste = liste->s;

    return element;
}

template<typename T>
bool PElement<T>::retire(const T* a, PElement<T>* &liste)
{
    if(liste == nullptr) return false;

    // Suppr�ssion en t�te
    if(liste->v == a)
    {
        liste = liste->s;
        return true;
    }

    PElement<T>* courant = liste;

    while(courant->s != nullptr)
    {
        if(courant->s->v == a)
        {
            courant->s = courant->s->s;
            return true;
        }
        courant = courant->s;
    }
    return false;
}

template<typename T>
void PElement<T>::efface1(PElement<T>* &liste)
{
    while(liste != nullptr)
    {
        PElement<T>* courant = liste;
        liste = liste->s;
        delete courant;
    }
}

template<typename T>
void PElement<T>::efface2(PElement<T>* &liste)
{
    while(liste != nullptr)
    {
        PElement<T>* courant = liste;
        liste = liste->s;
        delete courant->v;
        delete courant;
    }
}

template<typename T>
PElement<T>* PElement<T>::copie1(PElement<T>* liste)
{
    if (liste == nullptr)
        return nullptr;

    // Cr�ation de la t�te de la nouvelle liste
    PElement<T>* nouvelleListe = new PElement<T>(nullptr, liste->v);

    PElement<T>* courantSource = liste->s;
    PElement<T>* courantCopie  = nouvelleListe;

    // Copie du reste de la liste
    while (courantSource != nullptr)
    {
        courantCopie->s = new PElement<T>(nullptr, courantSource->v);

        courantCopie = courantCopie->s;
        courantSource = courantSource->s;
    }
    return nouvelleListe;
}


template<typename T>
PElement<T>* PElement<T>::copie2(PElement<T>* liste)
{
    if(liste == nullptr) return liste;
    /*
    T* p_valeur = liste->v;
    T valeur = *p_val;

    PElement<T>* nouvelleListe = new PElement<T>(nullptr, &val);
    */
    // Cr�ation de la nouvelle t�te de liste
    PElement<T>* nouvelleListe = new PElement<T>(nullptr, new T(*(liste->v)));

    PElement<T>* courantSource = liste->s;
    PElement<T>* courantCopie = nouvelleListe;

    while(courantSource != nullptr)
    {
        /*
        T *p_courantSourceValeur = courantSource->v;
        T courantSourceValeur = *p_courantSourceValeur;

        courantCopie->s = new PElement<T>(nullptr, courantSourceValeur);
        */
        courantCopie->s = new PElement<T>(nullptr, new T(*(courantSource->v)));

        courantSource = courantSource->s;
        courantCopie = courantCopie->s;
    }
    return nouvelleListe;
}

template<typename T>
PElement<T>* PElement<T>::copie3(PElement<T>* liste)
{
    if (liste == nullptr)
        return nullptr;

    // Cr�ation de la t�te de la nouvelle liste
    PElement<T>* nouvelleListe = new PElement<T>(nullptr, liste->v->copie());

    PElement<T>* courantSource = liste->s;
    PElement<T>* courantCopie  = nouvelleListe;

    while (courantSource != nullptr)
    {
        courantCopie->s = new PElement<T>(nullptr, courantSource->v->copie());

        courantCopie = courantCopie->s;
        courantSource = courantSource->s;
    }

    return nouvelleListe;
}

template<typename T>
PElement<T>* PElement<T>::appartient(const T* a, PElement<T>* liste)
{
    if(liste == nullptr) return nullptr;

    while(liste != nullptr)
    {
        if(liste->v == a) return liste;
        liste = liste->s;
    }
    return nullptr;
}

template<typename T>
template<typename FONCTEUR>
// Avec:  bool condition(const T* a);
PElement<T>* PElement<T>::appartient(PElement<T>* liste, const FONCTEUR &condition)
{
    if(liste == nullptr) return nullptr;

    while(liste != nullptr)
    {
        if(condition(liste->v)) return liste;
        liste = liste->s;
    }
    return nullptr;
}

#endif // P_ELEMENT_H_INCLUDED
