#include "math/algebra/vector/Vector2.h"

namespace urchin
{
	
	template<class T> Vector2<T>::Vector2() : X(0), Y(0)
	{

	}

	template<class T> Vector2<T>::Vector2(T Xu, T Yu) : X(Xu), Y(Yu)
	{

	}

	template<class T> Vector2<T>::Vector2(const Vector2<T> &vector) :
		X(vector.X), Y(vector.Y)
	{

	}

	template<class T> void Vector2<T>::setValues(T Xu, T Yu)
	{
		X = Xu;
		Y = Yu;
	}

	template<class T> void Vector2<T>::setNull()
	{
		X = Y = 0;
	}

	template<class T> Vector2<T> Vector2<T>::normalize() const
	{
		const T norm = (T)sqrt(X*X + Y*Y);

		if(norm > 0.0)
		{
			return Vector2<T>(X/norm, Y/norm);
		}

		return Vector2<T>(X, Y);
	}

	template<class T> T Vector2<T>::length() const
	{
		return (T)sqrt(X*X + Y*Y);
	}

	template<class T> T Vector2<T>::squareLength() const
	{
		return (X*X + Y*Y);
	}

	template<class T> T Vector2<T>::dotProduct(const Vector2<T> &v) const
	{
		return (X*v.X + Y*v.Y);
	}

	template<class T> Vector2<T> Vector2<T>::crossProduct(const Vector2<T> &v) const
	{
		return Vector2<T>(	Y*v.X - X*v.Y,
					X*v.Y - Y*v.X);
	}

	template<class T> Vector2<T> Vector2<T>::operator +() const
	{
		return Vector2<T>(X, Y);
	}

	template<class T> Vector2<T> Vector2<T>::operator -() const
	{
		return Vector2<T>(-X, -Y);
	}

	template<class T> Vector2<T> Vector2<T>::operator +(const Vector2<T> &v) const
	{
		return Vector2<T>(	X + v.X,
					Y + v.Y);
	}

	template<class T> Vector2<T> Vector2<T>::operator -(const Vector2<T> &v) const
	{
		return Vector2<T>(	X - v.X,
					Y - v.Y);
	}

	template<class T> Vector2<T> Vector2<T>::operator *(const Vector2<T> &v) const
	{
		return Vector2<T>(	X * v.X,
				Y * v.Y);
	}

	template<class T> Vector2<T> Vector2<T>::operator /(const Vector2<T> &v) const
	{
		return Vector2<T>(	X / v.X,
				Y / v.Y);
	}

	template<class T> const Vector2<T>& Vector2<T>::operator +=(const Vector2<T> &v)
	{
		X += v.X;
		Y += v.Y;

		return *this;
	}

	template<class T> const Vector2<T>& Vector2<T>::operator -=(const Vector2<T> &v)
	{
		X -= v.X;
		Y -= v.Y;

		return *this;
	}

	template<class T> const Vector2<T>& Vector2<T>::operator *=(const Vector2<T> &v)
	{
		X *= v.X;
		Y *= v.Y;

		return *this;
	}

	template<class T> const Vector2<T>& Vector2<T>::operator /=(const Vector2<T> &v)
	{
		X /= v.X;
		Y /= v.Y;

		return *this;
	}

	template<class T> const Vector2<T>& Vector2<T>::operator *=(T t)
	{
		X *= t;
		Y *= t;

		return *this;
	}

	template<class T> const Vector2<T>& Vector2<T>::operator /=(T t)
	{
		X /= t;
		Y /= t;

		return *this;
	}

	template<class T> T& Vector2<T>::operator [](int i)
	{
		return (&X)[i];
	}

	template<class T> const T& Vector2<T>::operator [](int i) const
	{
		return (&X)[i];
	}

	template<class T> Vector2<T>::operator T*()
	{
		return &X;
	}

	template<class T> Vector2<T>::operator const T*() const
	{
		return &X;
	}

	template<class T> Vector2<T> operator *(const Vector2<T> &v, T t)
	{
		return Vector2<T>(v.X * t, v.Y * t);
	}

	template<class T> Vector2<T> operator *(T t, const Vector2<T> &v)
	{
		return v * t;
	}

	template<class T> Vector2<T> operator /(const Vector2<T> &v, T t)
	{
		return Vector2<T>(v.X / t, v.Y / t);
	}

	template<class T> Vector2<T> operator *(const Matrix2<T> &m, const Vector2<T> &v)
	{
		return Vector2<T>(	m.a11 * v.X + m.a12 * v.Y,
					m.a21 * v.X + m.a22 * v.Y);
	}

	template<class T> Vector2<T> operator *(const Vector2<T> &v, const Matrix2<T> &m)
	{
		return m * v;
	}

	template<class T> std::ostream& operator <<(std::ostream &stream, const Vector2<T> &v)
	{
		return stream << v.X << " " << v.Y;
	}

	//explicit template
	template class Vector2<float>;
	template Vector2<float> operator *<float>(const Vector2<float> &, float);
	template Vector2<float> operator *<float>(float, const Vector2<float> &);
	template Vector2<float> operator /<float>(const Vector2<float> &, float);
	template Vector2<float> operator *<float>(const Matrix2<float> &, const Vector2<float> &);
	template Vector2<float> operator *<float>(const Vector2<float> &, const Matrix2<float> &);
	template std::ostream& operator <<<float>(std::ostream &, const Vector2<float> &);

	template class Vector2<int>;
	template Vector2<int> operator *<int>(const Vector2<int> &, int);
	template Vector2<int> operator *<int>(int, const Vector2<int> &);
	template Vector2<int> operator /<int>(const Vector2<int> &, int);
	template Vector2<int> operator *<int>(const Matrix2<int> &, const Vector2<int> &);
	template Vector2<int> operator *<int>(const Vector2<int> &, const Matrix2<int> &);
	template std::ostream& operator <<<int>(std::ostream &, const Vector2<int> &);

}
