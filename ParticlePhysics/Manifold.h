/*
* MManifold for particle collisions
* @author Dominick Dimpfel
* @date 01/22/2024
*/

#ifndef MANIFOLD_H
#define MANIFOLD_H
#include "Vec2f.h"

class Manifold
{
private:
	bool m_isCoalescing;

	Vec2f m_normal;
	Vec2f m_contactPoint;
	float m_depth;

public:
	Manifold() 
	{
		m_isCoalescing = false;

		m_normal = Vec2f();
		m_contactPoint = Vec2f();
		m_depth = 0.0f;
	}
	~Manifold() = default;

	void reset()
	{
		m_isCoalescing = false;

		m_normal.zero();
		m_contactPoint.zero();
		m_depth = 0.0f;
	}

	//void setActive(bool active) { m_isActive = active; }
	//const bool isActive() const { return m_isActive; }

	void setCoalescing(bool coalescing) { m_isCoalescing = coalescing; }
	const bool isCoalescing() const { return m_isCoalescing; }

	void setNormal(const Vec2f& n) { m_normal = n; }
	const Vec2f& getNormal() const { return m_normal; }

	void setContactPoint(const Vec2f& cp) { m_contactPoint = cp; }
	const Vec2f& getContactPoint() const { return m_contactPoint; }

	void setDepth(float depth) { m_depth = depth; }
	const float getDepth() const { return m_depth; }
};

#endif // !MANIFOLD_H
