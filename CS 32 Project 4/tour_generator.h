#ifndef tour_generator_h
#define tour_generator_h

#include "base_classes.h"
#include "hashmap.h"
#include "geotools.h"
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <set>
#include <unordered_set>
#include <cmath>
using namespace std;

class TourGenerator: public TourGeneratorBase
{
public:
 TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router);
 virtual ~TourGenerator();
 virtual std::vector<TourCommand> generate_tour(const Stops& stops) const;
private:
    const GeoDatabaseBase& geodatabase;
    const RouterBase& routerbase;
    std::string calculateDirection(double angle) const {
        if (angle < 22.5) return "east";
        else if (angle < 67.5) return "northeast";
        else if (angle < 112.5) return "north";
        else if (angle < 157.5) return "northwest";
        else if (angle < 202.5) return "west";
        else if (angle < 247.5) return "southwest";
        else if (angle < 292.5) return "south";
        else if (angle < 337.5) return "southeast";
        else return "east";
    }
};


#endif /* tour_generator_h */
