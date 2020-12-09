#pragma once

#include "EarthMath.h"
#include "dimension2d.h"

namespace Earth
{
namespace core
{
	template <class T>
	class vector2d
	{
	public:
		vector2d() : X(0), Y(0) {}
		vector2d(T nx, T ny) : X(nx), Y(ny) {}
		explicit vector2d(T n) : X(n), Y(n) {} // explicit : 묵시적 형변환은 허용하지 않는다. 반드시 명시적 형변환만 해야한다.
		vector2d(const vector2d<T>& other) : X(other.X), Y(other.Y) {}
		vector2d(const dimension2d<T>& other) : X(other.Width), Y(other.Height) {}

		// vector2d 에 대한 -,+,+=,-=,*,*=,/,/= etc(등)
		// 사칙연산에 대해 재 정의 한다.
		// 연산자 재정의
		vector2d<T> operator-() const { return vector2d<T>(-X, -Y); }

		vector2d<T>& operator=(const vector2d<T>& other) { X = other.X; Y = other.Y; return *this; };
		vector2d<T>& operator=(const dimension2d<T>& other) { X = other.Width; Y = other.Height; return *this; }
			
		vector2d<T> operator+(const vector2d<T>& other) const { return vector2d<T>(X + other.X, Y + other.Y); }
		vector2d<T> operator+(const dimension2d<T>& other) const { return vector2d<T>(X + other.Width, Y + other.Height); }
		vector2d<T>& operator+=(const vector2d<T>& other) { X += other.X; Y += other.Y; return *this; }
		vector2d<T> operator+(const T v) const { return vector2d<T>(X + v, Y + v); }
		vector2d<T>& operator+=(const T v) { X += v; Y += v; return *this; }
		vector2d<T>& operator+=(const dimension2d<T>& other) { X += other.Width; Y += other.Height; return *this; }

		vector2d<T> operator-(const vector2d<T>& other) const { return vector2d<T>(X - other.X, Y - other.Y); }
		vector2d<T> operator-(const dimension2d<T>& other) const { return vector2d<T>(X - other.Width, Y - other.Height); }
		vector2d<T>& operator-=(const vector2d<T>& other) { X -= other.X; Y -= other.Y; return *this; }
		vector2d<T> operator-(const T v) const { return vector2d<T>(X - v, Y - v); }
		vector2d<T>& operator-=(const T v) { X -= v; Y -= v; return *this; }
		vector2d<T>& operator-=(const dimension2d<T>& other) { X -= other.Width; Y -= other.Height; return *this; }

		vector2d<T> operator*(const vector2d<T>& other) const { return vector2d<T>(X * other.X, Y * other.Y); }
		vector2d<T>& operator*=(const vector2d<T>& other) { X *= other.X; Y *= other.Y; return *this; }
		vector2d<T> operator*(const T v) const { return vector2d<T>(X * v, Y * v); }
		vector2d<T>& operator*=(const T v) { X *= v; Y *= v; return *this; }

		vector2d<T> operator/(const vector2d<T>& other) const { return vector2d<T>(X / other.X, Y / other.Y); }
		vector2d<T>& operator/=(const vector2d<T>& other) { X /= other.X; Y /= other.Y; return *this; }
		vector2d<T> operator/(const T v) const { return vector2d<T>(X / v, Y / v); }
		vector2d<T>& operator/=(const T v) { X /= v; Y /= v; return *this; }

		// 크기 비교는 X 를 기준으로 하고, X 가 같다면, Y 를 기준으로 비교한다.
		//! sort in order X, Y. Equality with rounding tolerance.
		bool operator<=(const vector2d<T>& other) const
		{
			return 	(X < other.X || core::equals(X, other.X)) ||
				(core::equals(X, other.X) && (Y < other.Y || core::equals(Y, other.Y)));
		}

