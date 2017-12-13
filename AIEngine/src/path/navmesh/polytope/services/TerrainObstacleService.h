#ifndef URCHINENGINE_TERRAINOBSTACLESERVICE_H
#define URCHINENGINE_TERRAINOBSTACLESERVICE_H

#include "UrchinCommon.h"

#include "path/navmesh/model/NavMeshConfig.h"
#include "path/navmesh/csg/CSGPolygon.h"

namespace urchin
{

    class TerrainObstacleService
    {
        public:
            TerrainObstacleService(const Point3<float> &, const std::vector<Point3<float>> &, unsigned int, unsigned int);

            std::vector<CSGPolygon<float>> computeSelfObstacles(float);

        private:
            const Point3<float> &position;
            const std::vector<Point3<float>> &localVertices;
            unsigned int xLength;
            unsigned int zLength;

            enum EdgeDirection
            {
                LEFT,
                RIGHT,
                TOP,
                BOTTOM
            };

            bool isWalkableSquare(unsigned int, float) const;
            float computeTriangleSlope(const std::vector<Point3<float>> &) const;

            std::vector<unsigned int> findAllInaccessibleNeighbors(unsigned int, float) const;
            std::vector<unsigned int> retrieveNeighbors(unsigned int) const;

            CSGPolygon<float> squaresToPolygon(const std::vector<unsigned int> &) const;
            unsigned int retrieveNextPointIndex(unsigned int, const std::vector<EdgeDirection> &, const std::vector<unsigned int> &, EdgeDirection &) const;
            int nextPointInDirection(unsigned int, EdgeDirection) const;
            bool pointExistInSquares(unsigned int, const std::vector<unsigned int> &) const;

            CSGPolygon<float> pointIndicesToPolygon(const std::vector<unsigned int> &) const;
    };

}

#endif