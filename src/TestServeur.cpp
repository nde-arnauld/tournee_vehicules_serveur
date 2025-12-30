#include <iostream>
#include "utils/ServerSocket.h"
#include "utils/Socket.h"

using namespace std;

int main() {
    try {
        // Création d'un ServerSocket écoutant sur le port 8080
        ServerSocket serverSocket(8080);
        
        cout << "Serveur démarré sur le port 8080..." << endl;
        
        // Accepter une connexion
        cout << "En attente d'une connexion..." << endl;
        Socket clientSocket = serverSocket.accepter();
        
        // Le code ci-dessus lèvera une exception indiquant comment créer un Socket client
        // Cela est dû aux limitations de l'implémentation actuelle
        
        // Pour une utilisation réelle, il faudrait plutôt faire :
        /*
        Socket clientSocket("127.0.0.1", 8080); // Utiliser les paramètres réels
        */
        
    } catch (const SocketException& e) {
        cout << "Erreur: " << e.what() << endl;
    }
    
    return 0;
}