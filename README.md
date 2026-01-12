# Serveur d'optimisation de tournées - Backend C++

Ce projet est la partie serveur d'une application **(Application de livraison)** de calcul d'itinéraires. Il s'agit d'un serveur TCP multithreadé développé en C++ qui résout le **Problème du Voyageur de Commerce** (TSP).

Le serveur écoute les requêtes clients via des sockets, construit un graphe pondéré à partir de fichiers CSV de données routières de la France et renvoie le chemin optimal pour visiter une liste de villes donnée.

## 📂Structure et architecture du projet

Le code source est organisé en trois répertoires principaux, séparant clairement les données, les définitions (headers) et les implémentations :

* **`donnees/`** : Ce dossier contient les bases de données au format `.csv` nécessaires au chargement du graphe. On y trouve notamment les définitions des types de routes entre les villes des différentes régions.
* **`include/`** : Ce dossier regroupe tous les fichiers d'en-tête (`.h`) du projet. Il contient les déclarations des classes (`graphe.h`, `serveur.h`, `ville.h`, `algorithme_tsp.h`) ainsi que la librairie externe `json.hpp` requise pour le parsing.
* **`src/`** : Ce dossier contient les implémentations (`.cpp`) de la logique serveur, telles que `serveur.cpp` et `ville.cpp`. C'est également ici que se trouve `main_serveur.cpp`, qui constitue le **point d'entrée** et de lancement de l'application.

### Rôle des fichiers principaux

Voici une description détaillée des composants clés qui constituent la logique du serveur :

* **`serveur.cpp/h`** : C'est le cœur du système réseau. Il gère l'écoute sur le port (`8080`), l'acceptation des connexions TCP, la création des threads (multithreading) pour gérer plusieurs clients simultanément, et le respect du protocole de communication binaire/JSON.
* **`graphe.h`** : Il contient la structure de données générique `Graphe<S, T>` représentant le réseau routier complet. C'est sur cette structure que l'algorithme de recherche va opérer.
* **`ville.h`** : Cette classe représente les sommets du graphe. Chaque objet Ville contient ses métadonnées (Nom, Latitude, Longitude).
* **`route.h`** : Implémentation du *Factory Pattern*. Ce module est responsable de la création dynamique des arêtes (Routes) en fonction de leur type (Autoroute, Nationale, Départementale), influençant ainsi le calcul du poids (temps/distance).
* **`algorithme_tsp.h`** : Ce fichier contient l'algorithme de résolution du problème (TSP). Il prend un sous-graphe en entrée et retourne le cycle le plus court visitant toutes les villes demandées.

## 🛠 Installation et prérequis

Avant de pouvoir lancer le serveur, assurez-vous que votre environnement de développement est correctement configuré.

### 1. Prérequis système

* **MinGW (Minimalist GNU for Windows)** :
    * Le projet nécessite un compilateur C++ (GCC/G++) compatible C++17.
    * Il nécessite également l'outil de build `mingw32-make`.
    * *Note : Assurez-vous que le dossier `bin` de MinGW est bien ajouté à la variable d'environnement `PATH` de votre système.*

* **Librairie JSON (Dépendance externe)** :
    * Le projet dépend de la librairie *header-only* `nlohmann/json`.
    * Vous devez télécharger le fichier `json.hpp` (disponible [ici](https://github.com/nlohmann/json/releases)) et le placer manuellement dans le dossier `include/`.
    * **Vérification :** Le fichier doit se trouver à l'emplacement : `projet/include/json.hpp`.

### 2. Compilation et démarrage

Le projet inclut un `Makefile` configuré pour automatiser la compilation et l'exécution. Ouvrez votre terminal (Invite de commandes ou PowerShell) à la racine du projet.

**Pour compiler et lancer le serveur en une seule commande :**
* **Windows**
```bash
mingw32-make run
```

**Pour nettoyer les fichiers compilés:**
```bash
mingw32-make clean
```

## 📚 Documentation du protocole et de l'API

Le serveur utilise un protocole **TCP propriétaire** basé sur des sockets bruts. Pour interagir avec le serveur, le client doit implémenter strictement le format d'échange de paquets décrit ci-dessous.

### 1. Format du Paquet (Trame Réseau)

Chaque message (qu'il soit envoyé par le client ou renvoyé par le serveur) est composé de deux parties indissociables : un en-tête de taille et le corps du message.

| Partie | Taille | Type | Description |
| :--- | :--- | :--- | :--- |
| **1. En-tête** | 4 octets | `int` (32-bit) | Indique la taille exacte du JSON en octets. (Format Little Endian sur architectures x86/x64). |
| **2. Corps** | N octets | `string` | La chaîne de caractères JSON encodée en UTF-8. |

**Important :** Le client ne doit pas envoyer le JSON directement. Il doit d'abord envoyer les 4 octets représentant la taille, suivis immédiatement des données.

### 2. Flux de Communication

Le cycle de vie d'une requête se déroule comme suit :

1.  **Connexion :** Le client ouvre une connexion TCP sur le port d'écoute du serveur.
2.  **Envoi Requête :**
    * Le client sérialise sa liste de villes en JSON.
    * Il calcule la taille du blob JSON en octets.
    * Il envoie les 4 octets de taille + le blob JSON.
3.  **Traitement :** Le serveur reçoit la taille, alloue la mémoire, lit le JSON, calcule la tournée optimale (TSP).
4.  **Réponse :** Le serveur renvoie le résultat selon le même format (4 octets de taille + JSON trié).

### 3. Schéma des Données JSON

#### ➡️ Requête (Client vers Serveur)
Le client doit envoyer un tableau d'objets JSON. Chaque objet représente une ville à visiter.

**Structure attendue :**
* `ville` (String) : Nom de la ville (Identifiant).
* `latitude` (Number/Double) : Coordonnée géographique.
* `longitude` (Number/Double) : Coordonnée géographique.

**Exemple de payload :**
```json
[
  { "ville": "Metz", "latitude": 49.1193, "longitude": 6.1757 },
  { "ville": "Strasbourg", "latitude": 48.5734, "longitude": 7.7521 },
  { "ville": "Nancy", "latitude": 48.6921, "longitude": 6.1844 }
]
```

#### ➡️ Réponse (Serveur vers Client)

Le serveur renvoie la tournée ordonnée. Chaque objet contient désormais :

* distance : La distance géodésique vers la ville suivante dans la liste.
* type_route : Le type de route empruntée pour aller vers la ville suivante (ex: "autoroute", "nationale", ...).
* Le dernier élément de la liste aura toujours une distance de 0 et le type "terminus".

Exemple de réponse :

```json
[
  { "ville": "Metz", "latitude": 49.1193, "longitude": 6.1757, "distance": 165.4, "type_route": "autoroute" },
  { "ville": "Strasbourg", "latitude": 48.5734, "longitude": 7.7521, "distance": 150.2, "type_route": "nationale" },
  { "ville": "Nancy", "latitude": 48.6921, "longitude": 6.1844, "distance": 57.5, "type_route": "autoroute" },
  { "ville": "Metz", "latitude": 49.1193, "longitude": 6.1757, "distance": 0.0, "type_route": "terminus" }
]
```
(Note : Dans un cycle complet TSP, la ville de départ est souvent répétée à la fin, selon l'implémentation de l'algorithme).