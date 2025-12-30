#ifndef SERVEUR_H_INCLUDED
#define SERVEUR_H_INCLUDED

#include <iostream>
#include <string>
#include <vector>
#include <thread>
#include <mutex>
#include <map>

// Inclusion conditionnelle pour les Sockets
#ifdef _WIN32
    #include <winsock2.h>
    #include <ws2tcpip.h>
    #pragma comment(lib, "Ws2_32.lib")
    using SocketType = SOCKET;
    using SockLenType = int;
#else
    #include <sys/socket.h>
    #include <netinet/in.h>
    #include <unistd.h>
    #include <arpa/inet.h>
    #include <cstring>
    using SocketType = int;
    using SockLenType = socklen_t;
    #define INVALID_SOCKET -1
    #define SOCKET_ERROR -1
#endif

#include "json.hpp"

using json = nlohmann::json;

class Serveur {
private:
    int port;
    SocketType serveurSocket;
    bool actif;
    std::vector<std::string> regionsChargees;
    
    // Pour les routes ont va utiliser
    // Clef : "VilleA_VilleB", Valeur : "TypeDeRoute"
    std::map<std::string, std::string> baseDeDonneesRoutes;

    void initialiserReseau();
    void nettoyerReseau();
    
    // Méthode exécutée dans un thread pour chaque client
    void gererClient(SocketType clientSocket);

    // Lecture robuste de N octets (boucle tant que tout n'est pas reçu)
    bool recevoirDonnees(SocketType socket, char* buffer, int taille);
    
    // Envoi robuste
    bool envoyerDonnees(SocketType socket, const std::string& message);

    // Charge le CSV
    void chargerRegion(const std::string& nomRegion);

public:
    Serveur(int port);
    ~Serveur();
    void demarrer();
};

std::string trim(const std::string& s);
std::vector<std::string> split(const std::string& s, char delimiter);

#endif