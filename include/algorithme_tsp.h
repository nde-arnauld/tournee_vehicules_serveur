#ifndef ALGORITHME_TSP_H_INCLUDED
#define ALGORITHME_TSP_H_INCLUDED

#include <vector>
#include <set>
#include <limits>
#include "graphe.h"

template <typename S, typename T, typename EVALUATEUR>
class AlgorithmeTSP {
public:
    // Retourne une liste de pointeurs vers les sommets ordonnés
    static std::vector<Sommet<T>*> resoudre(const Graphe<S, T>* graphe, Sommet<T>* depart) {
        std::vector<Sommet<T>*> chemin;
        // Puisque nous connaissons le nombre de villes, nous reservons la mémoire à l'avance
        chemin.reserve(graphe->nombreSommets() + 1);

        std::set<int> visite; // On stocke les IDs (clef) des sommets visités
        EVALUATEUR eval;      // L'outil pour lire la distance

        Sommet<T>* courant = depart;
        
        // On ajoute le point de départ
        chemin.push_back(courant);
        visite.insert(courant->clef);

        // Boucle tant qu'on n'a pas visité tout le monde
        // (On s'arrête à nombreSommets - 1 car la dernière étape est le retour au dépôt)
        while (visite.size() < (size_t)graphe->nombreSommets()) {
            
            Sommet<T>* meilleurVoisin = nullptr;
            double minDistance = std::numeric_limits<double>::max();

            // On récupère la liste des adjacences (Paires Sommet/Arete)
            auto listeAdjacences = graphe->adjacences(courant);
            
            // Parcours manuel de la liste chaînée PElement
            auto p = listeAdjacences;
            while (p != nullptr) {
                // p->v est un pointeur vers une std::pair<Sommet<T>*, Arete<S, T>*>
                Sommet<T>* voisin = p->v->first;
                Arete<S, T>* arete = p->v->second;

                // Si le voisin n'est pas encore visité
                if (visite.find(voisin->clef) == visite.end()) {
                    // On utilise l'évaluateur pour récupérer la distance depuis l'arête
                    // arete->v contient l'objet S (votre shared_ptr<Route>)
                    double distance = eval(arete->v);

                    if (distance < minDistance) {
                        minDistance = distance;
                        meilleurVoisin = voisin;
                    }
                }
                
                p = p->s;
            }
            
            // Nettoyage de la liste temporaire d'adjacences
            PElement<std::pair<Sommet<T> *, Arete<S, T> *>>::efface2(listeAdjacences);

            // Validation du mouvement
            if (meilleurVoisin != nullptr) {
                visite.insert(meilleurVoisin->clef);
                chemin.push_back(meilleurVoisin);
                courant = meilleurVoisin;
            } else {
                std::cout << "Probleme avec l'algorithme TSP" << std::endl;
                break;
            }
        }

        // 4. Retour au point de départ
        chemin.push_back(depart);

        return chemin;
    }
};

#endif