#ifndef EVALUATEUR_H_INCLUDED
#define EVALUATEUR_H_INCLUDED

#include <memory>
#include "route.h" // Si vous utilisez vos classes de routes

// Cas 1 : Si S est un simple double
struct EvaluateurSimple {
    double operator()(const double& poids) const;
};

// Cas 2 : Si S est un pointeur intelligent vers une Route (VOTRE CAS)
struct EvaluateurRoutePtr {
    double operator()(const std::shared_ptr<Route>& route) const;
};

#endif