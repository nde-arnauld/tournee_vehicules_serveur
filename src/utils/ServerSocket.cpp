#include <iostream>
#include <cstring>
#include <string>
#include "utils/ServerSocket.h"
#include "utils/Socket.h"
#ifdef _WIN32
#include <sstream>
#include "utils/WinsockInitializer.h"
#endif

using namespace std;

ServerSocket::ServerSocket(const int &port, const string &address) {
    #ifdef _WIN32
    WinsockInitializer::getInstance();
    #endif
    _port = port;
    _address = address;
    
    int familleAdresses = AF_INET;         // IPv4
    int typeSocket = SOCK_STREAM;           // mode connecté TCP
    #ifdef _WIN32
    int protocole = IPPROTO_TCP;            // protocole. On peut aussi mettre 0 et la fct choisit le protocole en fct des 2 1ers paramètres
                                            // pour les valeurs des paramètres : cf. fct socket dans la doc sur winsock
    #elif __linux
    int protocole = 0;
    #endif

    _sock = socket(familleAdresses, typeSocket, protocole);
    #ifdef _WIN32
    if (_sock == INVALID_SOCKET) {
        ostringstream oss;
        oss << "la création du socket a échoué : code d'erreur = " << WSAGetLastError() << endl; // pour les valeurs renvoyées par WSAGetLastError() : cf. doc réf winsock
        throw SocketException(oss.str().c_str());
    }
    #elif __linux
    if (_sock < 0)
        throw SocketException("la création du socket a échoué");
    #endif
    
    // Permet de réutiliser l'adresse immédiatement
    int reuse = 1;
    #ifdef _WIN32
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, (char*)&reuse, sizeof(reuse)) == SOCKET_ERROR) {
    #elif __linux
    if (setsockopt(_sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) < 0) {
    #endif
        throw SocketException("Impossible de réutiliser l'adresse");
    }

    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = inet_addr(_address.c_str());
    _sockaddr.sin_port = htons(_port);
    
    // Lie le socket à l'adresse et au port
    #ifdef _WIN32
    if (bind(_sock, (SOCKADDR *) &_sockaddr, sizeof(_sockaddr)) == SOCKET_ERROR) {
    #elif __linux
    if (bind(_sock, (struct sockaddr *)&_sockaddr, sizeof(_sockaddr)) < 0) {
    #endif
        throw SocketException("La liaison du socket à l'adresse et au port a échoué");
    }
}

ServerSocket::~ServerSocket() {
    fermer();
}

Socket ServerSocket::accepter() {
    // Met le socket en mode écoute
    #ifdef _WIN32
    int r = listen(_sock, 5);
    if (r == SOCKET_ERROR) {
        ostringstream oss;
        oss << "l'écoute a échoué : code d'erreur = " << WSAGetLastError() << endl;
        throw SocketException(oss.str().c_str());
    }
    #elif __linux
    if (listen(_sock, 5) < 0) {
        throw SocketException("l'écoute a échoué");
    }
    #endif
    
    // Accepte la connexion
    #ifdef _WIN32
    int tailleAdresse = sizeof(_sockaddr);
    SOCKET clientSock = accept(_sock, (SOCKADDR *) &_sockaddr, &tailleAdresse);
    #elif __linux
    socklen_t tailleAdresse = sizeof(_sockaddr);
    int clientSock = accept(_sock, (struct sockaddr *)&_sockaddr, &tailleAdresse);
    #endif
    
    #ifdef _WIN32
    if (clientSock == INVALID_SOCKET) {
    #elif __linux
    if (clientSock < 0) {
    #endif
        throw SocketException("L'acceptation de la connexion a échoué");
    }
    
    // Informations du client pour l'utilisateur
    string clientAdr = inet_ntoa(_sockaddr.sin_addr);
    int clientPort = ntohs(_sockaddr.sin_port);
    
    // Note: Pour un usage complet, l'utilisateur doit créer un Socket avec les paramètres suivants :
    // Socket clientSocket(clientAdr, clientPort);
    throw SocketException("Pour créer un Socket client, utilisez : Socket clientSocket(\"" + 
                         clientAdr + "\", " + to_string(clientPort) + ")");
}

void ServerSocket::fermer() {
    #ifdef _WIN32
    int r = shutdown(_sock, SD_BOTH);
    if (r == SOCKET_ERROR)
    #elif __linux
    int r = shutdown(_sock, SHUT_RDWR);
    if (r < 0)
    #endif
        cerr << "la coupure de connexion a échoué" << endl;
    
    #ifdef _WIN32
    r = closesocket(_sock);
    if (r) {
    #elif __linux
    r = close(_sock);
    if (r < 0) {
    #endif
        cerr << "La fermeture du socket a échoué" << endl;
    }
}