#pragma once

#include "EarthMath.h"

namespace Earth
{
namespace core
{
	template <class T>
	class vector3d
	{
	public:
		vector3d() : X(0), Y(0), Z(0) {}
		vector3d(T nx, T ny, T nz) : X(nx), Y(ny), Z(nz) {}
		explicit vector3d(T n) : X(n), Y(n), Z(n) {} // explicit : 묵시적 형변환은 허용하지 않는다. 반드시 명시적 형변환만 해야한다.
		vector3d(const vector3d<T>& other) : X(other.X), Y(other.Y), Z(other.Z) {}

		// vector3d 에 대한 -,+,+=,-=,*,*=,/,/= etc(등)
		// 사칙연산에 대해 재 정의 한다.
		// 연산자 재정의
		vector3d<T> operator-() const { return vector3d<T>(-X, -Y, -Z); }

		vector3d<T>& operator=(const vector3d<T>& other) { X = other.X; Y = other.Y; Z = other.Z; return *this; }

		vector3d<T> operator+(const vector3d<T>& other) const { return vector3d<T>(X + other.X, Y + other.Y, Z + other.Z); }
		vector3d<T>& operator+=(const vector3d<T>& other) { X += other.X; Y += other.Y; Z += other.Z; return *this; }
		vector3d<T> operator+(const T val) const { return vector3d<T>(X + val, Y + val, Z + val); }
		vector3d<T>& operator+=(const T val) { X += val; Y += val; Z += val; return *this; }

		vector3d<T> operator-(const vector3d<T>& other) const { return vector3d<T>(X - other.X, Y - other.Y, Z - other.Z); }
		vector3d<T>& operator-=(const vector3d<T>& other) { X -= other.X; Y -= other.Y; Z -= other.Z; return *this; }
		vector3d<T> operator-(const T val) const { return vector3d<T>(X - val, Y - val, Z - val); }
		vector3d<T>& operator-=(const T val) { X -= val; Y -= val; Z -= val; return *this; }

		vector3d<T> operator*(const vector3d<T>& other) const { return vector3d<T>(X * other.X, Y * other.Y, Z * other.Z); }
		vector3d<T>& operator*=(const vector3d<T>& other) { X *= other.X; Y *= other.Y; Z *= other.Z; return *this; }
		vector3d<T> operator*(const T v) const { return vector3d<T>(X * v, Y * v, Z * v); }
		vector3d<T>& operator*=(const T v) { X *= v; Y *= v; Z *= v; return *this; }

		vector3d<T> operator/(const vector3d<T>& other) const { return vector3d<T>(X / other.X, Y / other.Y, Z / other.Z); }
		vector3d<T>& operator/=(const vector3d<T>& other) { X /= other.X; Y /= other.Y; Z /= other.Z; return *this; }
		vector3d<T> operator/(const T v) const { T i = (T)1.0 / v; return vector3d<T>(X * i, Y * i, Z * i); }
		vector3d<T>& operator/=(const T v) { T i = (T)1.0 / v; X *= i; Y *= i; Z *= i; return *this; }

		// 크기 비교는 X 를 기준으로 하고, X 가 같다면, Y 를 기준으로 비교한다.
		// Y 도 같으면 Z 를 기준으로 비교한다.
		//! sort in order X, Y, Z. Equality with rounding tolerance.
		bool operator<=(const vector3d<T>& other) const
		{
			return 	(X < other.X || core::equals(X, other.X)) ||
				(core::equals(X, other.X) && (Y < other.Y || core::equals(Y, other.Y))) ||
				(core::equals(X, other.X) && core::equals(Y, other.Y) && (Z < other.Z || core::equals(Z, other.Z)));
		}

		// 크기 비교는 X 를 기준으로 하고, X 가 같다면, Y 를 기준으로 비교한다.
		// Y 도 같으면 Z 를 기준으로 비교한다.
		//! sort in order X, Y, Z. Equality with rounding tolerance.
		bool operator>=(const vector3d<T>& other) const
		{
			return 	(X > other.X || core::equals(X, other.X)) ||
				(core::equals(X, other.X) && (Y > other.Y || core::equals(Y, other.Y))) ||
				(core::equals(X, other.X) && core::equals(Y, other.Y) && (Z > other.Z || core::equals(Z, other.Z)));
		}

