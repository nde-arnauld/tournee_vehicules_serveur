//
// Created by jimenezy on 08/03/25.
//

#ifndef WINSOCKINITIALIZER_H
#define WINSOCKINITIALIZER_H

#include <winsock2.h> // Pour Windows
#include "utils/Erreur.h"

class WinsockInitializer {
private:
    static WinsockInitializer* instance;

    WinsockInitializer() { // Constructeur privé
        WSADATA wsaData;        // structure contenant les données de la librairie winsock à initialiser
        /* en cas de succès, wsaData a été initialisée et l'appel a renvoyé la valeur 0 */
        if (WSAStartup(MAKEWORD(2,0), &wsaData))
            throw Erreur("L'initialisation a échoué"); // MAKEWORD(2,0) sert à indiquer la version de la librairie à utiliser : 1 pour winsock et 2 pour winsock2
    }

    ~WinsockInitializer() { // Destructeur
        WSACleanup();
    }

public:
    static WinsockInitializer& getInstance() {
        static WinsockInitializer instance; // Initialisation thread-safe
        return instance;
    }

    // Désactiver la copie et le déplacement
    WinsockInitializer(const WinsockInitializer&) = delete;
    WinsockInitializer& operator=(const WinsockInitializer&) = delete;
};

#endif //WINSOCKINITIALIZER_H
