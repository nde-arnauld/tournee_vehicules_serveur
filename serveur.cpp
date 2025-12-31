#include <fstream>
#include <sstream>
#include <algorithm>
#include <cctype>
#include <cmath>
#include <locale>

#include "Serveur.h"
#include "Graphe.h"
#include "Ville.h"
#include "route.h"
#include "distance.h"
#include "algorithme_tsp.h"
#include "evaluateur.h"

#include <sstream>

using namespace std;

Serveur::Serveur(int p) : port(p), actif(false), serveurSocket(INVALID_SOCKET)
{
    initialiserReseau();
}

Serveur::~Serveur()
{
    if (serveurSocket != INVALID_SOCKET)
    {
#ifdef _WIN32
        closesocket(serveurSocket);
#else
        close(serveurSocket);
#endif
    }
    nettoyerReseau();
}

void Serveur::initialiserReseau()
{
#ifdef _WIN32
    WSADATA wsaData;
    if (WSAStartup(MAKEWORD(2, 2), &wsaData) != 0)
    {
        cerr << "[Erreur] Echec WSAStartup" << endl;
        exit(1);
    }
#endif
}

void Serveur::nettoyerReseau()
{
#ifdef _WIN32
    WSACleanup();
#endif
}

// Fonction pour enlever les espaces au début et à la fin (TRIM)
string trim(const string &s)
{
    auto start = s.begin();
    while (start != s.end() && isspace(*start))
    {
        start++;
    }

    auto end = s.end();
    do
    {
        end--;
    } while (distance(start, end) > 0 && isspace(*end));

    return string(start, end + 1);
}

// Fonction de découpage robuste (garde les champs vides)
vector<string> split(const string &s, char delimiter)
{
    vector<string> tokens;
    string token;
    istringstream tokenStream(s);

    // L'astuce ici est de gérer correctement les ;; successifs
    while (getline(tokenStream, token, delimiter))
    {
        tokens.push_back(trim(token));
    }
    // Si la ligne finit par un ; getline l'ignore parfois, mais pour votre fichier c'est ok
    return tokens;
}

void Serveur::chargerRegion(const string &nomRegion)
{
    // 1. Vérification si déjà chargé
    if (find(regionsChargees.begin(), regionsChargees.end(), nomRegion) != regionsChargees.end())
    {
        return;
    }

    string nomFichier = "donnees/" + nomRegion + ".csv";
    ifstream fichier(nomFichier);

    if (!fichier.is_open())
    {
        cerr << "[Erreur] Impossible d'ouvrir le CSV : " << nomFichier << endl;
        return;
    }

    string ligne;
    vector<string> enTetesVilles;

    // Lecture de la première ligne (Les colonnes)
    if (getline(fichier, ligne))
    {
        // Le premier token sera vide (car la ligne commence par ;)
        enTetesVilles = split(ligne, ';');
    }

    // Lecture des lignes de données
    // Format: Strasbourg;;Communale;...
    while (getline(fichier, ligne))
    {
        if (ligne.empty())
            continue;

        vector<string> colonnes = split(ligne, ';');

        // La première colonne est la ville de départ
        if (colonnes.empty())
            continue;
        string villeDepart = colonnes[0];

        // On parcourt les colonnes suivantes
        for (size_t i = 1; i < colonnes.size(); ++i)
        {
            // Ne pas déborder si la ligne est plus longue que l'entête
            if (i >= enTetesVilles.size())
                break;

            string typeRoute = colonnes[i];
            string villeArrivee = enTetesVilles[i];

            // Si le type n'est pas vide, on stocke l'information
            if (!typeRoute.empty())
            {
                // Stockage bi-directionnel
                baseDeDonneesRoutes[villeDepart + "_" + villeArrivee] = typeRoute;
                baseDeDonneesRoutes[villeArrivee + "_" + villeDepart] = typeRoute;
            }
        }
    }

    fichier.close();
    regionsChargees.push_back(nomRegion);
    cout << "[Serveur] Region chargee (Matrice) : " << nomRegion << endl;
}

