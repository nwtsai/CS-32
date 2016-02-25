#include "WeightMap.h"

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

WeightMap::WeightMap()
// Create an empty weight map.
{
}

bool WeightMap::enroll(KeyType name, ValueType startWeight)
// If a person with the given name is not currently in the map, 
// there is room in the map, and the startWeight is not negative,
// add an entry for that person and weight and return true.
// Otherwise make no change to the map and return false.
{
	if (m_map.contains(name) == false && m_map.size() < DEFAULT_MAX_ITEMS && startWeight >= 0)
	{
		return m_map.insert(name, startWeight);
	}
	return false;
}

double WeightMap::weight(KeyType name) const
// If a person with the given name is in the map, return that
// person's weight; otherwise, return -1.
{
	if (m_map.contains(name))
	{
		ValueType weight;

		m_map.get(name, weight);
		return weight;
	}
	return -1;
}

bool WeightMap::adjustWeight(KeyType name, ValueType amt)
// If no person with the given name is in the map or if
// weight() + amt is negative, make no change to the map and return
// false.  Otherwise, change the weight of the indicated person by
// the given amount and return true.  For example, if amt is -8.2,
// the person loses 8.2 pounds; if it's 3.7, the person gains 3.7
// pounds.
{
	if (m_map.contains(name) == false || (weight(name) + amt) < 0)
	{
		return false;
	}

	ValueType weight;
	m_map.get(name, weight);
	m_map.update(name, weight + amt);
	return true;
}

int WeightMap::size() const  // Return the number of people in the WeightMap.
{
	return m_map.size();
}

void WeightMap::print() const
// Write to cout one line for every person in the map.  Each line
// has the person's name, followed by one space, followed by that
// person's weight.
{
	for (int i = 0; i < size(); i++)
	{
		KeyType name;
		ValueType weight;
		m_map.get(i, name, weight);

		cout << name << " " << weight << endl;
	}
}