#ifndef VILLE_H_INCLUDED
#define VILLE_H_INCLUDED

#include <string>
#include <iostream>

class Ville {
private:
    std::string nom;
    double latitude;
    double longitude;

public:
    Ville();
    Ville(std::string n, double lat, double lon);

    std::string getNom() const;
    double getLatitude() const;
    double getLongitude() const;

    friend std::ostream& operator<<(std::ostream& os, const Ville& v);
    
    operator std::string() const;
};
#endif