		// 크기 비교는 X 를 기준으로 하고, X 가 같다면, Y 를 기준으로 비교한다.
		// Y 도 같으면 Z 를 기준으로 비교한다.
		//! sort in order X, Y, Z. Difference must be above rounding tolerance.
		bool operator<(const vector3d<T>& other) const
		{
			return 	(X < other.X && !core::equals(X, other.X)) ||
				(core::equals(X, other.X) && Y < other.Y && !core::equals(Y, other.Y)) ||
				(core::equals(X, other.X) && core::equals(Y, other.Y) && Z < other.Z && !core::equals(Z, other.Z));
		}

		// 크기 비교는 X 를 기준으로 하고, X 가 같다면, Y 를 기준으로 비교한다.
		// Y 도 같으면 Z 를 기준으로 비교한다.
		//! sort in order X, Y, Z. Difference must be above rounding tolerance.
		bool operator>(const vector3d<T>& other) const
		{
			return 	(X > other.X && !core::equals(X, other.X)) ||
				(core::equals(X, other.X) && Y > other.Y && !core::equals(Y, other.Y)) ||
				(core::equals(X, other.X) && core::equals(Y, other.Y) && Z > other.Z && !core::equals(Z, other.Z));
		}

		bool operator==(const vector3d<T>& other) const
		{
			return this->equals(other);
		}

		bool operator!=(const vector3d<T>& other) const
		{
			return !this->equals(other);
		}

		bool equals(const vector3d<T>& other, const T tolerance = (T)ROUNDING_ERROR_f32) const
		{
			return core::equals(X, other.X, tolerance) &&
				core::equals(Y, other.Y, tolerance) &&
				core::equals(Z, other.Z, tolerance);
		}

		vector3d<T>& set(const T nx, const T ny, const T nz) { X = nx; Y = ny; Z = nz; return *this; }
		vector3d<T>& set(const vector3d<T>& p) { X = p.X; Y = p.Y; Z = p.Z; return *this; }

		// 길이를 얻는다.
		T getLength() const { return core::squareroot(X * X + Y * Y + Z * Z); }

		// 굳이 길이를 구하지 않아도 되는 경우, squareroot 연산을 하지 않는다.
		// ex) 두벡터중 어느것이 원점에서 먼지 확인할 때 굳이 길이를 구하지 않아도 됨
		// squareroot (제곱근 구하는 식) 은 비용이 많이 들기 때문에...
		T getLengthSQ() const { return X * X + Y * Y + Z * Z; }

		// 스칼라 곱(점곱)
		T dotProduct(const vector3d<T>& other) const
		{
			return X * other.X + Y * other.Y + Z * other.Z;
		}

		T getDistanceFrom(const vector3d<T>& other) const
		{
			return vector3d<T>(X - other.X, Y - other.Y, Z - other.Z).getLength();
		}
		
		T getDistanceFromSQ(const vector3d<T>& other) const
		{
			return vector3d<T>(X - other.X, Y - other.Y, Z - other.Z).getLengthSQ();
		}

		// 가위 곱(크로스 프로덕트)
		// 두 벡터의 모두 수직인 벡터가 구해진다.
		vector3d<T> crossProduct(const vector3d<T>& p) const
		{
			return vector3d<T>(Y * p.Z - Z * p.Y, Z * p.X - X * p.Z, X * p.Y - Y * p.X);
		}

		// 현재 벡터가 두 벡터 사이의 값인지 확인한다.
		// 두벡터 사이의 값이면 true
		// 그렇치 않으면 false
		bool isBetweenPoints(const vector3d<T>& begin, const vector3d<T>& end) const
		{
			const T f = (end - begin).getLengthSQ();
			return getDistanceFromSQ(begin) <= f &&
				getDistanceFromSQ(end) <= f;
		}