void Serveur::demarrer()
{
    serveurSocket = socket(AF_INET, SOCK_STREAM, 0);

    if (serveurSocket == INVALID_SOCKET)
    {
        cerr << "[Erreur] Creation socket impossible" << endl;
        return;
    }

    sockaddr_in adresse;
    adresse.sin_family = AF_INET;
    adresse.sin_addr.s_addr = INADDR_ANY;
    adresse.sin_port = htons(port);

    if (bind(serveurSocket, (struct sockaddr *)&adresse, sizeof(adresse)) == SOCKET_ERROR)
    {
        cerr << "[Erreur] Bind impossible (port deja utilise ?)" << endl;
        return;
    }

    if (listen(serveurSocket, 5) == SOCKET_ERROR)
    {
        cerr << "[Erreur] Listen impossible" << endl;
        return;
    }

    actif = true;
    cout << "[Serveur] En ecoute sur le port: " << port << "..." << endl;

    while (actif)
    {
        sockaddr_in clientAddr;
        SockLenType clientSize = sizeof(clientAddr);
        SocketType clientSocket = accept(serveurSocket, (struct sockaddr *)&clientAddr, &clientSize);

        if (clientSocket != INVALID_SOCKET)
        {
            cout << "[Info] Nouveau client connecte." << endl;
            // Lancement du thread (multithreading)
            // On utilise detach() pour que le thread vive sa vie sans bloquer le main
            thread t(&Serveur::gererClient, this, clientSocket);
            t.detach();
        }
    }
}

// Fonction utilitaire pour s'assurer qu'on reçoit tout
bool Serveur::recevoirDonnees(SocketType socket, char *buffer, int tailleAttendue)
{
    int totalRecu = 0;
    while (totalRecu < tailleAttendue)
    {
        int recu = recv(socket, buffer + totalRecu, tailleAttendue - totalRecu, 0);
        if (recu <= 0)
            return false; // Erreur ou déconnexion
        totalRecu += recu;
    }
    return true;
}

bool Serveur::envoyerDonnees(SocketType socket, const string &message)
{
    // 1. Envoi de la taille (4 octets, Big Endian idéalement, ici simple int)
    int tailleLocale = static_cast<int>(message.size());

    int tailleReseau = htonl(tailleLocale);

    if (send(socket, (char *)&tailleReseau, sizeof(int), 0) == SOCKET_ERROR)
        return false;

    // 2. Envoi du contenu
    int totalEnvoye = 0;
    while (totalEnvoye < tailleLocale)
    {
        int envoye = send(socket, message.c_str() + totalEnvoye, tailleLocale - totalEnvoye, 0);
        if (envoye == SOCKET_ERROR)
            return false;
        totalEnvoye += envoye;
    }
    return true;
}

