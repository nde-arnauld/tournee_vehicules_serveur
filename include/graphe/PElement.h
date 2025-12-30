#ifndef TEMPLATE_DE_CLASSE_PELEMENT_H
#define TEMPLATE_DE_CLASSE_PELEMENT_H

#include <ostream>
#include <sstream>
#include "../utils/Erreur.h"

using namespace std;

template <class T>
class PElement {
public:
    PElement<T>* s; // Le suivant
    T* v; // La valeur
    PElement(T *v, PElement<T> *s = nullptr)
            : v(v), s(s) {
    }

    static int taille(const PElement<T>* l) {
        if (l == nullptr) return 0;
        return 1 + taille(l->s);
    }

    static string toString(const PElement<T>* p,
                              const char* debut = "(",
                              const char* separateur = ", ",
                              const char* fin = ")"){
        ostringstream oss;
        oss << debut;

        const PElement<T>* current = p;
        while (current != nullptr) {
            if (current->s == nullptr) {
                oss << *current->v << fin;
            } else {
                oss << *current->v << separateur;
            }
            current = current->s;
        }

        return oss.str();
    }

    operator string() const {
        return toString(this);
    }

    // Méthode pour insertion ordonnée (récursive)
    static void insertionOrdonnee(T* a, PElement<T>*& l,
                                bool (*estPlusPetitOuEgal)(const T* a1, const T* a2)) {
        if (l == nullptr || estPlusPetitOuEgal(a, l->v)) {
            l = new PElement<T>(a, l);
        } else {
            insertionOrdonnee(a, l->s, estPlusPetitOuEgal);
        }
    }

    // Méthode pour dépiler la liste
    static T* depiler(PElement<T>*& l) {
        if (l == nullptr) {
            throw Erreur();
        }
        PElement<T>* oldHead = l;
        l = l->s;
        T* valeur = oldHead->v;
        delete oldHead;
        return valeur;
    }

    // Méthode pour retirer un élément
    static bool retire(const T* a, PElement<T>*& l) {
        if (l == nullptr) return false;

        if (l->v == a) {
            PElement<T>* oldHead = l;
            l = l->s;
            delete oldHead;
            return true;
        }

        return retire(a, l->s);
    }

    // Méthode pour effacer la liste (sans effacer les données)
    static void efface1(PElement<T>*& l) {
        if (l == nullptr) return;
        efface1(l->s);
        delete l;
        l = nullptr;
    }

    // Méthode pour effacer la liste et les données
    static void efface2(PElement<T>*& l) {
        if (l == nullptr) return;
        delete l->v;
        efface2(l->s);
        delete l;
        l = nullptr;
    }

    // Méthode pour copier la liste (sans copier les données)
    static PElement<T>* copie1(PElement<T>* l) {
        if (l == nullptr) return nullptr;
        return new PElement<T>(l->v, copie1(l->s));
    }

    // Méthode pour copier la liste (avec copie des données)
    static PElement<T>* copie2(PElement<T>* l) {
        if (l == nullptr) return nullptr;
        return new PElement<T>(new T(*l->v), copie2(l->s));
    }

    // Méthode pour copier la liste (avec copie des données via copie())
    static PElement<T>* copie3(PElement<T>* l) {
        if (l == nullptr) return nullptr;
        return new PElement<T>(l->v->copie(), copie3(l->s));
    }

    // Méthode pour vérifier l'appartenance d'un élément
    static PElement<T>* appartient(const T* a, PElement<T>* l) {
        if (l == nullptr) return nullptr;
        if (l->v == a) {
            return l;
        }
        return appartient(a, l->s);
    }

    // Méthode pour vérifier l'appartenance via un foncteur
    template <class FONCTEUR>
    static PElement<T>* appartient(PElement<T>* l, const FONCTEUR& condition) {
        if (l == nullptr) return nullptr;
        if (condition(l->v)) {
            return l;
        }
        return appartient(l->s, condition);
    }
};

template <class T>
ostream& operator<<(ostream& os, const PElement<T>* p) {
    return os << PElement<T>::toString(p);
}

// Foncteurs pour les comparaisons
class CompareDouble {
public:
    static bool estPlusPetitOuEgal(const double* a1, const double* a2) {
        return *a1 <= *a2;
    }
};

class CompareString {
public:
    static bool estPlusPetitOuEgal(const string* a1, const string* a2) {
        return a1->compare(*a2) <= 0;
    }
};


#endif //TEMPLATE_DE_CLASSE_PELEMENT_H
