/*
* Simple vector 2D float class with most 2D vector operations available.
* @author Dominick Dimpfel
* @date 12/23/23
*/

#include "Vec2f.h"
#include <cmath>
#include <iostream>
#include <iomanip>

#define PI						3.14159265358979323846
#define PI_OVER_2				1.57079632679489661923
#define PI_OVER_4				0.78539816339744830961

#define RAD2DEG(x)				((x) * 57.295754f)
#define DEG2RAD(x)				((x) * 0.0174533f)

#define FLOAT_EPSILON			0.0000000001f

/*
* Unary negation of this
* @return new Vec2f
*/
Vec2f Vec2f::operator-() const
{
	return { -x, -y };
}

/*
* Vector addition between vector on left side and vector
* on right side.
* @return new Vec2f
*/
Vec2f Vec2f::operator + (const Vec2f& r) const
{
	return { x + r.x, y + r.y };
}

/*
* Vector subtraction between this and vector on right side.
* From right side to left side.
* @return new Vec2f
*/
Vec2f Vec2f::operator - (const Vec2f& r) const
{
	return { x - r.x, y - r.y };
}

/*
* Vector scalar multiplication on left vector by float on right.
* @return new Vec2f
*/
Vec2f Vec2f::operator * (float rf) const
{
	return { x * rf, y * rf };
}

/*
* Vector scalar division on left vector by float on right.
* @return new Vec2f
*/
Vec2f Vec2f::operator / (float rf) const
{
	return { x / rf, y / rf };
}

/*
* Vector addition between vector on left side and float
* on right side. Result is stored in left side vector.
* @return void
*/
void Vec2f::operator += (float r)
{
	x += r; y += r;
}

/*
* Vector subtraction between vector on left side and float
* on right side. Result is stored in left side vector.
* @return void
*/
void Vec2f::operator -= (float r)
{
	x -= r; y -= r;
}

/*
* Vector multiplication between vector on left side and float
* on right side. Result is stored in left side vector.
* @return void
*/
void Vec2f::operator *= (float r)
{
	x *= r; y *= r;
}

/*
* Vector division between vector on left side and float
* on right side. Result is stored in left side vector.
* @return void
*/
void Vec2f::operator /= (float r)
{
	x /= r; y /= r;
}

/*
* Vector addition between vector on left side and vector
* on right side. Result is stored in left side vector.
* @return void
*/
void Vec2f::operator+=(const Vec2f& r)
{
	x += r.x;
	y += r.y;
}

/*
* Vector subtraction between vector on left side and vector
* on right side. Result is stored in left side vector.
* @return void
*/
void Vec2f::operator-=(const Vec2f& r)
{
	x -= r.x;
	y -= r.y;
}

/*
* Set this vector to the zero vector.
*/
void Vec2f::zero()
{
	x = 0; y = 0;
}

/*
* Vector multiplication between vectors on left side
* and right side.
* @return new Vec2f
*/
Vec2f Vec2f::multiply(const Vec2f& other) const
{
	return { x * other.x, y * other.y };
}

/*
* Get the length of this vector, uses sqrt
* @return float
*/
float Vec2f::magnitude() const
{
	return sqrt(x * x + y * y);
}

/*
* Get the length of this vector squared to avoid sqrt
* @return float
*/
float Vec2f::magnitudeSquared() const
{
	return x * x + y * y;
}

/*
* Get the length between this vector and another, uses sqrt.
* Vector goes from other to this.
* @return float
*/
float Vec2f::distance(const Vec2f& other) const
{
	return (*this - other).magnitude();
}

/*
* Get the length between this vector and another squared to
* avoid sqrt. Vector goes from other to this.
* @return float
*/
float Vec2f::distanceSquared(const Vec2f& other) const
{
	return (*this - other).magnitudeSquared();
}

/*
* Normalize vector to length 1.
* @return new Vec2f
*/
Vec2f Vec2f::normalized() const
{
	float mag = magnitude();
	return mag == 0 ? Vec2f(0, 0) : Vec2f(x / mag, y / mag);
}

/*
* Normalize this vector to length 1.
* @return void
*/
void Vec2f::normalize()
{
	float mag = magnitude();
	mag == 0 ? x = 0, y = 0 : x /= mag, y /= mag;
}


