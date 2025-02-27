#include "geodb.h"

GeoDatabase::GeoDatabase()
{
    
}
GeoDatabase::~GeoDatabase()
{
    
}

bool GeoDatabase::load(const string& map_data_file) {
    ifstream file(map_data_file);
    if (!file) {
        return false;
    }

    string streetName, line;
    while (getline(file, streetName)) {
        int numSeg = 0;
        file >> numSeg;
        file.ignore(numeric_limits<streamsize>::max(), '\n');

        for (int i = 0; i < numSeg; i++) {
            getline(file, line);
            istringstream iss(line);
            string startLat, startLong, endLat, endLong;
            iss >> startLat >> startLong >> endLat >> endLong;

            GeoPoint start(startLat, startLong), end(endLat, endLong);
            GeoPoint mid = midpoint(start, end);

            StreetSegment forwardSeg(start, mid, streetName), backwardSeg(mid, end, streetName);
            insertOrUpdateSegment(start, forwardSeg);
            insertOrUpdateSegment(mid, forwardSeg);
            insertOrUpdateSegment(mid, backwardSeg);
            insertOrUpdateSegment(end, backwardSeg);
        }
    }
    return true;
}

void GeoDatabase::insertOrUpdateSegment(const GeoPoint& point, const StreetSegment& segment) {
    if (streetSegmentsByLocation.find(point.to_string())) {
        streetSegmentsByLocation[point.to_string()].push_back(segment);
    } else {
        streetSegmentsByLocation.insert(point.to_string(), vector<StreetSegment>{segment});
    }
}

bool GeoDatabase::get_poi_location(const std::string& poi, GeoPoint& point) const
{
    const GeoPoint* foundPoint = poiLocations.find(poi);
    if (foundPoint)
    {
        point = *foundPoint;
        return true;
    }
    return false;
}
std::vector<GeoPoint> GeoDatabase::get_connected_points(const GeoPoint& pt) const
{
    std::vector<GeoPoint> connectedPoints;
    const std::vector<StreetSegment>* segments = streetSegmentsByLocation.find(pt.to_string());
    if (segments) {
        for (const StreetSegment& seg : *segments) {
            if (!(seg.startPoint.to_string() == pt.to_string())) {
                connectedPoints.push_back(seg.startPoint);
            }
            if (!(seg.endPoint.to_string() == pt.to_string())) {
                connectedPoints.push_back(seg.endPoint);
            }
        }
    }
    return connectedPoints;
}
std::string GeoDatabase::get_street_name(const GeoPoint& pt1, const GeoPoint& pt2) const
{
    const std::vector<StreetSegment>* segmentsFromPt1 = streetSegmentsByLocation.find(pt1.to_string());
    if (segmentsFromPt1) {
        for (const StreetSegment& seg : *segmentsFromPt1) {
            if (seg.endPoint.to_string() == pt2.to_string() || seg.startPoint.to_string() == pt2.to_string()) {
                return seg.streetName;
            }
        }
    }
    return "";
}
