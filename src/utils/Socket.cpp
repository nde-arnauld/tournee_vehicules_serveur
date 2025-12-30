#include <iostream>
#include <cstring>
#include "utils/Socket.h"
#ifdef _WIN32
#include <sstream>
#include "utils/WinsockInitializer.h"
#endif

Socket::Socket(const string &adr, const int &port) {
    #ifdef _WIN32
    WinsockInitializer::getInstance();
    #endif
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
    _sockaddr.sin_family = AF_INET;
    _sockaddr.sin_addr.s_addr = inet_addr(adr.c_str());
    _sockaddr.sin_port = htons(port);  //htons() assure que le port est bien inscrit dans le format du réseau (little-endian ou big-endian)
    
    #ifdef _WIN32
    int r = connect(_sock, (SOCKADDR *) &_sockaddr, sizeof(_sockaddr));     // renvoie une valeur non nulle en cas d'échec.
    #elif __linux
    int r = connect(_sock, (struct sockaddr *)&_sockaddr, sizeof(_sockaddr));
    #endif
    #ifdef _WIN32
    if (r == SOCKET_ERROR)
    #elif __linux
    if (r < 0)
    #endif
        throw SocketException("La connexion a échoué");
}

Socket::Socket() {
    // Constructeur par défaut - utilisé pour les sockets serveur
    #ifdef _WIN32
    WinsockInitializer::getInstance();
    #endif
}

Socket::~Socket() {
    #ifdef _WIN32
    int r = shutdown(_sock, SD_BOTH);       // on coupe la connexion pour l'envoi et la réception
                // renvoie une valeur non nulle en cas d'échec. Le code d'erreur peut être obtenu par un appel à WSAGetLastError()
    if (r == SOCKET_ERROR)
    #elif __linux
    int r = shutdown(_sock, SHUT_RDWR); // on coupe la connexion pour l'envoi et la réception
    if (r < 0)
    #endif
        cerr << "la coupure de connexion a échoué" << endl;
    
    #ifdef _WIN32
    r = closesocket(_sock);                          // renvoie une valeur non nulle en cas d'échec. Le code d'erreur peut être obtenu par un appel à WSAGetLastError()
    if (r) {
    #elif __linux
    r = close(_sock);
    if (r < 0) {
    #endif
        cerr << "La fermeture du socket a échoué" << endl;
    }
}

void Socket::envoyer(const string &s) {
    string requete = s + "\r\n";
    int r = send(_sock, requete.c_str(), requete.length(), 0);
    #ifdef _WIN32
    if (r == SOCKET_ERROR)
    #elif __linux
    if (r < 0)
    #endif
        throw SocketException("échec de l'envoi de la requête");
}

string Socket::recevoir() {
    char * reponse = (char*)calloc(L, sizeof(char)) ;
    int r = recv(_sock, reponse, L, 0);
    #ifdef _WIN32
    if (r == SOCKET_ERROR)
    #elif __linux
    if (r < 0)
    #endif
        throw SocketException("La réception de la réponse a échoué");
    string s(reponse);
    free(reponse);
    return s;
}

string Socket::getAdresse() const {
    return inet_ntoa(_sockaddr.sin_addr);
}

int Socket::getPort() const {
    return ntohs(_sockaddr.sin_port);
}