void Serveur::gererClient(SocketType clientSocket)
{
    // PROTOCOLE :
    // 1. Lire 4 octets (int) = Taille du JSON
    // 2. Lire N octets = Le JSON

    while (true)
    {
        int tailleMessage = 0;
        if (!recevoirDonnees(clientSocket, (char *)&tailleMessage, sizeof(int)))
        {
            cerr << "[Client] Erreur reception entete taille." << endl;
#ifdef _WIN32
            closesocket(clientSocket);
#else
            close(clientSocket);
#endif
            return;
        }

        // Correction de la taille de données reçue
        tailleMessage = ntohl(tailleMessage);

        // Protection contre allocation abusive
        if (tailleMessage <= 0 || tailleMessage > 10000000)
        {
            // Fermer
            return;
        }

        vector<char> buffer(tailleMessage + 1); // +1 pour le \0

        if (!recevoirDonnees(clientSocket, buffer.data(), tailleMessage))
        {
            cerr << "[Client] Erreur reception corps message." << endl;
            return;
        }
        buffer[tailleMessage] = '\0'; // Fin de chaîne

        string jsonRecu = buffer.data();

        try
        {
            // === 1. PARSING JSON ===
            auto jInput = json::parse(jsonRecu);

            std::string region = "defaut";
            if (jInput.contains("region"))
            {
                region = jInput["region"];
            }

            chargerRegion(region);

            auto JVilles = jInput["villes"];

            // === 2. CONSTRUCTION DU GRAPHE ===
            using Carte = Graphe<shared_ptr<Route>, Ville>;

            Carte *carte = new Carte();
            CalculateurDistance calculDistance;
            vector<Sommet<Ville> *> sommets;

            // Création des sommets
            for (const auto &elem : JVilles)
            {
                string nom = elem["ville"];
                double lat = elem["latitude"];
                double lon = elem["longitude"];
                sommets.push_back(carte->creeSommet(Ville(nom, lat, lon)));
            }

            // Création des arêtes (Graphe Complet)
            for (size_t i = 0; i < sommets.size(); ++i)
            {
                for (size_t j = i + 1; j < sommets.size(); ++j)
                {
                    Ville v1 = sommets[i]->v;
                    Ville v2 = sommets[j]->v;

                    // Calcul distance
                    double dist = calculDistance(v1, v2);

                    // Recherche du type dans "l'Excel"
                    string clef = v1.getNom() + "_" + v2.getNom();
                    string clefInv = v2.getNom() + "_" + v1.getNom();

                    string typeRoute = "nationale";

                    if (baseDeDonneesRoutes.count(clef))
                        typeRoute = baseDeDonneesRoutes[clef];
                    else if (baseDeDonneesRoutes.count(clefInv))
                        typeRoute = baseDeDonneesRoutes[clefInv];
                    else if (dist > 100)
                        typeRoute = "autoroute";

                    auto route = FabriqueRoute::creer(typeRoute, dist);
                    carte->creeArete(route, sommets[i], sommets[j]);
                    carte->creeArete(route, sommets[j], sommets[i]);
                }
            }

            // === 3. RESOLUTION TSP ===
            auto tournee = AlgorithmeTSP<shared_ptr<Route>, Ville, EvaluateurRoutePtr>::resoudre(carte, sommets[0]);

            // === 4. PREPARATION REPONSE JSON ===
            json jOutput = json::array();

            for (size_t i = 0; i < tournee.size(); ++i)
            {
                auto *sommetActuel = tournee[i];
                json villeObj;

                // Infos de base
                villeObj["ville"] = sommetActuel->v.getNom();
                villeObj["latitude"] = sommetActuel->v.getLatitude();
                villeObj["longitude"] = sommetActuel->v.getLongitude();

                // Si ce n'est pas la dernière ville, on regarde la SUIVANTE
                if (i < tournee.size() - 1)
                {
                    auto *sommetSuivant = tournee[i + 1];
                    double distance = 0;

                    try
                    {
                        // On récupère l'arête entre Actuel -> Suivant
                        shared_ptr<Route> route = carte->getAreteParSommets(sommetActuel, sommetSuivant)->v;

                        if (route)
                        {
                            distance = round(route->getDistance() * 100.0) / 100.0;
                            villeObj["distance"] = distance;
                            villeObj["type_route"] = route->getType();
                        }
                        else
                        {
                            // Cas de secours
                            distance = calculDistance(sommetActuel->v, sommetSuivant->v);
                            distance = round(distance * 100.0) / 100.0;
                            villeObj["distance"] = distance;
                            villeObj["type_route"] = "vol_oiseau";
                        }
                    }
                    catch (...)
                    {
                        villeObj["type_route"] = "inconnu";
                    }
                }
                else
                {
                    // La dernière ville
                    villeObj["distance"] = 0;
                    villeObj["type_route"] = "terminus";
                }

                jOutput.push_back(villeObj);
            }

            string reponse = jOutput.dump();

            // === 5. ENVOI REPONSE ===
            envoyerDonnees(clientSocket, reponse);

            delete carte;
        }
        catch (const exception &e)
        {
            cerr << "[Erreur Traitement] " << e.what() << endl;
            // Optionnel : renvoyer un JSON d'erreur
        }
    }

#ifdef _WIN32
    closesocket(clientSocket);
#else
    close(clientSocket);
#endif
}