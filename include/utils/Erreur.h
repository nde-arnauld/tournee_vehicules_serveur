#ifndef ERREUR
#define ERREUR

using namespace std;

class Erreur : public exception {
public:
    virtual const char* what() const throw() {
        return "Erreur : liste vide";
    }
};

#endif
