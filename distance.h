#ifndef DISTANCE_H_INCLUDED
#define DISTANCE_H_INCLUDED

#include <cmath>
#include "Ville.h"

// Rayon moyen de la Terre en km
const double RAYON_TERRE = 6371.0;

#ifndef M_PI
#define M_PI 3.14159265358979323846
#endif

struct CalculateurDistance
{
    // Conversion degrés -> radians
    double toRadians(double degree) const;

    double operator()(const Ville& vA, const Ville& vB) const;
};

#endif