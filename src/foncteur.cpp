#include <string>

using namespace std;

struct CommenceParC
{
    bool operator()(const string* s) const
    {
        return !s->empty() && (*s)[0] == 'c';
    }
};
