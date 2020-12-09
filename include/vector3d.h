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
		explicit vector3d(T n) : X(n), Y(n), Z(n) {} // explicit : ������ ����ȯ�� ������� �ʴ´�. �ݵ�� ����� ����ȯ�� �ؾ��Ѵ�.
		vector3d(const vector3d<T>& other) : X(other.X), Y(other.Y), Z(other.Z) {}

		// vector3d �� ���� -,+,+=,-=,*,*=,/,/= etc(��)
		// ��Ģ���꿡 ���� �� ���� �Ѵ�.
		// ������ ������
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

		// ũ�� �񱳴� X �� �������� �ϰ�, X �� ���ٸ�, Y �� �������� ���Ѵ�.
		// Y �� ������ Z �� �������� ���Ѵ�.
		//! sort in order X, Y, Z. Equality with rounding tolerance.
		bool operator<=(const vector3d<T>& other) const
		{
			return 	(X < other.X || core::equals(X, other.X)) ||
				(core::equals(X, other.X) && (Y < other.Y || core::equals(Y, other.Y))) ||
				(core::equals(X, other.X) && core::equals(Y, other.Y) && (Z < other.Z || core::equals(Z, other.Z)));
		}

		// ũ�� �񱳴� X �� �������� �ϰ�, X �� ���ٸ�, Y �� �������� ���Ѵ�.
		// Y �� ������ Z �� �������� ���Ѵ�.
		//! sort in order X, Y, Z. Equality with rounding tolerance.
		bool operator>=(const vector3d<T>& other) const
		{
			return 	(X > other.X || core::equals(X, other.X)) ||
				(core::equals(X, other.X) && (Y > other.Y || core::equals(Y, other.Y))) ||
				(core::equals(X, other.X) && core::equals(Y, other.Y) && (Z > other.Z || core::equals(Z, other.Z)));
		}

		// ũ�� �񱳴� X �� �������� �ϰ�, X �� ���ٸ�, Y �� �������� ���Ѵ�.
		// Y �� ������ Z �� �������� ���Ѵ�.
		//! sort in order X, Y, Z. Difference must be above rounding tolerance.
		bool operator<(const vector3d<T>& other) const
		{
			return 	(X < other.X && !core::equals(X, other.X)) ||
				(core::equals(X, other.X) && Y < other.Y && !core::equals(Y, other.Y)) ||
				(core::equals(X, other.X) && core::equals(Y, other.Y) && Z < other.Z && !core::equals(Z, other.Z));
		}

		// ũ�� �񱳴� X �� �������� �ϰ�, X �� ���ٸ�, Y �� �������� ���Ѵ�.
		// Y �� ������ Z �� �������� ���Ѵ�.
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

		// ���̸� ��´�.
		T getLength() const { return core::squareroot(X * X + Y * Y + Z * Z); }

		// ���� ���̸� ������ �ʾƵ� �Ǵ� ���, squareroot ������ ���� �ʴ´�.
		// ex) �κ����� ������� �������� ���� Ȯ���� �� ���� ���̸� ������ �ʾƵ� ��
		// squareroot (������ ���ϴ� ��) �� ����� ���� ��� ������...
		T getLengthSQ() const { return X * X + Y * Y + Z * Z; }

		// ��Į�� ��(����)
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

		// ���� ��(ũ�ν� ���δ�Ʈ)
		// �� ������ ��� ������ ���Ͱ� ��������.
		vector3d<T> crossProduct(const vector3d<T>& p) const
		{
			return vector3d<T>(Y * p.Z - Z * p.Y, Z * p.X - X * p.Z, X * p.Y - Y * p.X);
		}

		// ���� ���Ͱ� �� ���� ������ ������ Ȯ���Ѵ�.
		// �κ��� ������ ���̸� true
		// �׷�ġ ������ false
		bool isBetweenPoints(const vector3d<T>& begin, const vector3d<T>& end) const
		{
			const T f = (end - begin).getLengthSQ();
			return getDistanceFromSQ(begin) <= f &&
				getDistanceFromSQ(end) <= f;
		}

		// ���͸� ����ȭ �ϴ�
		// ũ�Ⱑ 1�� ���� ���͸� �����.
		vector3d<T>& normalize()
		{
			f64 length = X * X + Y * Y + Z * Z;
			if (length == 0) // ���� ���̰� 0 �̸� ������ ���ϴ� ���Ŀ��� NAN����(0���� ����)�� �߻��ϹǷ� ���̻� ��� ���� �ʴ´�.
				return *this;
			length = core::reciprocal_squareroot(length);

			X = (T)(X * length);
			Y = (T)(Y * length);
			Z = (T)(Z * length);
			return *this;
		}

		// ������ ���̸� �ٽ� �����Ѵ�.
		vector3d<T>& setLength(T newlength)
		{
			normalize();
			return (*this *= newlength);
		}

		// ������ ������ �� �ݴ�� �ٲ۴�.
		vector3d<T>& invert()
		{
			X *= -1;
			Y *= -1;
			Z *= -1;
			return *this;
		}

		// XZ ��� �ȿ��� ȸ���Ѵ�.
		// center �� �߽����� ȸ��
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

		// YZ ��� �ȿ��� ȸ���Ѵ�.
		// center �� �߽����� ȸ��
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

		// this <- other ������ ���͸� ����
		// d �� 0 �̸� other vector
		// d �� 1 �̸� this vector
		vector3d<T> getInterpolated(const vector3d<T>& other, f64 d) const
		{
			const f64 inv = 1.0 - d;
			return vector3d<T>((T)(other.X * inv + X * d), (T)(other.Y * inv + Y * d), (T)(other.Z * inv + Z * d));
		}

		// a <- b ������ ���͸� ����
		// d �� 0 �̸� b
		// d �� 1 �̸� a
		vector3d<T>& interpolate(const vector3d<T>& a, const vector3d<T>& b, f64 d)
		{
			X = (T)((f64)b.X + ((a.X - b.X) * d));
			Y = (T)((f64)b.Y + ((a.Y - b.Y) * d));
			Z = (T)((f64)b.Z + ((a.Z - b.Z) * d));
			return *this;
		}

		// (0,0,1) ���⺤�Ϳ� ���� ������ ����Ű���� �ϴ� ȸ���� ���� �ɴϴ�.
		// �� �Լ��� ��� ����� Ư�� ��ü�� ���ϵ��� �� �� �����մϴ�.
		// ��ȯ �Ǵ� X �� pitch, ��ȯ �Ǵ� Y �� raw �ؾ��ϴ� degree �� �ǹ��ϸ�,
		// �ΰ��� ���Ϸ� ȸ���� ����ϹǷ� Z �� �׻� 0 �Դϴ�.
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

		//! ���� ��ǥ ���� ���
		// �������� ǥ�õǴ� ���� ���� ���Ϸ����� ��ȯ�մϴ�.
		//	�� ����.����� ������(0, 1, 0)�̰�
		//	X�� Y�� ������ ��ȯ�մϴ�.		
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

		//! (this) ȸ�� ���Ϳ��� ���� ���͸� ����ϴ�.
		// �� ���ʹ� 3 ���� ���Ϸ� ���� ȸ��(�� ����)���� ������ ȸ�� ���Ͷ�� �����մϴ�.
		//	������ ȸ���� �����ϱ� ���� ����� ����ϴ� �Ͱ� ������ ����� �����մϴ�.
		//
		//	\ param[in] forwards�� ���Ϳ� ���� ȸ�� �� "������"�� ��Ÿ���� ����.
		//	������ �������� ������ + Z ��(0, 0, 1)�� �������� ���ֵ˴ϴ�.
		//	\ return 3 ���� ���Ϸ� ������ �������� ȸ���Ͽ� ��� �� ���� ����
		//	(�� ����)�� ���ͷ� ǥ���˴ϴ�.
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

	// ��Į��� ����
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

	// ��Į��� ����
	template<class S, class T>
	vector3d<T> operator*(const S scalar, const vector3d<T>& vector) { return vector * scalar; }

} // end namespace core
} // end namespace Earth