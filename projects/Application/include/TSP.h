#pragma once

#include <vector>
#include <list>
#include <unordered_map>

class Planet;

class TSP
{
public:
	struct PathData;

private:
	
	std::vector<std::vector<int>> m_weights;
	std::unordered_map<const Planet*, int> m_planetToKey;
	std::unordered_map<int, Planet*> m_keyToPlanet;

	PathData calculatePathWeight(int home, int current, std::list<int> shouldVisit) const;

public:
	struct PathData
	{
		int weight;
		std::list<int> path;
	};

	bool m_verbose;

	TSP();
	
	void createWeightMatrix(const std::vector<Planet*> &planets);
	std::vector<Planet*> shortestPath(const Planet& current, const Planet& home, const std::vector<Planet*> &planetsToVisit) const;
};