		// 크기 비교는 X 를 기준으로 하고, X 가 같다면, Y 를 기준으로 비교한다.
		//! sort in order X, Y. Equality with rounding tolerance.
		bool operator>=(const vector2d<T>& other) const
		{
			return 	(X > other.X || core::equals(X, other.X)) ||
				(core::equals(X, other.X) && (Y > other.Y || core::equals(Y, other.Y)));
		}

		// 크기 비교는 X 를 기준으로 하고, X 가 같다면, Y 를 기준으로 비교한다.
		//! sort in order X, Y. Difference must be above rounding tolerance.
		bool operator<(const vector2d<T>& other) const
		{
			return 	(X < other.X && !core::equals(X, other.X)) ||
				(core::equals(X, other.X) && Y < other.Y && !core::equals(Y, other.Y));
		}

		// 크기 비교는 X 를 기준으로 하고, X 가 같다면, Y 를 기준으로 비교한다.
		//! sort in order X, Y. Difference must be above rounding tolerance.
		bool operator>(const vector2d<T>& other) const
		{
			return 	(X > other.X && !core::equals(X, other.X)) ||
				(core::equals(X, other.X) && Y > other.Y && !core::equals(Y, other.Y));
		}

		bool operator==(const vector2d<T>& other) const { return equals(other); }
		bool operator!=(const vector2d<T>& other) const { return !equals(other); }

			
		bool equals(const vector2d<T>& other) const
		{
			return core::equals(X, other.X) && core::equals(Y, other.Y);
		}

		vector2d<T>& set(T nx, T ny) { X = nx; Y = ny; return *this; }
		vector2d<T>& set(const vector2d<T>& p) { X = p.X; Y = p.Y; return *this; }

		// 길이를 얻는다.
		T getLength() const { return core::squareroot(X * X + Y * Y); }

		// 굳이 길이를 구하지 않아도 되는 경우, squareroot 연산을 하지 않는다.
		// ex) 두벡터중 어느것이 원점에서 먼지 확인할 때 굳이 길이를 구하지 않아도 됨
		// squareroot (제곱근 구하는 식) 은 비용이 많이 들기 때문에...
		T getLengthSQ() const { return X * X + Y * Y; }

		// 스칼라 곱(점곱)
		T dotProduct(const vector2d<T>& other) const
		{
			return X * other.X + Y * other.Y;
		}

		T getDistanceFrom(const vector2d<T>& other) const
		{
			return vector2d<T>(X - other.X, Y - other.Y).getLength();
		}

		T getDistanceFromSQ(const vector2d<T>& other) const
		{
			return vector2d<T>(X - other.X, Y - other.Y).getLengthSQ();
		}

		// centor 를 중심으로 degrees 만큼 회전한다.
		vector2d<T>& rotateBy(f64 degrees, const vector2d<T>& center = vector2d<T>())
		{
			degrees *= DEGTORAD64;
			const f64 cs = cos(degrees);
			const f64 sn = sin(degrees);

			X -= center.X;
			Y -= center.Y;

			set((T)(X * cs - Y * sn), (T)(X * sn + Y * cs));

			X += center.X;
			Y += center.Y;
			return *this;
		}

		// 벡터를 정규화 한다.(길이가 1인 단위벡터로 만듬)
		vector2d<T>& normalize()
		{
			f32 length = (f32)(X * X + Y * Y);
			if (length == 0)
				return *this;
			length = core::reciprocal_squareroot(length);
			X = (T)(X * length);
			Y = (T)(Y * length);
			return *this;
		}

		// 이 벡터의 각도를 계산한다.
		// 0 도는 오른쪽 즉, 3시 방향을 의미하며, 시계반대방향으로 값이 증가한다.
		// 0 ~ 360 사이의 값을 반환한다.
		f64 getAngleTrig() const
		{
			if (Y == 0)
				return X < 0 ? 180 : 0;
			else
				if (X == 0)
					return Y < 0 ? 270 : 90;

			if (Y > 0)
				if (X > 0)
					return atan((Earth::f64)Y / (Earth::f64)X) * RADTODEG64;
				else
					return 180.0 - atan((Earth::f64)Y / -(Earth::f64)X) * RADTODEG64;
			else
				if (X > 0)
					return 360.0 - atan(-(Earth::f64)Y / (Earth::f64)X) * RADTODEG64;
				else
					return 180.0 + atan(-(Earth::f64)Y / -(Earth::f64)X) * RADTODEG64;
		}

