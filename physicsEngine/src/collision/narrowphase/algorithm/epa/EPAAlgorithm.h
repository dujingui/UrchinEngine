#ifndef URCHINENGINE_EPAALGORITHM_H
#define URCHINENGINE_EPAALGORITHM_H

#include <vector>
#include <limits>
#include <map>
#include <cmath>
#include <stdexcept>
#include <cassert>
#include <memory>
#include "UrchinCommon.h"

#include "collision/narrowphase/algorithm/epa/EPATriangleData.h"
#include "collision/narrowphase/algorithm/epa/result/EPAResult.h"
#include "collision/narrowphase/algorithm/epa/result/EPAResultCollide.h"
#include "collision/narrowphase/algorithm/epa/result/EPAResultNoCollide.h"
#include "collision/narrowphase/algorithm/epa/result/EPAResultInvalid.h"
#include "collision/narrowphase/algorithm/utils/AlgorithmResultDeleter.h"
#include "object/CollisionConvexObject3D.h"
#include "collision/narrowphase/algorithm/gjk/result/GJKResult.h"

namespace urchin
{

	template<class T> class EPAAlgorithm
	{
		public:
			EPAAlgorithm();

			std::unique_ptr<EPAResult<T>, AlgorithmResultDeleter> processEPA(const CollisionConvexObject3D &, const CollisionConvexObject3D &, const GJKResult<T> &) const;

		private:
			std::unique_ptr<EPAResult<T>, AlgorithmResultDeleter> handleSubTriangle(const CollisionConvexObject3D &, const CollisionConvexObject3D &) const;

			void determineInitialPoints(const Simplex<T> &, const CollisionConvexObject3D &, const CollisionConvexObject3D &,
					std::map<std::size_t, ConvexHullPoint<T>> &, std::map<std::size_t, Point3<T>> &, std::map<std::size_t, Point3<T>> &) const;
			void determineInitialTriangles(std::map<std::size_t, ConvexHullPoint<T>> &, std::map<std::size_t, IndexedTriangle3D<T>> &) const;

			typename std::map<std::size_t, EPATriangleData<T>>::const_iterator getClosestTriangleData(const typename std::map<std::size_t, EPATriangleData<T>> &) const;
            EPATriangleData<T> createTriangleData(const ConvexHullShape3D<T> &, std::size_t) const;

            void logInputData(const std::string &, const CollisionConvexObject3D &, const CollisionConvexObject3D &, const GJKResult<T> &) const;

			const unsigned int maxIteration;
			const float terminationTolerance;
	};

}

#endif
