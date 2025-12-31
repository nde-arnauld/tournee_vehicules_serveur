#ifndef ROUTE_H_INCLUDED
#define ROUTE_H_INCLUDED

#include <iostream>
#include <string>
#include <map>
#include <memory>
#include <functional>
#include <sstream>

// Interface de base
class Route {
protected:
    double distance;
    
public:
    Route(double dist) : distance(dist) {}
    virtual ~Route() = default;
    virtual std::string getType() const = 0;
    double getDistance() const { return distance; }

    operator std::string() const 
    {
        std::ostringstream oss;
        oss << getType() << ": " << distance << "km";
        return oss.str();
    }

    friend std::ostream& operator <<(std::ostream& os, const Route& r)
    {
        return os << (std::string) r;
    }
};

// =========== Implémentations concrètes ===========
class RouteEuropeenne : public Route {
public:
    RouteEuropeenne(double dist) : Route(dist) {}
    std::string getType() const override { return "Route Europeenne"; }
};

class VoieRapide : public Route {
public:
    VoieRapide(double dist) : Route(dist) {}
    std::string getType() const override { return "Voie Rapide"; }
};

class RouteNationale : public Route {
public:
    RouteNationale(double dist) : Route(dist) {}
    std::string getType() const override { return "Route Nationale"; }
};

class RouteCommunale : public Route {
public:
    RouteCommunale(double dist) : Route(dist) {}
    std::string getType() const override { return "Route Communale"; }
};

class Autoroute : public Route {
public:
    Autoroute(double dist) : Route(dist) {}
    std::string getType() const override { return "Autoroute"; }
};

class RouteDepartementale : public Route {
public:
    RouteDepartementale(double dist) : Route(dist) {}
    std::string getType() const override { return "Route Departementale"; }
};

// =========== Factory avec registre ===========
class FabriqueRoute {
private:
    using Constructeur = std::function<std::shared_ptr<Route>(double)>;

    static std::map<std::string, Constructeur>& obtenirRegistre() {
        static std::map<std::string, Constructeur> registre;
        return registre;
    }
    
    // transforme la chaine reçue en minuscule
    static std::string normaliser(const std::string& str) {
        std::string result = str;
        for (char& c : result) {
            c = std::tolower(c);
        }
        return result;
    }

public:
    // Permet d'enregistrer un nouvel type de route
    template<typename T>
    static void enregistrer(const std::string& cle) {
        obtenirRegistre()[normaliser(cle)] = [](double dist) {
            return std::make_shared<T>(dist);
        };
    }
    
    // Créer une route à partir d'une chaîne et d'une distance
    static std::shared_ptr<Route> creer(const std::string& type, double distance) {
        chargerTypes();

        std::string typeNormalise = normaliser(type);
        auto route_constructeur = obtenirRegistre().find(typeNormalise);
        
        if (route_constructeur == obtenirRegistre().end()) {
            throw std::invalid_argument("Type de route inconnu : " + type);
        }
        
        return route_constructeur->second(distance);
    }
    
    // Initialiser tous les types de routes
    static void chargerTypes() {
        if (!obtenirRegistre().empty()) return;

        enregistrer<RouteEuropeenne>("europeenne");
        enregistrer<RouteEuropeenne>("route europeenne");
        enregistrer<VoieRapide>("voie rapide");
        enregistrer<VoieRapide>("rapide");
        enregistrer<RouteNationale>("nationale");
        enregistrer<RouteNationale>("route nationale");
        enregistrer<RouteCommunale>("communale");
        enregistrer<RouteCommunale>("route communale");
        enregistrer<Autoroute>("autoroute");
        enregistrer<RouteDepartementale>("departementale");
        enregistrer<RouteDepartementale>("route departementale");
    }
};

#endif