#include "TriangleShape3D.h"

namespace urchin
{

	template<class T> TriangleShape3D<T>::TriangleShape3D(const Point3<T> *points)
	{
		this->points[0] = points[0];
		this->points[1] = points[1];
		this->points[2] = points[2];
	}

	template<class T> TriangleShape3D<T>::TriangleShape3D(const Point3<T> &point1, const Point3<T> &point2, const Point3<T> &point3)
	{
		this->points[0] = point1;
		this->points[1] = point2;
		this->points[2] = point3;
	}

	template<class T> const Point3<T> *TriangleShape3D<T>::getPoints() const
	{
		return points;
	}

	//explicit template
	template class TriangleShape3D<float>;
	template class TriangleShape3D<double>;

}