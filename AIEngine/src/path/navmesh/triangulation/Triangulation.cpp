#include <stdexcept>
#include <stack>
#include <algorithm>

#include "Triangulation.h"
#include "path/navmesh/triangulation/MonotonePolygon.h"

namespace urchin
{

	/**
	 * @param ccwPolygonPoints Polygon points in counter clockwise order. Points must be unique.
	 */
	Triangulation::Triangulation(const std::vector<Point2<float>> &ccwPolygonPoints) :
			polygonPoints(ccwPolygonPoints)
	{
		endContourIndices.push_back(ccwPolygonPoints.size());
	}

	/**
	 * @return Polygon points in counter clockwise order.
	 */
	std::vector<Point2<float>> Triangulation::getPolygonPoints() const
	{
		return std::vector<Point2<float>>(polygonPoints.begin(), polygonPoints.begin() + endContourIndices[0]);
	}

	/**
	 * @param cwHolePoints Hole points in clockwise order. Points must be unique and not go outside the polygon contour.
	 * @return Hole index (start to 0).
	 */
	unsigned int Triangulation::addHolePoints(const std::vector<Point2<float>> &cwHolePoints)
	{
		polygonPoints.insert(polygonPoints.end(), cwHolePoints.begin(), cwHolePoints.end());
		endContourIndices.push_back(polygonPoints.size());

		return endContourIndices.size() - 2;
	}

	/**
	 * @return Number of holes
	 */
	unsigned int Triangulation::getHolesSize() const
	{
		return endContourIndices.size() - 1;
	}

	/**
	 * @return Hole points in clockwise order.
	 */
	std::vector<Point2<float>> Triangulation::getHolePoints(unsigned int holeIndex) const
	{
		return std::vector<Point2<float>>(polygonPoints.begin() + endContourIndices[holeIndex], polygonPoints.begin() + endContourIndices[holeIndex+1]);
	}

	std::vector<IndexedTriangle2D<float>> Triangulation::triangulate() const
	{ //based on "Computational Geometry - Algorithms and Applications, 3rd Ed" - "Polygon Triangulation"
		MonotonePolygon monotonePolygon(polygonPoints, endContourIndices);
		std::vector<std::vector<unsigned int>> monotonePolygons = monotonePolygon.createYMonotonePolygons();

		std::vector<IndexedTriangle2D<float>> triangles;
		triangles.reserve((polygonPoints.size()-2) + (2*getHolesSize()));

		for(unsigned monotonePolygonIndex = 0; monotonePolygonIndex<monotonePolygons.size(); ++monotonePolygonIndex)
		{
			#ifdef _DEBUG
				std::vector<IndexedTriangle2D<float>> monotonePolygonTriangles;
				monotonePolygonTriangles.reserve(monotonePolygons[monotonePolygonIndex].size());
				triangulateMonotonePolygon(monotonePolygons[monotonePolygonIndex], monotonePolygonTriangles);
				triangles.insert(triangles.end(), monotonePolygonTriangles.begin(), monotonePolygonTriangles.end());
				//logOutputData("Debug monotone polygon " + std::to_string(monotonePolygonIndex) + " triangulation.", monotonePolygonTriangles, Logger::INFO);
			#else
				triangulateMonotonePolygon(monotonePolygons[monotonePolygonIndex], triangles);
			#endif
		}

		return triangles;
	}

	/**
	 * Return points size for all points: point of main polygon + points of holes
	 */
	unsigned int Triangulation::getAllPointsSize() const
	{
		return polygonPoints.size();
	}

