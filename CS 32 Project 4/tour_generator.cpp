#include "tour_generator.h"

TourGenerator::TourGenerator(const GeoDatabaseBase& geodb, const RouterBase& router) : geodatabase(geodb), routerbase(router)
{
    
}

TourGenerator::~TourGenerator()
{
    
}

std::vector<TourCommand> TourGenerator::generate_tour(const Stops& stops) const
{
    std::vector<TourCommand> tourCommands;
    std::string poi, description;
    GeoPoint previousPoint, currentPoint;

    for (int i = 0; i < stops.size(); ++i) {
        if (!stops.get_poi_data(i, poi, description)) {
            return {};
        }

        if (!geodatabase.get_poi_location(poi, currentPoint)) {
            return {};
        }

        TourCommand commentaryCmd;
        commentaryCmd.init_commentary(poi, description);
        tourCommands.push_back(commentaryCmd);

        if (i > 0) {
            std::vector<GeoPoint> route = routerbase.route(previousPoint, currentPoint);
            if (route.empty()) {
                return {};
            }

            for (size_t j = 1; j < route.size(); ++j) {
                double angle = angle_of_line(route[j-1], route[j]);
                std::string direction = calculateDirection(angle);
                double distance = distance_earth_miles(route[j-1], route[j]);

                TourCommand proceedCmd;
                proceedCmd.init_proceed(direction, "a path", distance, route[j-1], route[j]);
                tourCommands.push_back(proceedCmd);

                if (j < route.size() - 1) {
                    double turnAngle = angle_of_turn(route[j-1], route[j], route[j+1]);
                    if (turnAngle >= 1 && turnAngle < 180) {
                        TourCommand turnCmd;
                        turnCmd.init_turn("left", route[j+1].to_string());
                        tourCommands.push_back(turnCmd);
                    } else if (turnAngle >= 180 && turnAngle <= 359) {
                        TourCommand turnCmd;
                        turnCmd.init_turn("right", route[j+1].to_string());
                        tourCommands.push_back(turnCmd);
                    }
                }
            }
        }

        previousPoint = currentPoint;
    }

    return tourCommands;
}
