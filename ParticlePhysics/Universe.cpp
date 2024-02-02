/*
* Universe system to hold and update all particles 
* @author Dominick Dimpfel
* @date 01/22/2024
*/

#include "Universe.h"
#include <vector>
#include <map>
#include "Vec2f.h"
#include "Particle.h"
#include "Manifold.h"
#include "SpatialHashGrid.h"

Universe::Universe()
{
	m_collisionGrid = SpatialHashGrid(Vec2f(0, 0), Vec2f(1200, 680), 25, 15);
	//gravityGrid = Grid(21, 14, Vec2f(80, 80), Vec2f(-240, -180));
	m_size = UNIVERSE_CAPACITY;
	m_manifold = Manifold();
}
Universe::~Universe(){}

// Need to fix id list so this can work. Also some how accessing deleted particle which is causing major data corruption
void Universe::update(float deltaTime)
{
	for (auto& particle : m_particles)
	{
		Particle& a = particle.second;

		m_collisionGrid.findNear(a.getID(), m_potentialCollisionsIds);

		for (int id : m_potentialCollisionsIds)
		{
			if (a.getID() == id) continue;
			Particle& b = m_particles[id];

			m_manifold.reset();
			if (particlesColliding(a, b, m_manifold))
			{
				if (m_manifold.isCoalescing())
				{
					//std::cout << id << " b id" << std::endl;
					applyCoalescence(a, b);
					std::cout << std::endl;
					break;
				}
				applyImpulse(a, b, m_manifold);
			}
		}

		m_potentialCollisionsIds.clear();
	}

	for (auto itA = m_particles.begin(); itA != m_particles.end(); ++itA)
	{
		auto itB = itA;
		itB++;
		for (; itB != m_particles.end(); ++itB)
		{
			applyGravity(itA->second, itB->second);
		}
	}

	for (auto& kv : m_particles)
	{
		Particle& particle = kv.second;
		particle.update(deltaTime);
		m_collisionGrid.update(particle.getID(), particle.getPos(), particle.getRadius());
	}
}

// TODO: Make new particle as container of old particles to add destruction?
void Universe::applyCoalescence(Particle& a, Particle& b)
{
	// B is larger mass but A cannot be deleted (itA++ throws exceptions otherwise)
	if (b.getMass() > a.getMass())
	{
		//std::cout << "b was larger" << std::endl;
		Vec2f pr = a.getPos() - b.getPos();
		Vec2f pOffset = pr * b.getInvMass();
		a.setPos(b.getPos() + pOffset);

		Vec2f vr = a.getVel() - b.getVel();
		Vec2f vOffset = vr * b.getInvMass();
		a.setVel(b.getVel() + vOffset);

		a.setRadius(std::sqrt(a.getRadius() * a.getRadius() + b.getRadius() * b.getRadius()));
		a.setMass(a.getMass() + b.getMass());
		a.addForce(b.getForces());
		a.setColor(b.getColor());

		int idb = b.getID();
		m_collisionGrid.deleteClient(idb);
		m_particles.erase(idb);
		m_size--;
		//std::cout << idb << " deleted by " << a.getID() << std::endl;

		return;
	}
	//std::cout << "a was larger" << std::endl;

	Vec2f pr = b.getPos() - a.getPos();
	Vec2f pOffset = pr * a.getInvMass();
	a.setPos(a.getPos() + pOffset);

	Vec2f vr = b.getVel() - a.getVel();
	Vec2f vOffset = vr * a.getInvMass();
	a.setVel(a.getVel() + vOffset);

	a.setRadius(std::sqrt(a.getRadius() * a.getRadius() + b.getRadius() * b.getRadius()));
	a.setMass(a.getMass() + b.getMass());
	a.addForce(b.getForces());

	int idb = b.getID();
	m_collisionGrid.deleteClient(idb);
	m_particles.erase(idb);
	m_size--;
	//std::cout << idb << " deleted by " << a.getID() << std::endl;
}

void Universe::applyImpulse(Particle& a, Particle& b, const Manifold& m)
{
	Vec2f normal = m.getNormal();
	// Normal should point from a to b
	if (normal.dot(b.getPos() - a.getPos()) < 0.f)
		normal.negate();

	Vec2f relativeVelocity = a.getVel() - b.getVel();
	float relNormalVelMag = relativeVelocity.dot(normal);

	// Linear impulse
	float res = RESTITUTION + 1;
	float j = (-relNormalVelMag * res) / (a.getInvMass() + b.getInvMass());

	Vec2f jn = normal * j;
	Vec2f av = a.getVel() + (jn * a.getInvMass());
	Vec2f bv = b.getVel() - (jn * b.getInvMass());

	Vec2f correction = (normal * CORRECTION_SLOP) * m.getDepth() / 2;
	a.setPos(a.getPos() - correction);
	b.setPos(b.getPos() + correction);

	a.setVel(av);
	b.setVel(bv);
}

void Universe::applyGravity(Particle& a, Particle& b)
{
	Vec2f r = a.getPos() - b.getPos();

	// Ignore overlapping particles to avoid infinite force
	if (r.magnitudeSquared() < EPSILON_ACCURACY)
		return;

	Vec2f rNormal = r.normalized();

	float n = G_CONSTANT * a.getMass() * b.getMass();
	float d = r.magnitudeSquared();


	Vec2f fg = rNormal * (n / d);

	b.addForce(fg);
	a.addForce(-fg);
}

bool Universe::particlesColliding(Particle& a, Particle& b, Manifold& m)
{
	float radii = a.getRadius() + b.getRadius();
	Vec2f distance = a.getPos() - b.getPos();

	if (distance.magnitudeSquared() > radii * radii)
		return false;

	// Relative speed or distance between centers below threshold 
	if (abs(a.getVel().dot(b.getVel()) - a.getVel().magnitudeSquared()) < COALESCE_TOLERANCE || 
		a.getMass() > b.getMass() * MASS_COALESCE_RATIO ||
		distance.magnitudeSquared() < EPSILON_ACCURACY)
	{
		m.setCoalescing(true);
		return true;
	}

	m.setNormal(distance.normalized());
	m.setContactPoint(b.getPos() + distance / 2);
	m.setDepth(radii - distance.magnitude());

	return true;
}

Particle& Universe::createParticle(const Vec2f& startPos, const Vec2f& startVel)
{
	Particle p = Particle(m_idCount);
	p.setPos(startPos);
	p.setVel(startVel);
	m_particles[m_idCount] = p;

	m_collisionGrid.addClient(m_idCount, startPos, p.getRadius());

	return m_particles[m_idCount++];
}

Particle& Universe::createParticle(const Vec2f& startPos, const Vec2f& startVel, float mass, float radius)
{
	Particle p = Particle(m_idCount);
	p.setPos(startPos);
	p.setVel(startVel);
	p.setMass(mass);
	p.setRadius(radius);
	m_particles[m_idCount] = p;

	m_collisionGrid.addClient(m_idCount, startPos, radius);

	return m_particles[m_idCount++];
}


Vec2f Universe::getTotalEnergy() const
{
	return Vec2f();
}

Vec2f Universe::sumPotentialEnergies() const
{
	return Vec2f();
}

Vec2f Universe::sumKineticEnergies() const
{
	return Vec2f();
}
