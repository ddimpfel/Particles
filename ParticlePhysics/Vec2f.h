/*
* Simple vector 2D float class with most 2D vector operations available.
* @author Dominick Dimpfel
* @date 12/23/23
*/
#ifndef VEC2F_H
#define VEC2F_H
#include <cmath>
#include <iostream>
#include <iomanip>

#define PI						3.14159265358979323846
#define PI_OVER_2				1.57079632679489661923
#define PI_OVER_4				0.78539816339744830961

#define RAD2DEG(x)				((x) * 57.295754f)
#define DEG2RAD(x)				((x) * 0.0174533f)

#define FLOAT_EPSILON			0.0000000001f

class Vec2f
{
public:
	float x, y;

	/* Zero vector */
	Vec2f() : x(0.0f), y(0.0f) {}
	/* Initialize vector to _x, _y */
	Vec2f(float _x, float _y) : x(_x), y(_y) {}
	~Vec2f() {}

	/*
	* Unary negation of this
	* @return new Vec2f
	*/
	Vec2f operator-() const;

	/*
	* Vector addition between vector on left side and vector
	* on right side.
	* @return new Vec2f
	*/
	Vec2f operator + (const Vec2f& r) const;

	/*
	* Vector subtraction between this and vector on right side.
	* From right side to left side.
	* @return new Vec2f
	*/
	Vec2f operator - (const Vec2f& r) const;

	/*
	* Vector scalar multiplication on left vector by float on right.
	* @return new Vec2f
	*/
	Vec2f operator * (float rf) const;

	/*
	* Vector scalar division on left vector by float on right.
	* @return new Vec2f
	*/
	Vec2f operator / (float rf) const;

	/*
	* Vector addition between vector on left side and float
	* on right side. Result is stored in left side vector.
	* @return void
	*/
	void operator += (float r);

	/*
	* Vector subtraction between vector on left side and float
	* on right side. Result is stored in left side vector.
	* @return void
	*/
	void operator -= (float r);

	/*
	* Vector multiplication between vector on left side and float
	* on right side. Result is stored in left side vector.
	* @return void
	*/
	void operator *= (float r);

	/*
	* Vector division between vector on left side and float
	* on right side. Result is stored in left side vector.
	* @return void
	*/
	void operator /= (float r);

	/*
	* Vector addition between vector on left side and vector
	* on right side. Result is stored in left side vector.
	* @return void
	*/
	void operator += (const Vec2f& r);

	/*
	* Vector subtraction between vector on left side and vector
	* on right side. Result is stored in left side vector.
	* @return void
	*/
	void operator -= (const Vec2f& r);

	/*
	* Set this vector to the zero vector.
	*/
	void zero();

	/*
	 * Multiply this Vector2f component-wise by another Vector2f.
	 * @param other, the vector to multiply by
	 * @return this
	 */
	Vec2f multiply(const Vec2f& other) const;

	/*
	* Get the length of this vector, uses sqrt
	* @return float magnitude
	*/
	float magnitude() const;

	/*
	* Get the length of this vector squared to avoid sqrt
	* @return float magnitude squared
	*/
	float magnitudeSquared() const;

	/*
	* Get the length between this vector and another, uses sqrt
	* @return float
	*/
	float distance(const Vec2f& other) const;

	/*
	* Get the length  between this vector and another squared to
	* avoid sqrt
	* @return float
	*/
	float distanceSquared(const Vec2f& other) const;

	/*
	* Normalize vector to length 1.
	* @return new Vec2f
	*/
	Vec2f normalized() const;

	/*
	* Normalize this vector to length 1.
	* @return void
	*/
	void normalize();

	/*
	* Negate vector
	* @return new Vec2f
	*/
	Vec2f negated() const;

	/*
	* Negate this vector
	* @return void
	*/
	void negate();

	/*
	* Dot product between this vector and another. Result is magnitude of
	* this vector in the direction of the other.
	* @return float
	*/
	float dot(const Vec2f& other) const;

	/*
	* Cross product of two vectors, this and other. The result is a scalar
	* representing the signed area of the parallelogram created by them.
	* The sign represents rotation; if positive, other is ccw from this,
	* and negative means other is cw from this.
	* @return float signed area of parallelogram, positive if other ccw
	*/
	float cross(const Vec2f& other) const;

	/*
	* Cross three vectors in 2D to obtain which direction two lines turn.
	* If the result is 0, the points are collinear; if it is positive, the
	* three points constitute a "left turn" or, otherwise a "right turn"
	* (X2 - X1)(Y3 - Y1) - (Y2 - Y1)(X3 - X1)
	*/
	static float cross3(const Vec2f& one, const Vec2f& two, const Vec2f& three);

	/*
	* Cross product of this vector and a scalar on the left. The result
	* is a perpendicular vector rotated 90 degrees cw (right) and scaled.
	*/
	Vec2f crossScl(float scalar) const;

	/*
	* Angle between this vector and another.
	* @return angle between -1 and 1, -1 is 180 degrees away from this
	*/
	float angle(const Vec2f& other) const;

	/*
	* Angle between this vector and another.
	* @return angle between 0 and pi
	*/
	float angleRad(const Vec2f& other) const;

	/*
	* Angle between this vector and another.
	* @return angle between 0 and 180
	*/
	float angleDeg(const Vec2f& other) const;

	/*
	* Project this vector onto another.
	* @return new Vec2f
	*/
	Vec2f project(const Vec2f& onto) const;

	/*
	* Find the perpendicular vector from another back to this
	* @return new Vec2f
	*/
	Vec2f perp(const Vec2f& onto) const;

	/*
	* Reflect a vector with respect to a normal. This vector is reflected
	* naturally off the normal.
	* @return new Vec2f
	*/
	Vec2f reflect(const Vec2f& normal) const;

	/*
	* Find the smallest x and the smallest y separately between this and vector r
	* @return new Vec2f(smallest x, smallest y)
	*/
	Vec2f smallestComponents(const Vec2f& r) const;

	/*
	* Find the biggest x and the biggest y separately between this and vector r
	* @return new Vec2f(biggest x, biggest y)
	*/
	Vec2f biggestComponents(const Vec2f& r) const;

	bool equals(const Vec2f& other) const;

	void print() const;
};

#endif // !VEC2F_H