		// 0 도는 오른쪽(3시)를 의미하며, 시계방향으로 값이 증가한다.
		// 0 ~ 360 사이의 값을 반환한다.
		inline f64 getAngle() const
		{
			if (Y == 0)
				return X < 0 ? 180 : 0;
			else if (X == 0)
				return Y < 0 ? 90 : 270;

			const f64 tmp = core::clamp(Y / sqrt((f64)(X * X + Y * Y)), -1.0, 1.0);
			const f64 angle = atan(core::squareroot(1 - tmp * tmp) / tmp) * RADTODEG64;

			if (X > 0 && Y > 0)
				return angle + 270;
			else
				if (X > 0 && Y < 0)
					return angle + 90;
				else
					if (X < 0 && Y < 0)
						return 90 - angle;
					else
						if (X < 0 && Y>0)
							return 270 - angle;

			return angle;
		}

		// 다른 벡터와 이루는 각을 반환한다.
		// 0 ~ 90 사이의 값을 반환한다.
		inline f64 getAngleWith(const vector2d<T>& b) const
		{
			f64 tmp = (f64)(X * b.X + Y * b.Y);

			if (tmp == 0.0)
				return 90.0;

			tmp = tmp / core::squareroot((f64)((X * X + Y * Y) * (b.X * b.X + b.Y * b.Y)));
			if (tmp < 0.0)
				tmp = -tmp;
			if (tmp > 1.0)
				tmp = 1.0;

			return atan(sqrt(1 - tmp * tmp) / tmp) * RADTODEG64;
		}

		// 크기 비교는 X 를 기준으로 하고, X 가 같다면, Y 를 기준으로 비교한다.
		// 현재 벡터가 두 벡터 사이의 값인지 확인한다.
		// 두벡터 사이의 값이면 true
		// 그렇치 않으면 false
		bool isBetweenPoints(const vector2d<T>& begin, const vector2d<T>& end) const
		{
			if (begin.X != end.X)
			{
				return ((begin.X <= X && X <= end.X) ||
					(begin.X >= X && X >= end.X));
			}
			else
			{
				return ((begin.Y <= Y && Y <= end.Y) ||
					(begin.Y >= Y && Y >= end.Y));
			}
		}

		vector2d<T> getInterpolated(const vector2d<T>& other, f64 d) const
		{
			f64 inv = 1.0f - d;
			return vector2d<T>((T)(other.X * inv + X * d), (T)(other.Y * inv + Y * d));
		}

		// a <- b 사이의 벡터를 구함
		// d 가 0 이면 b
		// d 가 1 이면 a
		vector2d<T>& interpolate(const vector2d<T>& a, const vector2d<T>& b, f64 d)
		{
			X = (T)((f64)b.X + ((a.X - b.X) * d));
			Y = (T)((f64)b.Y + ((a.Y - b.Y) * d));
			return *this;
		}
			
		T X;
		T Y;
	}; // end class vector2d<T>
		
	typedef vector2d<f32> vector2df;
	typedef vector2d<s32> vector2di;

	// 벡터의 스칼라 곱에 대해 재 정의 한다.
	template<class S, class T>
	vector2d<T> operator*(const S scalar, const vector2d<T>& vector) { return vector * scalar; }

	template<class T>
	dimension2d<T>::dimension2d(const vector2d<T>& other) : Width(other.X), Height(other.Y) { }

	template<class T>
	bool dimension2d<T>::operator==(const vector2d<T>& other) const { return Width == other.X && Height == other.Y; }

} // end namespace core
}// end namespace Earth