	/**
	 * @param triangles [out] Triangles of monotone polygon are added to this vector
	 */
	void Triangulation::triangulateMonotonePolygon(const std::vector<unsigned int> &monotonePolygonPoints, std::vector<IndexedTriangle2D<float>> &triangles) const
	{
		std::vector<SidedPoint> sortedSidedPoints = buildSortedSidedPoints(monotonePolygonPoints);

		std::stack<SidedPoint> stack;
		stack.push(sortedSidedPoints[0]);
		stack.push(sortedSidedPoints[1]);

		for(unsigned int j=2; j<sortedSidedPoints.size()-1; ++j)
		{
			SidedPoint currentPoint = sortedSidedPoints[j];

			if(currentPoint.onLeft != stack.top().onLeft)
			{
				while(stack.size() > 1)
				{
					SidedPoint topPoint = stack.top();
					stack.pop();
					SidedPoint top2Point = stack.top();

					triangles.push_back(IndexedTriangle2D<float>(currentPoint.pointIndex, topPoint.pointIndex, top2Point.pointIndex));
				}
				stack.pop();
				stack.push(sortedSidedPoints[j-1]);
				stack.push(currentPoint);
			}else
			{
				while(stack.size() > 1)
				{
					SidedPoint topPoint = stack.top();
					stack.pop();
					SidedPoint top2Point = stack.top();
					stack.push(topPoint);

					Vector2<float> diagonalVector = polygonPoints[currentPoint.pointIndex].vector(polygonPoints[top2Point.pointIndex]);
					Vector2<float> stackVector = polygonPoints[topPoint.pointIndex].vector(polygonPoints[top2Point.pointIndex]);
					float orientationResult = diagonalVector.crossProduct(stackVector); //note: orientation could be 0.0 if currentPoint.pointIndex and topPoint.pointIndex are very close due to float imprecision

					if((orientationResult <= 0.0 && topPoint.onLeft) || (orientationResult >= 0.0 && !topPoint.onLeft))
					{
						triangles.push_back(IndexedTriangle2D<float>(currentPoint.pointIndex, top2Point.pointIndex, topPoint.pointIndex));
						stack.pop();
					}else
					{
						break;
					}
				}

				stack.push(currentPoint);
			}
		}

		SidedPoint currentPoint = sortedSidedPoints[sortedSidedPoints.size()-1];
		while(stack.size() > 1)
		{
			SidedPoint topPoint = stack.top();
			stack.pop();
			SidedPoint top2Point = stack.top();

			triangles.push_back(IndexedTriangle2D<float>(currentPoint.pointIndex, top2Point.pointIndex, topPoint.pointIndex));
		}
	}

	std::vector<SidedPoint> Triangulation::buildSortedSidedPoints(const std::vector<unsigned int> &monotonePolygonPoints) const
	{
		std::vector<SidedPoint> sortedSidedPoints;
		sortedSidedPoints.reserve(monotonePolygonPoints.size());

		for(unsigned int i=0; i<monotonePolygonPoints.size(); ++i)
		{
			SidedPoint sidedPoint;

			unsigned int currentIndex = monotonePolygonPoints[i];
			sidedPoint.pointIndex = currentIndex;

			unsigned int nextIndex = monotonePolygonPoints[(i+1)%monotonePolygonPoints.size()];
			sidedPoint.onLeft = isFirstPointAboveSecond(currentIndex, nextIndex);

			sortedSidedPoints.push_back(sidedPoint);
		}

		std::sort(sortedSidedPoints.begin(), sortedSidedPoints.end(), [&](const SidedPoint &left, const SidedPoint &right)
				{return isFirstPointAboveSecond(left.pointIndex, right.pointIndex);});

		return sortedSidedPoints;
	}

	bool Triangulation::isFirstPointAboveSecond(unsigned int firstIndex, unsigned int secondIndex) const
	{
		if(polygonPoints[firstIndex].Y == polygonPoints[secondIndex].Y)
		{
			return polygonPoints[firstIndex].X < polygonPoints[secondIndex].X;
		}
		return polygonPoints[firstIndex].Y > polygonPoints[secondIndex].Y;
	}

	void Triangulation::logOutputData(const std::string &message, const std::vector<IndexedTriangle2D<float>> &triangles, Logger::CriticalityLevel logLevel) const
	{
		std::stringstream logStream;
		logStream.precision(std::numeric_limits<float>::max_digits10);

		logStream<<message<<std::endl;
		logStream<<"Monotone polygon triangles output data:"<<std::endl;
		for(const auto &triangle : triangles)
		{
			logStream<<" - {"<<polygonPoints[triangle.getIndex(0)]<<"}, {"<<polygonPoints[triangle.getIndex(1)]<<"}, {"<<polygonPoints[triangle.getIndex(2)]<<"}"<<std::endl;
		}
		Logger::logger().log(logLevel, logStream.str());
	}
}