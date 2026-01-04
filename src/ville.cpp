#include <string>
#include <iostream>

#include "ville.h"

Ville::Ville() : nom(""), latitude(0), longitude(0) {}

Ville::Ville(std::string n, double lat, double lon) : nom(n), latitude(lat), longitude(lon) {}

std::string Ville::getNom() const { return nom; }
double Ville::getLatitude() const { return latitude; }
double Ville::getLongitude() const { return longitude; }

std::ostream& operator<<(std::ostream &oss, const Ville &v)
{
    oss << v.nom;
    return oss;
}

Ville::operator std::string() const { return nom; }
