#include "evaluateur.h"

double EvaluateurSimple::operator()(const double &poids) const
{
    return poids;
}

double EvaluateurRoutePtr::operator()(const std::shared_ptr<Route> &route) const
{
    if (route)
        return route->getDistance();
    return 1e9; // Distance infinie si null
}
