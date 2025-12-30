#include <memory>
#include "route.h"
#include "distance.h"

// Fonction utilitaire pour convertir degrés en radians
double CalculateurDistance::toRadians(double degree) const
{
    return degree * (M_PI / 180.0);
}

// Surcharge de l'opérateur () : C'est ce qui fait de cette structure un "Foncteur"
double CalculateurDistance::operator()(const Ville &v1, const Ville &v2) const
{
    // 1. Récupération des coordonnées et conversion en radians
    double lat1 = toRadians(v1.getLatitude());
    double lon1 = toRadians(v1.getLongitude());
    double lat2 = toRadians(v2.getLatitude());
    double lon2 = toRadians(v2.getLongitude());

    // 2. Calcul de la distance angulaire (Loi des cosinus sphérique)
    // Cette formule respecte la géométrie du triangle sphérique formé par les deux villes et le centre de la terre
    double u = std::sin(lat1) * std::sin(lat2) +
               std::cos(lat1) * std::cos(lat2) * std::cos(lon2 - lon1);

    // Correction pour éviter les erreurs numériques (si u > 1 ou u < -1 à cause des arrondis)
    if (u > 1.0) u = 1.0;
    if (u < -1.0) u = -1.0;

    // 3. Calcul de l'angle (arc cosinus)
    double angle = std::acos(u);

    // 4. Distance finale = Rayon * Angle
    return RAYON_TERRE * angle;
}