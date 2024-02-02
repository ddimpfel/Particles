#include "Main.h"
#include <SFML/Graphics.hpp>
#include "Universe.h"
using namespace std;
using namespace sf;


void wrapAround(int width, int height, Particle& p)
{
	if (p.getPos().x > width)
		p.setPos(Vec2f(0, p.getPos().y));
	else if (p.getPos().y > height)
		p.setPos(Vec2f(p.getPos().x, 0));
	else if (p.getPos().x < 0)
		p.setPos(Vec2f(width, p.getPos().y));
	else if (p.getPos().y < 0)
		p.setPos(Vec2f(p.getPos().x, height));
}


void setupCircularOrbits(Universe& u, const Vec2f& CENTER, float CENTER_MASS, float CENTER_RADIUS, float MAX_RADIUS) {
	Particle& sun = u.createParticle(CENTER, Vec2f(), CENTER_MASS, CENTER_RADIUS);
	sun.setColor(253, 184, 19);

	CircleShape csCenter = CircleShape(sun.getRadius());
	csCenter.setFillColor(sun.getColor());

	for (int i = 0; i < UNIVERSE_CAPACITY - 1; i++) 
	{
		auto distance = static_cast<float>(rand() % static_cast<int>(MAX_RADIUS)) + sun.getRadius() * 1.2f;
		float angle = static_cast<float>(rand()) / RAND_MAX * 2 * PI;
		Vec2f pos = Vec2f(distance * cos(angle), distance * sin(angle)) + CENTER;

		float speed = sqrt(G_CONSTANT * CENTER_MASS / distance);
		Vec2f vel = Vec2f(-speed * sin(angle), speed * cos(angle));

		Particle& p = u.createParticle(pos, vel);
		p.setMass(rand() % 2 + .5f);
		p.setRadius(p.getMass() * RADIUS_TO_MASS_RATIO);
		p.setColor(rand() % 255, rand() % 255, rand() % 255);

		CircleShape csi;
		csi.setRadius(p.getRadius());
		csi.setFillColor(p.getColor());
		csi.setPosition(p.getPos().x - p.getRadius(), p.getPos().y - p.getRadius());
	}
}

void setupDiskOfParticles(Universe& u, const Vec2f& CENTER, float MAX_RADIUS) {
	for (int i = 0; i < UNIVERSE_CAPACITY; i++) 
	{
		auto distance = static_cast<float>(rand() % static_cast<int>(MAX_RADIUS) + 1);
		float angle = static_cast<float>(rand()) / RAND_MAX * 2 * PI;
		Vec2f pos = Vec2f(distance * cos(angle), distance * sin(angle)) + CENTER;

		float speed = sqrt(G_CONSTANT * PARTICLE_MASS / distance);
		Vec2f vel = Vec2f(-speed * sin(angle), speed * cos(angle));

		Particle& p = u.createParticle(pos, vel);
		p.setColor(rand() % 255, rand() % 255, rand() % 255);

		CircleShape csi;
		csi.setRadius(p.getRadius());
		csi.setFillColor(p.getColor());
		csi.setPosition(p.getPos().x - p.getRadius(), p.getPos().y - p.getRadius());
	}
}

void setupRandomDispersion(Universe& u, int WIDTH, int HEIGHT) {
	for (int i = 0; i < UNIVERSE_CAPACITY; i++) 
	{
		Vec2f pos = Vec2f(rand() % WIDTH, rand() % HEIGHT);

		Particle& p = u.createParticle(pos, Vec2f());
		p.setColor(rand() % 255, rand() % 255, rand() % 255);
		p.setMass(10.f);

		CircleShape csi;
		csi.setRadius(p.getRadius());
		csi.setFillColor(p.getColor());
		csi.setPosition(p.getPos().x - p.getRadius(), p.getPos().y - p.getRadius());
	}
}

void drawGrid(SpatialHashGrid grid, RenderWindow& window, Color color)
{
	RectangleShape rs;
	float rowWidth = (grid.getExtents().x - grid.getOrigin().x) / grid.getRows(); 
	float colHeight = (grid.getExtents().y - grid.getOrigin().y) / grid.getCols();
	for (int r = 0; r < grid.getRows(); r++)
	{
		for (int c = 0; c < grid.getCols(); c++)
		{
			rs.setSize(Vector2f(rowWidth, colHeight));
			rs.setPosition(grid.getOrigin().x + r * rowWidth, grid.getOrigin().y + c * colHeight);
			rs.setFillColor(Color::Transparent);
			rs.setOutlineColor(color);
			rs.setOutlineThickness(1.f);
			window.draw(rs);
		}
	}
}


#define	WIDTH		1200
#define HEIGHT		675
#define DELTA_TIME	100.f

int main()
{
	srand(time(nullptr));
	const Vec2f CENTER = Vec2f(WIDTH / 2, HEIGHT / 2);
	int count = 0;


	RenderWindow window(VideoMode(WIDTH, HEIGHT), "Particles");
	window.setFramerateLimit(60);
	RectangleShape fadeBackground = RectangleShape(Vector2f(WIDTH, HEIGHT));
	fadeBackground.setFillColor(Color(0, 0, 0, 100));


	Universe u = Universe();
	CircleShape shape;


	//Particle& p1 = u.createParticle(Vec2f(500, 225), Vec2f(0.f, 0), 50, 5);
	//p1.setColor(rand() % 255, rand() % 255, rand() % 255);
	//Particle& p2 = u.createParticle(Vec2f(700, 425), Vec2f(0.f, 0), 50, 5);
	//p2.setColor(rand() % 255, rand() % 255, rand() % 255);
	//Particle& p3 = u.createParticle(Vec2f(700, 225), Vec2f(0.f, 0), 50, 5);
	//p3.setColor(rand() % 255, rand() % 255, rand() % 255);
	//Particle& p4 = u.createParticle(Vec2f(500, 425), Vec2f(0.f, 0), 50, 5);
	//p4.setColor(rand() % 255, rand() % 255, rand() % 255);

	//setupCircularOrbits(u, CENTER, 3'000.f, 50.f, 300.f);
	setupDiskOfParticles(u, CENTER, 250.f);
	//setupRandomDispersion(u, WIDTH, HEIGHT);



	while (window.isOpen())
	{
		Event event = Event();
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed)
				window.close();
		}


		window.draw(fadeBackground);
		//window.clear();


		//drawGrid(u.getCollisionGrid(), window, Color::Green);
		//drawGrid(u.getGravityGrid(), window, Color::Blue);

		for (auto& kv :u.getParticles())
		{
			const Particle& p = kv.second;
			//wrapAround(WIDTH, HEIGHT, p);
			shape.setFillColor(p.getColor());
			shape.setPosition(p.getPos().x - p.getRadius(), p.getPos().y - p.getRadius());
			if (shape.getRadius() != p.getRadius())
				shape.setRadius(static_cast<int>(p.getRadius()));
		
			window.draw(shape);
		}


		window.display();


		u.update(DELTA_TIME);

	}
	return 0;
}