/*
* Negate vector
* @return new Vec2f
*/
Vec2f Vec2f::negated() const
{
	return { -x, -y };
}

/*
* Negate this vector
* @return void
*/
void Vec2f::negate()
{
	x = -x, y = -y;
}

/*
* Dot product between this vector and another. Result is magnitude of
* this vector in the direction of the other.
* @return float
*/
float Vec2f::dot(const Vec2f& other) const
{
	return x * other.x + y * other.y;
}

/*
* Cross product of two vectors, this and other. The result is a scalar
* representing the signed area of the parallelogram created by them.
* The sign represents rotation; if positive, other is ccw from this,
* and negative means other is cw from this.
* @return float signed area of parallelogram
*/
float Vec2f::cross(const Vec2f& other) const
{
	return x * other.y - y * other.x;
}

/*
* Cross three vectors in 2D to obtain which direction two lines turn.
* If the result is 0, the points are collinear; if it is positive, the
* three points constitute a "left turn" or, otherwise a "right turn"
* (X2 - X1)(Y3 - Y1) - (Y2 - Y1)(X3 - X1)
*/
float Vec2f::cross3(const Vec2f& one, const Vec2f& two, const Vec2f& three)
{
	return(two.x - one.x) * (three.y - one.y) - (two.y - one.y) * (three.x - one.x);
}

/*
* Cross product of this vector and a scalar on the left. The result
* is a perpendicular vector rotated 90 degrees cw (right) and scaled.
*/
Vec2f Vec2f::crossScl(float scalar) const
{
	return { scalar * y, -scalar * x };
}

/*
* Angle between this vector and another.
* @return angle between -1 and 1, -1 is 180 degrees away from this
*/
float Vec2f::angle(const Vec2f& other) const
{
	float magSq = magnitudeSquared();
	float otherMagSq = other.magnitudeSquared();
	return magSq == 0 || otherMagSq == 0 ? 0 : this->dot(other) / sqrt(magSq * otherMagSq);
}

/*
* Angle between this vector and another.
* @return angle between 0 and pi
*/
float Vec2f::angleRad(const Vec2f& other) const
{
	return std::acos(this->angle(other));
}

/*
* Angle between this vector and another.
* Use for printing angles.
* @return angle between 0 and 180
*/
float Vec2f::angleDeg(const Vec2f& other) const
{
	return RAD2DEG(std::acos(this->angle(other)));
}

/*
* Project this vector onto another.
* @return new Vec2f
*/
Vec2f Vec2f::project(const Vec2f& onto) const
{
	float d = this->dot(onto);
	float ontoMagSq = onto.magnitudeSquared();
	return onto * (d / ontoMagSq);
}

/*
* Find the perpendicular vector from a point projected onto another
* vector back to this
* @return new Vec2f
*/
Vec2f Vec2f::perp(const Vec2f& onto) const
{
	return *this - this->project(onto);
}

/*
* Reflect a vector with respect to a normal. This vector is reflected
* naturally off the normal.
* @return new Vec2f
*/
Vec2f Vec2f::reflect(const Vec2f& normal) const
{
	return *this - (normal * (this->dot(normal) * 2.0f));
}

/*
* Find the smallest x and the smallest y separately between this and vector r
* @return new Vec2f(smallest x, smallest y)
*/
Vec2f Vec2f::smallestComponents(const Vec2f& r) const
{
	return { std::min(x, r.x), std::min(y, r.y) };
}

/*
* Find the biggest x and the biggest y separately between this and vector r
* @return new Vec2f(biggest x, biggest y)
*/
Vec2f Vec2f::biggestComponents(const Vec2f& r) const
{
	return { std::max(x, r.x), std::max(y, r.y) };
}

bool Vec2f::equals(const Vec2f& other) const
{
	return fabs(x - other.x) <= FLT_EPSILON && fabs(y - other.y) <= FLT_EPSILON;
}

void Vec2f::print() const {
	std::cout << std::fixed << std::setprecision(2);
	std::cout << "(" << std::setw(7) << x << ", " << std::setw(7) << y << ")\n\n";
}