		// 벡터를 정규화 하다
		// 크기가 1인 단위 벡터를 만든다.
		vector3d<T>& normalize()
		{
			f64 length = X * X + Y * Y + Z * Z;
			if (length == 0) // 만약 길이가 0 이면 제곱근 구하는 공식에서 NAN문제(0으로 나눔)가 발생하므로 더이상 계산 하지 않는다.
				return *this;
			length = core::reciprocal_squareroot(length);

			X = (T)(X * length);
			Y = (T)(Y * length);
			Z = (T)(Z * length);
			return *this;
		}

		// 벡터의 길이를 다시 설정한다.
		vector3d<T>& setLength(T newlength)
		{
			normalize();
			return (*this *= newlength);
		}

		// 벡터의 방향을 정 반대로 바꾼다.
		vector3d<T>& invert()
		{
			X *= -1;
			Y *= -1;
			Z *= -1;
			return *this;
		}

		// XZ 평면 안에서 회전한다.
		// center 를 중심으로 회전
		void rotateXZBy(f64 degrees, const vector3d<T>& center = vector3d<T>())
		{
			degrees *= DEGTORAD64;
			f64 cs = cos(degrees);
			f64 sn = sin(degrees);
			X -= center.X;
			Z -= center.Z;
			set((T)(X * cs - Z * sn), Y, (T)(X * sn + Z * cs));
			X += center.X;
			Z += center.Z;
		}

		// YZ 평면 안에서 회전한다.
		// center 를 중심으로 회전
		void rotateYZBy(f64 degrees, const vector3d<T>& center = vector3d<T>())
		{
			degrees *= DEGTORAD64;
			f64 cs = cos(degrees);
			f64 sn = sin(degrees);
			Z -= center.Z;
			Y -= center.Y;
			set(X, (T)(Y * cs - Z * sn), (T)(Y * sn + Z * cs));
			Z += center.Z;
			Y += center.Y;
		}

		// this <- other 사이의 벡터를 구함
		// d 가 0 이면 other vector
		// d 가 1 이면 this vector
		vector3d<T> getInterpolated(const vector3d<T>& other, f64 d) const
		{
			const f64 inv = 1.0 - d;
			return vector3d<T>((T)(other.X * inv + X * d), (T)(other.Y * inv + Y * d), (T)(other.Z * inv + Z * d));
		}

		// a <- b 사이의 벡터를 구함
		// d 가 0 이면 b
		// d 가 1 이면 a
		vector3d<T>& interpolate(const vector3d<T>& a, const vector3d<T>& b, f64 d)
		{
			X = (T)((f64)b.X + ((a.X - b.X) * d));
			Y = (T)((f64)b.Y + ((a.Y - b.Y) * d));
			Z = (T)((f64)b.Z + ((a.Z - b.Z) * d));
			return *this;
		}

		// (0,0,1) 방향벡터와 같은 방향을 가리키도록 하는 회전을 가져 옵니다.
		// 이 함수는 어떠한 대상이 특정 물체를 향하도록 할 때 유용합니다.
		// 반환 되는 X 는 pitch, 반환 되는 Y 는 raw 해야하는 degree 를 의미하며,
		// 두개의 오일러 회전이 충분하므로 Z 는 항상 0 입니다.
		vector3d<T> getHorizontalAngle() const
		{
			vector3d<T> angle;

			const f64 tmp = (atan2((f64)X, (f64)Z) * RADTODEG64);
			angle.Y = (T)tmp;

			if (angle.Y < 0)
				angle.Y += 360;
			if (angle.Y >= 360)
				angle.Y -= 360;

			const f64 z1 = core::squareroot(X * X + Z * Z);

			angle.X = (T)(atan2((f64)z1, (f64)Y) * RADTODEG64 - 90.0);

			if (angle.X < 0)
				angle.X += 360;
			if (angle.X >= 360)
				angle.X -= 360;

			return angle;
		}

