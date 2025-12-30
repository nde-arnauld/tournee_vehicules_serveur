//
// Created by jimenezy on 08/03/25.
//

#pragma once
#ifdef _WIN32
#include <winsock2.h>
#elif __linux
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <unistd.h>
#endif
#include <string>

using namespace std;

#define L 200 // longueur max (en octets) de la réponse du serveur

class SocketException : public exception {
private :
    string _msg;
public:
    SocketException(const string &msg) : _msg(msg) {}
    virtual ~SocketException() {}
    virtual const char* what() const throw() {
        return _msg.c_str();
    }
};

class Socket {
private:
    #ifdef _WIN32
    SOCKET _sock;
    SOCKADDR_IN _sockaddr;
    #elif __linux
    int _sock;
    struct sockaddr_in _sockaddr;
    #endif
public:
    Socket(const string &adr, const int &port);
    Socket(); // Constructeur par défaut pour les sockets serveur
    ~Socket();

    /**
     * Envoie une chaîne de caractères au serveur.
     * @param s La chaîne de caractères à envoyer.
     */
    void envoyer(const string &s);

    /**
     * Reçoit une réponse du serveur.
     * @return Une chaîne de caractères contenant la réponse du serveur.
     */
    string recevoir();
    
    // Méthode pour obtenir les informations du socket
    string getAdresse() const;
    int getPort() const;
};
