/*
* Simple 2D partitioning grid
* @author Dominick Dimpfel
* @date 01/25/2024
*/
#ifndef SPATIALHASHGRID_H
#define SPATIALHASHGRID_H
#include <list>
#include <map>
#include <set>
#include <string>
#include "Vec2f.h"

struct Client
{
	int id;
	int min[2]{};
	int max[2]{};

	Client() { id = -1; }
	Client(int i) { id = i; }
	~Client() {}

	bool operator < (const Client& rs) const
	{
		return id < rs.id;
	}
};

// TODO : make grid infinite where empty cells are disabled
class SpatialHashGrid
{
private:
	Vec2f m_origin;
	Vec2f m_extents;
	Vec2f m_cellDims;
	int m_rows;
	int m_cols;

	std::map<std::string, std::set<int>> m_cells{};
	std::map<int, Client> m_clients;
	std::set<int> m_potentialCollisions{};

	std::string _key(int r, int c)
	{
		return std::to_string(r) + '.' + std::to_string(c);
	}

public:
	SpatialHashGrid() { m_rows = -1; m_cols = -1; }
	SpatialHashGrid(const Vec2f& origin, const Vec2f& extents, int rows, int cols)
	{
		m_origin = origin;
		m_extents = extents;
		m_cellDims.x = (extents.x - origin.x) / (rows);
		m_cellDims.y = (extents.y - origin.y) / (cols);
		m_rows = rows;
		m_cols = cols;
	}
	~SpatialHashGrid() {}
	
	/*
	* Add new client to grid
	*/
	void addClient(int id, const Vec2f& position, float radius);

	/*
	* Update client's position in grid
	*/
	void update(int id, const Vec2f& position, float radius);
	
	/*
	* Remove client from grid
	*/
	void remove(int id);

	/*
	* Delete client from grid
	*/
	void deleteClient(int id);

	/*
	* Find all clients in cells in radius near position
	*/
	std::set<int> findNear(const Vec2f& position, float radius, std::set<int>& results);
	/*
	* Find all clients in cells in radius near position
	*/
	std::set<int> findNear(int i, std::set<int>& results);

	const Vec2f& getOrigin() const		{ return m_origin; }
	const Vec2f& getExtents() const		{ return m_extents; }
	int getRows() const					{ return m_rows; }
	int getCols() const					{ return m_cols; }

private:
	/*
	* Find cells { min, max } and clamp to bounds of grid
	* @return row and col index of position
	*/
	void _getCellIndex(const Vec2f& position, int* b) const;
};

#endif // !SPATIALHASHGRID_H
