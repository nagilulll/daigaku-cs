#include "router.h"

struct GeoPointHash {
    size_t operator()(const GeoPoint& point) const {
        return std::hash<std::string>()(point.sLatitude + point.sLongitude);
    }
};

struct QueueItem {
    GeoPoint point;
    double priority;

    QueueItem(const GeoPoint& pt, double p) : point(pt), priority(p) {}

    bool operator>(const QueueItem& other) const {
        return priority > other.priority;
    }
};

Router::Router(const GeoDatabaseBase& geo_db) : geodatabase(geo_db) {}

Router::~Router() {}

std::vector<GeoPoint> Router::route(const GeoPoint& pt1, const GeoPoint& pt2) const {
    if (pt1.to_string() == pt2.to_string()) {
        return {pt1};
    }

    std::priority_queue<QueueItem, std::vector<QueueItem>, std::greater<QueueItem>> openSet;
    std::unordered_map<std::string, GeoPoint> cameFrom;
    std::unordered_map<std::string, double> costSoFar;

    openSet.emplace(pt1, 0.0);
    cameFrom[pt1.to_string()] = pt1;
    costSoFar[pt1.to_string()] = 0.0;

    while (!openSet.empty()) {
        GeoPoint current = openSet.top().point;
        openSet.pop();

        if (current.to_string() == pt2.to_string()) {
            break;
        }

        for (const GeoPoint& next : geodatabase.get_connected_points(current)) {
            double newCost = costSoFar[current.to_string()] + 1;
            if (!costSoFar.count(next.to_string()) || newCost < costSoFar[next.to_string()]) {
                costSoFar[next.to_string()] = newCost;
                double priority = newCost;
                openSet.emplace(next, priority);
                cameFrom[next.to_string()] = current;
            }
        }
    }

    std::vector<GeoPoint> path;
    GeoPoint current = pt2;
    while (current.to_string() != pt1.to_string()) {
        path.push_back(current);
        current = cameFrom[current.to_string()];
    }
    path.push_back(pt1);
    std::reverse(path.begin(), path.end());
    return path;
}
