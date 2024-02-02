/*
* Universe system to hold and update all particles 
* @author Dominick Dimpfel
* @date 01/22/2024
*/
#ifndef UNIVERSE_H
#define UNIVERSE_H
#include <vector>
#include <map>
#include "Vec2f.h"
#include "Particle.h"
#include "Manifold.h"
#include "SpatialHashGrid.h"

#define UNIVERSE_CAPACITY		2000
#define GRID_ROWS				50
#define GRID_COLS				50
#define G_CONSTANT				0.000001f //6.67e-11f
#define GRAV_EFFECT_DISTANCE	10.f
#define RESTITUTION				0.5f
#define	MASS_COALESCE_RATIO		1000.f // Needed to handle tunnel issues
#define COALESCE_TOLERANCE		0.0000001f
#define EPSILON_ACCURACY		0.0000001f
#define CORRECTION_SLOP			1.0001f

class Universe
{
private:
	SpatialHashGrid m_collisionGrid;
	SpatialHashGrid m_gravityGrid;
	std::set<int> m_potentialCollisionsIds;
	std::set<int> m_gravityEffectors;

	std::map<int, Particle> m_particles;
	Manifold m_manifold;
	int m_size;
	int m_idCount = 0;

	Vec2f potentialEnergy;
	Vec2f kineticEnergy;

public:
	Universe();
	~Universe();

	void update(float deltaTime);

	Particle& createParticle(const Vec2f& startPos, const Vec2f& startVel);

	Particle& createParticle(const Vec2f& startPos, const Vec2f& startVel, float mass, float radius);

	const std::map<int, Particle>& getParticles() const { return m_particles; }
	Particle& getParticleByID(int id)					{ return m_particles[id]; }
	int& size()											{ return m_size; }

	const SpatialHashGrid& getCollisionGrid() const		{ return m_collisionGrid; }
	const SpatialHashGrid& getGravityGrid() const		{ return m_gravityGrid; }

private:
	void applyCoalescence(Particle& a, Particle& b);

	void applyImpulse(Particle& a, Particle& b, const  Manifold& m);

	void applyGravity(Particle& a, Particle& b);

	bool particlesColliding(Particle& a, Particle& b, Manifold& m);


	// TODO : potential too many calculations
	Vec2f getTotalEnergy() const;

	Vec2f sumPotentialEnergies() const;
	
	Vec2f sumKineticEnergies() const;
};

#endif // !UNIVERSE_H

