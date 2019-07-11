#include "TSP.h"
#include "drawables/Planet.h"

#include <set>

std::ostream & operator<< (std::ostream &os, const std::list<int> &list)
{
	for (int i : list)
	{
		os << i << ", ";
	}
	return os;
}

TSP::PathData TSP::calculatePathWeight(int home, int current, std::list<int> shouldVisit) const
{
	PathData data;

	if (shouldVisit.size() == 1)
	{
		data.weight = m_weights[current][shouldVisit.front()];
		data.weight += m_weights[home][shouldVisit.front()];
		data.path.push_front(home);
		data.path.push_front(shouldVisit.front());
		data.path.push_front(current);
	}
	else
	{
		data.weight = INT_MAX;

		for (size_t i = 0; i < shouldVisit.size(); i++)
		{
			if (m_verbose)
			{
				for (size_t j = shouldVisit.size(); j < m_weights.size(); j++)
					std::cout << "- ";
				std::cout << current << " | " << shouldVisit << '\n';
			}

			PathData temp = calculatePathWeight(home, shouldVisit.front(), std::list<int>(++shouldVisit.begin(), shouldVisit.end()));
			temp.weight += m_weights[current][shouldVisit.front()];

			// Check if current is smaller then previous
			if (temp.weight < data.weight)
			{
				data = temp;
			}

			// Rotate the list
			int old = shouldVisit.front();
			shouldVisit.pop_front();
			shouldVisit.push_back(old);
		}

		data.path.push_front(current);
	}
	return data;
}

TSP::TSP():
	m_verbose(false)
{
}

void TSP::createWeightMatrix(const std::vector<Planet*>& planets)
{
	m_weights.clear();
	m_planetToKey.clear();
	m_keyToPlanet.clear();

	for (size_t i = 0; i < planets.size(); i++)
	{
		m_planetToKey[planets[i]] = i;
		m_keyToPlanet[i] = planets[i];
	}

	for (size_t i = 0; i < planets.size(); i++)
	{
		m_weights.emplace_back();

		for (size_t j = 0; j < planets.size(); j++)
		{
			const int distance = static_cast<int>(glm::distance(planets[i]->pos(), planets[j]->pos()));
			m_weights[i].push_back(distance);
			
			if(m_verbose)
			std::cout << distance << '\t';
		}
		if(m_verbose)
		std::cout << '\n';
	}
}

std::vector<Planet*> TSP::shortestPath(const Planet& current, const Planet& home, const std::vector<Planet*> &planetsToVisit) const
{
	// Make sure that only unique addresses are passed
	std::vector<Planet*> planetPointers;
	{
		std::set<Planet*> uniquePlanets;
		uniquePlanets.insert(planetsToVisit.begin(), planetsToVisit.end());
		assert(uniquePlanets.size() == planetsToVisit.size());
	
		planetPointers.insert(planetPointers.begin(), uniquePlanets.begin(), uniquePlanets.end());
	}

	std::list<int> planetIndices;

	// Convert adresses to indices
	for (Planet * address : planetPointers)
	{
		// Doesn't work because I need the index
		//const std::vector<std::vector<Node>>::const_iterator iter = 
		//	std::find_if(m_weights.begin(),m_weights.end(),
		//	[address](const std::vector<Node> &node) -> bool {
		//	return node[0].planet == address; });

		if (address == &home)
			continue;

		const auto key = m_planetToKey.find(address);
		if (key != m_planetToKey.end())
		{
			planetIndices.push_back(key->second);
		}
		else
		{
			std::cout << "TSP couldn't convert address to index\n";
			return {};
		}
	}
	
	if (planetIndices.empty())
		return {};

	PathData data = calculatePathWeight(m_planetToKey.at(&home), m_planetToKey.at(&current), planetIndices);
	std::cout << data.path << '\n' << "Weight " << data.weight << '\n';

	// Convert integers back to addresses
	std::vector<Planet*> returner;
	for (int key : data.path)
	{
		returner.push_back(m_keyToPlanet.at(key));
	}

	return returner;
}