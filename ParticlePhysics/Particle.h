/*
* Simple 2D particle class to implement physics.
* Has no physical representation.
* @author Dominick Dimpfel
* @date 01/22/2024
*/
#ifndef PARTICLE_H
#define PARTICLE_H
#include "Vec2f.h"
#include <SFML/Graphics.hpp>

#define PARTICLE_MASS			1.f
#define RADIUS_TO_MASS_RATIO	1.f

class Particle
{
private:
	int m_id;
	bool m_active;
	float m_radius;

	Vec2f m_pos;
	Vec2f m_vel;
	Vec2f m_acc;

	float m_mass;
	float m_invMass;

	Vec2f m_forces;

	sf::Color m_c{};

public:
	Particle() {}
	Particle(int i)
	{
		m_id = i;
		m_active = false;
		m_radius = RADIUS_TO_MASS_RATIO * PARTICLE_MASS;

		m_pos = Vec2f();
		m_vel = Vec2f();
		m_acc = Vec2f();

		m_mass = PARTICLE_MASS;
		m_invMass = 1.f / PARTICLE_MASS;

		m_forces = Vec2f();
	}
	~Particle() = default;

	void update(float dt)
	{
		m_acc = m_forces * m_invMass;
		m_vel += m_acc * dt;
		m_pos += m_vel * dt;
		clearForces();
	}

	const sf::Color& getColor() const		{ return m_c; }
	void setColor(int r, int g, int b)		{ m_c = sf::Color(r, g, b); }
	void setColor(sf::Color c)				{ m_c = c; }

	void addForce(const Vec2f& f)			{ m_forces += f; }
	const Vec2f& getForces()				{ return m_forces; }
	void clearForces()						{ m_forces.zero(); }

	const float getRadius() const			{ return m_radius; }
	void setRadius(float radius)			{ m_radius = radius; }

	const Vec2f& getPos() const				{ return m_pos; }
	void setPos(const Vec2f& pos)			{ m_pos = pos; }

	const Vec2f& getVel() const				{ return m_vel; }
	void setVel(const Vec2f& vel)			{ m_vel = vel; }

	const Vec2f& getAcc() const				{ return m_acc; }
	void setAcc(const Vec2f& acc)			{ m_acc = acc; }

	const float getMass() const				{ return m_mass; }
	const float getInvMass() const			{ return m_invMass; }
	void setMass(float mass) 
	{ 
		m_mass = mass; 
		m_invMass = m_mass != 0.0f ? 1 / m_mass : 0.0f;
	}

	const int getID() const					{ return m_id; }

	bool isActive() const					{ return m_active; }
	void setActive(bool val)				{ m_active = val; }

	bool operator < (const Particle& rs) const
	{
		return m_id < rs.getID();
	}
};

#endif // !PARTICLE_H

