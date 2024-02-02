/*
* Simple 2D partitioning grid
* @author Dominick Dimpfel
* @date 01/26/2024
*/

#include "SpatialHashGrid.h"
#include <set>
#include <map>
#include <vector>
#include <string>
#include "Vec2f.h"

void SpatialHashGrid::addClient(int id, const Vec2f& position, float radius)
{
	Vec2f min = { position.x - radius - m_origin.x, position.y - radius - m_origin.y };
	Vec2f max = { position.x + radius - m_origin.x, position.y + radius - m_origin.y };

	Client cli = Client(id);
	_getCellIndex(min, cli.min);
	_getCellIndex(max, cli.max);

	for (int r = cli.min[0]; r <= cli.max[0]; r++)
	{
		for (int c = cli.min[1]; c <= cli.max[1]; c++)
		{
			std::string cell = _key(r, c);
			m_cells[cell].insert(id);
		}
	}
}

void SpatialHashGrid::_getCellIndex(const Vec2f& position, int* b) const
{
	// infinite?
	float r = (position.y - m_origin.y) / m_cellDims.y;
	float c = (position.x - m_origin.x) / m_cellDims.x;

	b[0] = static_cast<int>(r);
	b[1] = static_cast<int>(c);

	// simon dev way
	//float r = std::min(std::max((position.y - m_origin.y) / (m_extents.y - m_origin.y), 0.0f), 1.0f);
	//float c = std::min(std::max((position.x - m_origin.x) / (m_extents.x - m_origin.x), 0.0f), 1.0f);

	//b[0] = std::floor(r * (m_rows - 1));
	//b[1] = std::floor(c * (m_cols - 1));
}

void SpatialHashGrid::update(int i, const Vec2f& position, float radius)
{
	Vec2f min = { position.x - radius - m_origin.x, position.y - radius - m_origin.y };
	Vec2f max = { position.x + radius - m_origin.x, position.y + radius - m_origin.y };
	 
	int iMin[2], iMax[2]; 
	_getCellIndex(min, iMin);
	_getCellIndex(max, iMax);

	Client& cli = m_clients[i];
	//std::cout << i << std::endl;
	//std::cout << cli.min[0] << " " << cli.min[1] << " min" << std::endl;
	//std::cout << cli.max[0] << " " << cli.max[1] << " max" << std::endl;

	if (cli.min[0] == iMin[0] &&
		cli.min[1] == iMin[1] &&
		cli.max[0] == iMax[0] &&
		cli.max[1] == iMax[1])
		return;

	remove(i);

	cli.min[0] = iMin[0];
	cli.min[1] = iMin[1];
	cli.max[0] = iMax[0];
	cli.max[1] = iMax[1];
	for (int r = cli.min[0]; r <= cli.max[0]; r++)
	{
		for (int c = cli.min[1]; c <= cli.max[1]; c++)
		{
			std::string cell = _key(r, c);
			m_cells[cell].insert(i);
		}
	}
}

void SpatialHashGrid::remove(int i)
{
	Client& cli = m_clients[i];
	for (int r = cli.min[0]; r <= cli.max[0]; r++)
	{
		for (int c = cli.min[1]; c <= cli.max[1]; c++)
		{
			std::string cell = _key(r, c);
			m_cells[cell].erase(i);
		}
	}
}

void SpatialHashGrid::deleteClient(int i)
{
	remove(i);
	m_clients.erase(i);
}

std::set<int> SpatialHashGrid::findNear(const Vec2f& position, float radius, std::set<int>& results)
{
	Vec2f min = { position.x - radius - m_origin.x, position.y - radius - m_origin.y };
	Vec2f max = { position.x + radius - m_origin.x, position.y + radius - m_origin.y };

	int iMin[2], iMax[2];
	_getCellIndex(min, iMin);
	_getCellIndex(max, iMax);

	for (int r = iMin[0]; r <= iMax[0]; r++)
	{
		for (int c = iMin[1]; c <= iMax[1]; c++)
		{
			std::string cell = _key(r, c);
			for (int id : m_cells[cell])
			{
				results.insert(id);
			}
		}
	}
	return results;
}

std::set<int> SpatialHashGrid::findNear(int i, std::set<int>& results)
{
	Client& cli = m_clients[i];

	for (int r = cli.min[0]; r <= cli.max[0]; r++)
	{
		for (int c = cli.min[1]; c <= cli.max[1]; c++)
		{
			std::string cell = _key(r, c);
			for (int id : m_cells[cell])
			{
				results.insert(id);
			}
		}
	}
	return results;
}