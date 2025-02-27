#ifndef router_h
#define router_h

#include "base_classes.h"
#include "hashmap.h"
#include "geotools.h"
#include <vector>
#include <string>
#include <sstream>
#include <queue>
#include <set>
#include <unordered_set>
#include <unordered_map>
#include <cmath>
using namespace std;

class Router: public RouterBase
{
public:
 Router(const GeoDatabaseBase& geo_db);
 virtual ~Router();
 virtual std::vector<GeoPoint> route(const GeoPoint& pt1,
 const GeoPoint& pt2) const;
private:
    const GeoDatabaseBase& geodatabase;
};

#endif /* router_h */
