#ifndef geodb_h
#define geodb_h
#include "base_classes.h"
#include "hashmap.h"
#include "geotools.h"
#include <vector>
#include <string>
#include <sstream>
using namespace std;

class GeoDatabase: public GeoDatabaseBase
{
public:
 GeoDatabase();
 virtual ~GeoDatabase();
 virtual bool load(const std::string& map_data_file);
 virtual bool get_poi_location(const std::string& poi,
 GeoPoint& point) const;
 virtual std::vector<GeoPoint> get_connected_points(const GeoPoint& pt)
 const;
 virtual std::string get_street_name(const GeoPoint& pt1,
 const GeoPoint& pt2) const;
private:
    struct StreetSegment {
        GeoPoint startPoint;
        GeoPoint endPoint;
        std::string streetName;
        GeoPoint midPoint;

        StreetSegment(const GeoPoint& start, const GeoPoint& end, const std::string& name)
            : startPoint(start), endPoint(end), streetName(name) {
            midPoint = midpoint(start, end);
        }
    };
    HashMap<std::vector<StreetSegment>> streetSegmentsByLocation;
    HashMap<GeoPoint> poiLocations;
    void insertOrUpdateSegment(const GeoPoint& point, const StreetSegment& segment);
};

#endif /* geodb_h */
