#ifndef ENGINE_POLYGONSINTERSECTIONTEST_H
#define ENGINE_POLYGONSINTERSECTIONTEST_H

#include <cppunit/TestFixture.h>
#include <cppunit/Test.h>

class PolygonsIntersectionTest : public CppUnit::TestFixture
{
	public:
		static CppUnit::Test *suite();

		void subjectCoverClipper();
		void clipperCoverSubject();

		void subjectClippedByBox();
};

#endif
