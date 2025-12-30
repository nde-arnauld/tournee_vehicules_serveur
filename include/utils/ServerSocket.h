#pragma once
#include <string>
#include "Socket.h"

#ifdef _WIN32
#include <winsock2.h>
#elif __linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif

using namespace std;

class ServerSocket {
private:
    #ifdef _WIN32
    SOCKET _sock;
    SOCKADDR_IN _sockaddr;
    #elif __linux
    int _sock;
    struct sockaddr_in _sockaddr;
    #endif
    int _port;
    string _address;

public:
    /**
     * Constructeur d'un ServerSocket.
     * @param port Le numéro de port sur lequel écouter.
     * @param address L'adresse IP sur laquelle écouter (par défaut "127.0.0.1").
     */
    ServerSocket(const int &port, const string &address = "127.0.0.1");
    
    /**
     * Destructeur du ServerSocket.
     */
    ~ServerSocket();

    /**
     * Accepte une connexion entrante.
     * @return Un objet Socket connecté au client.
     * Note: Cette méthode nécessite une modification de la classe Socket pour fonctionner correctement.
     * L'utilisateur doit créer un Socket avec les paramètres retournés par cette méthode.
     */
    Socket accepter();
    
    /**
     * Ferme le socket serveur.
     */
    void fermer();
};