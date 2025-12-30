#include <iostream>
#include <exception>
#include <string>

#include "serveur.h"

using namespace std;

int main(int argc, char* argv[]) {
    // 1. Configuration du Port
    // Par défaut 8080, ou celui passé en argument (ex: ./serveur.exe 9000)
    int port = 8080;
    
    if (argc > 1) {
        try {
            port = stoi(argv[1]);
        } catch (...) {
            cerr << "[Attention] Port invalide, utilisation du port 8080." << endl;
        }
    }

    cout << "========================================" << endl;
    cout << "   SERVEUR D'OPTIMISATION DE TOURNEES   " << endl;
    cout << "========================================" << endl;

    try {
        // 2. Instanciation du Serveur
        // Le constructeur va préparer Winsock (sur Windows)
        Serveur monServeur(port);

        // 3. Démarrage de la boucle d'écoute
        // Cette méthode contient une boucle while(true) et ne rendra pas la main
        // tant que le serveur n'est pas arrêté.
        monServeur.demarrer();

    } catch (const exception& e) {
        cerr << "[Erreur Critique] Le serveur a plante : " << e.what() << endl;
        return 1;
    } catch (...) {
        cerr << "[Erreur Critique] Erreur inconnue survenue." << endl;
        return 1;
    }

    return 0;
}