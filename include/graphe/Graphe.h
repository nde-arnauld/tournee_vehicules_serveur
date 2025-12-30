#ifndef GRAPHE_GRAPHE_H
#define GRAPHE_GRAPHE_H

#include "PElement.h"
#include "Arete.h"
#include "Sommet.h"
#include "AElement.h"

template <class S, class T>
class Graphe {
private:
    int prochaineClef;
    PElement<Sommet<T>> * ISommets;
    PElement<Arete<S,T>> * IAretes;

    class CritereSommet {
    private:
        Sommet<T> * a;
    public:
        CritereSommet(Sommet<T> * a) : a(a) {}
        bool operator () (Sommet<T> *sommet) const { return sommet->v == a->v; }
    };

     /**
     * crée une arête joignant les 2 sommets debut et fin
     * met à jour les champs degre de debut et de fin
     * met à jour prochaineClef
     * */
    Arete<S,T> * copieArete(const int clef, const S & info, Sommet<T> * debut, Sommet<T> * fin) {
        IAretes = new PElement<Arete<S, T>>(new Arete<S, T>(clef, info, debut, fin), IAretes);
        prochaineClef++;
        return IAretes->v;
    }

    void copieDe(const Graphe &graphe) {
        ISommets = PElement<Sommet<T>>::copie2(graphe.ISommets);
        PElement<Arete<S,T>> * a = graphe.IAretes;
        while (a != nullptr) {
            if (a->v == nullptr) {
                a = a->s;
                continue;
            }

            CritereSommet critere(a->v->debut);
            PElement<Sommet<T>> * tmpDebElement = PElement<Sommet<T>>::appartient(ISommets, critere);
            Sommet<T> * tmpDeb = tmpDebElement != nullptr ? tmpDebElement->v : nullptr;

            critere = CritereSommet(a->v->fin);
            PElement<Sommet<T>> * tmpFinElement = PElement<Sommet<T>>::appartient(ISommets, critere);
            Sommet<T> * tmpFin = tmpFinElement != nullptr ? tmpFinElement->v : nullptr;


            copieArete(a->v->clef, a->v->v, tmpDeb, tmpFin);
            a = a->s;
        }
        prochaineClef = graphe.prochaineClef;
    }

public:
    Graphe() : prochaineClef(0), ISommets(nullptr), IAretes(nullptr) {}

    // Constructeur de copie
    Graphe(const Graphe<S, T> & graphe) : prochaineClef(graphe.prochaineClef) {
        this->copieDe(graphe);
    }

    // Destructeur
    ~Graphe() {
        PElement<Arete<S,T>>::efface2(IAretes);
        PElement<Sommet<T>>::efface2(ISommets);
    }

    /**
     * crée un sommet isolé
     * met à jour prochaineClef
     * */
    Sommet<T> * creeSommet(const T & info) {
        int clef = prochaineClef;
        ISommets = new PElement<Sommet<T>>(new Sommet<T>(clef, info), ISommets);
        prochaineClef++;

        return ISommets->v;
    }
    /**
     * crée une arête joignant les 2 sommets debut et fin
     * met à jour les champs degre de debut et de fin
     * met à jour prochaineClef
     * */
    Arete<S,T> * creeArete(const S & info, Sommet<T> * debut, Sommet<T> * fin) {
        int clef = prochaineClef;
        ++debut->degre;
        ++fin->degre;
        IAretes = new PElement<Arete<S, T>>(new Arete<S, T>(clef, info, debut, fin), IAretes);
        prochaineClef++;
        return IAretes->v;
    }

    int nombreSommets() const {
        int nombre = 0;
        PElement<Sommet<T>> * s = ISommets;
        while (s != nullptr) {
            nombre++;
            s = s->s;
        }
        return nombre;
    }

    int nombreAretes() const {
        int nombre = 0;
        PElement<Arete<S,T>> * a = IAretes;
        while (a != nullptr) {
            nombre++;
            a = a->s;
        }
        return nombre;
    }

    /**
     * construit la liste de toutes les arêtes incidentes en sommet
     */
    PElement< Arete<S,T> > * aretesAdjacentes(const Sommet<T> * sommet) const {
        PElement<Arete<S,T>> * result = nullptr;
        PElement<Arete<S,T>> * a = IAretes;

        while (a != nullptr) {
            if (a->v->debut == sommet || a->v->fin == sommet)
                result = new PElement<Arete<S,T>>(a->v, result);
            a = a->s;
        }

        return result;
    }

    /**
     * construit la liste de tous les sommets voisins en sommet
     */
    PElement<Sommet<T> > * voisins(const Sommet<T> * sommet) const {
        PElement<Sommet<T>> * result = nullptr;
        PElement<Arete<S,T>> * a = IAretes;

        while (a != nullptr) {
            if (a->v->debut == sommet)
                result = new PElement<Sommet<T>>(a->v->fin, result);
            else if (a->v->fin == sommet)
                result = new PElement<Sommet<T>>(a->v->debut, result);
            a = a->s;
        }

        return result;
    }

    /**
     * cherche l'arête s1 - s2 ou l'arête s2 - s1 si elle existe
     */
    Arete<S,T> * getAreteParSommets(const Sommet<T> * s1, const Sommet<T> * s2) const {
        PElement<Arete<S,T>> * a = IAretes;

        while (a != nullptr) {
            if ((a->v->debut == s1 && a->v->fin == s2) ||
                (a->v->debut == s2 && a->v->fin == s1)) {
                return a->v;
            }
            a = a->s;
        }

        return nullptr;
    }

    // Opérateur de conversion en string
    operator string() const {
       ostringstream os;
        os << "Graphe{nombreSommets=" << nombreSommets()
           << ", nombreAretes=" << nombreAretes() << ", (";

        // Ajouter les sommets
        PElement<Sommet<T>>* s = ISommets;
        while (s != nullptr) {
            os << *s->v;
            if (s->s != nullptr)
                os << ", ";
            s = s->s;
        }
        os << "), (";

        // Ajouter les arêtes
        PElement<Arete<S, T>>* a = IAretes;
        while (a != nullptr) {
            os << *a->v;
            if (a->s != nullptr)
                os << ", ";
            a = a->s;
        }

        os << ")}";

        return os.str();
    }

    // Opérateur << pour l'écriture sur un flux
    friend ostream& operator<<(ostream& os, const Graphe<S, T>& graphe) {
        os << string(graphe);
        return os;
    }

    // Opérateur d'affectation
    const Graphe<S, T> & operator=(const Graphe<S, T> & graphe) {
        if (this != &graphe) {
            PElement<Arete<S,T>>::efface2(IAretes);
            PElement<Sommet<T>>::efface2(ISommets);

            this->copieDe(graphe);
        }
        return *this;
    }

};

#endif //GRAPHE_GRAPHE_H
