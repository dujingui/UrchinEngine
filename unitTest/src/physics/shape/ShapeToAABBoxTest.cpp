#include <cppunit/TestSuite.h>
#include <cppunit/TestCaller.h>
#include <memory>
#include "UrchinCommon.h"
#include "UrchinPhysicsEngine.h"

#include "AssertHelper.h"
#include "physics/shape/ShapeToAABBoxTest.h"
using namespace urchin;

void ShapeToAABBoxTest::boxConversion()
{
	CollisionBoxShape collisionBox(Vector3<float>(1.0, 2.0, 1.0)); //box 1x2x1
	PhysicsTransform transform(urchin::Point3<float>(0.0, 0.0, 0.0), //move 0 unit on X, Y and Z axis
			urchin::Quaternion<float>(urchin::Vector3<float>(0.0, 0.0, 1.0), 3.14159265358979/4)); //rotate 45° on Z axis

	AABBox<float> box = collisionBox.toAABBox(transform);

	AssertHelper::assertFloatEquals(box.getHalfSize(0), 2.12132034356);
	AssertHelper::assertFloatEquals(box.getHalfSize(1), 2.12132034356);
	AssertHelper::assertFloatEquals(box.getHalfSize(2), 1.0);
	AssertHelper::assertPoint3FloatEquals(box.getMin(), Point3<float>(-2.12132034356, -2.12132034356, -1.0));
	AssertHelper::assertPoint3FloatEquals(box.getMax(), Point3<float>(2.12132034356, 2.12132034356, 1.0));
}

void ShapeToAABBoxTest::convexHullConversion()
{
	Point3<float> boxPointsTab[] = {
			Point3<float>(-1.0, -2.0, 1.0), Point3<float>(1.0, -2.0, 1.0), Point3<float>(1.0, 2.0, 1.0), Point3<float>(-1.0, 2.0, 1.0),
			Point3<float>(-1.0, -2.0, -1.0), Point3<float>(1.0, -2.0, -1.0), Point3<float>(1.0, 2.0, -1.0), Point3<float>(-1.0, 2.0, -1.0),
	};
	std::vector<Point3<float>> boxPoints(boxPointsTab, boxPointsTab+sizeof(boxPointsTab)/sizeof(Point3<float>));

	CollisionConvexHullShape collisionConvexHull(0.0, boxPoints);
	PhysicsTransform transform(urchin::Point3<float>(0.0, 0.0, 0.0), //move 0 unit on X, Y and Z axis
			urchin::Quaternion<float>(urchin::Vector3<float>(0.0, 0.0, 1.0), -3.14159265358979/4)); //rotate 45° on Z axis

	AABBox<float> box = collisionConvexHull.toAABBox(transform);

	AssertHelper::assertFloatEquals(box.getHalfSize(0), 2.12132034356);
	AssertHelper::assertFloatEquals(box.getHalfSize(1), 2.12132034356);
	AssertHelper::assertFloatEquals(box.getHalfSize(2), 1.0);
	AssertHelper::assertPoint3FloatEquals(box.getMin(), Point3<float>(-2.12132034356, -2.12132034356, -1.0));
	AssertHelper::assertPoint3FloatEquals(box.getMax(), Point3<float>(2.12132034356, 2.12132034356, 1.0));
}

CppUnit::Test *ShapeToAABBoxTest::suite()
{
	CppUnit::TestSuite *suite = new CppUnit::TestSuite("ShapeToAABBoxTest");

	suite->addTest(new CppUnit::TestCaller<ShapeToAABBoxTest>("boxConversion", &ShapeToAABBoxTest::boxConversion));
	suite->addTest(new CppUnit::TestCaller<ShapeToAABBoxTest>("convexHullConversion", &ShapeToAABBoxTest::convexHullConversion));

	return suite;
}