		//! 구면 좌표 각도 얻기
		// 다음으로 표시되는 점에 대한 오일러도를 반환합니다.
		//	이 벡터.계산은 극점이(0, 1, 0)이고
		//	X와 Y의 각도를 반환합니다.		
		vector3d<T> getSphericalCoordinateAngles() const
		{
			vector3d<T> angle;
			const f64 length = X * X + Y * Y + Z * Z;

			if (length)
			{
				if (X != 0)
				{
					angle.Y = (T)(atan2((f64)Z, (f64)X) * RADTODEG64);
				}
				else if (Z < 0)
					angle.Y = 180;

				angle.X = (T)(acos(Y * core::reciprocal_squareroot(length)) * RADTODEG64);
			}
			return angle;
		}

		//! (this) 회전 벡터에서 방향 벡터를 만듭니다.
		// 이 벡터는 3 개의 오일러 각도 회전(도 단위)으로 구성된 회전 벡터라고 가정합니다.
		//	구현은 회전을 수행하기 위해 행렬을 사용하는 것과 동일한 계산을 수행합니다.
		//
		//	\ param[in] forwards이 벡터에 의해 회전 될 "앞으로"를 나타내는 방향.
		//	방향을 제공하지 않으면 + Z 축(0, 0, 1)이 전방으로 간주됩니다.
		//	\ return 3 개의 오일러 각으로 정방향을 회전하여 계산 된 방향 벡터
		//	(도 단위)이 벡터로 표현됩니다.
		vector3d<T> rotationToDirection(const vector3d<T>& forwards = vector3d<T>(0, 0, 1)) const
		{
			const f64 cr = cos(core::DEGTORAD64 * X);
			const f64 sr = sin(core::DEGTORAD64 * X);
			const f64 cp = cos(core::DEGTORAD64 * Y);
			const f64 sp = sin(core::DEGTORAD64 * Y);
			const f64 cy = cos(core::DEGTORAD64 * Z);
			const f64 sy = sin(core::DEGTORAD64 * Z);

			const f64 srsp = sr * sp;
			const f64 crsp = cr * sp;

			const f64 pseudoMatrix[] = {
				(cp * cy), (cp * sy), (-sp),
				(srsp * cy - cr * sy), (srsp * sy + cr * cy), (sr * cp),
				(crsp * cy + sr * sy), (crsp * sy - sr * cy), (cr * cp) };

			return vector3d<T>(
				(T)(forwards.X * pseudoMatrix[0] +
					forwards.Y * pseudoMatrix[3] +
					forwards.Z * pseudoMatrix[6]),
				(T)(forwards.X * pseudoMatrix[1] +
					forwards.Y * pseudoMatrix[4] +
					forwards.Z * pseudoMatrix[7]),
				(T)(forwards.X * pseudoMatrix[2] +
					forwards.Y * pseudoMatrix[5] +
					forwards.Z * pseudoMatrix[8]));
		}

		T X;
		T Y;
		T Z;
	}; // end class vector3d<T>

	// 스칼라와 연산
	template <>
	inline vector3d<s32> vector3d<s32>::operator /(s32 val) const { return core::vector3d<s32>(X / val, Y / val, Z / val); }
	template <>
	inline vector3d<s32>& vector3d<s32>::operator /=(s32 val) { X /= val; Y /= val; Z /= val; return *this; }

	template <>
	inline vector3d<s32> vector3d<s32>::getSphericalCoordinateAngles() const
	{
		vector3d<s32> angle;
		const f64 length = X * X + Y * Y + Z * Z;

		if (length)
		{
			if (X != 0)
			{
				angle.Y = round32((f32)(atan2((f64)Z, (f64)X) * RADTODEG64));
			}
			else if (Z < 0)
				angle.Y = 180;

			angle.X = round32((f32)(acos(Y * core::reciprocal_squareroot(length)) * RADTODEG64));
		}
		return angle;
	}

	typedef vector3d<f32> vector3df;
	typedef vector3d<s32> vector3di;

	// 스칼라와 연산
	template<class S, class T>
	vector3d<T> operator*(const S scalar, const vector3d<T>& vector) { return vector * scalar; }

} // end namespace core
} // end namespace Earth