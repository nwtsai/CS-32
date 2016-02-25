#include "Map.h"

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

Map::Map()  // Create an empty map (i.e., one with no key/value pairs)
{	
	m_index = -1;
}

bool Map::empty() const  // Return true if the map is empty, otherwise false.
{
	if (size() == 0)
		return true;
	return false;
}

int Map::size() const    // Return the number of key/value pairs in the map.
{
	return m_index + 1;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the map, and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that either the key is already in the map, or the map has a fixed
// capacity and is full).
{
	if (size() >= DEFAULT_MAX_ITEMS)
		return false;

	for (int i = 0; i < size(); i++)
	{
		if (m_grid[i].key == key)
		{
			return false;
		}
	}

	m_index++;
	m_grid[m_index].key = key;
	m_grid[m_index].value = value;
	return true;
}

bool Map::update(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
{
	for (int i = 0; i < size(); i++)
	{
		if (m_grid[i].key == key)
		{
			m_grid[i].value = value;
			return true;
		}
	}
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the map and if the
// key/value pair can be added to the map, then do so and return true.
// Otherwise, make no change to the map and return false (indicating
// that the key is not already in the map and the map has a fixed
// capacity and is full).
{
	if (update(key, value))
		return true;
	else
		return insert(key, value);

}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the map, remove the key/value
// pair with that key from the map and return true.  Otherwise, make
// no change to the map and return false.
{
	if (m_index <= 0)
	{
		if (m_grid[0].key == key && !empty())
		{
			m_index--;
			return true;
		}
		return false;
	}

	for (int i = 0; i < size(); i++)
	{
		if (m_grid[i].key == key)
		{
			m_grid[i].key = m_grid[size()].key;
			m_grid[i].value = m_grid[size()].value;
			m_index--;
			return true;
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the map, otherwise
// false.
{
	if (empty())
		return false;

	for (int i = 0; i < size(); i++)
	{
		if (m_grid[i].key == key)
			return true;
	}
	return false;
}

bool Map::get(const KeyType& key, ValueType& value) const
// If key is equal to a key currently in the map, set value to the
// value in the map that that key maps to, and return true.  Otherwise,
// make no change to the value parameter of this function and return
// false.
{
	if (contains(key))
	{
		for (int i = 0; i < size(); i++)
		{
			if (m_grid[i].key == key)
			{
				value = m_grid[i].value;
				return true;
			}
		}
	}
	return false;
}

bool Map::get(int i, KeyType& key, ValueType& value) const
// If 0 <= i < size(), copy into the key and value parameters the
// key and value of one of the key/value pairs in the map and return
// true.  Otherwise, leave the key and value parameters unchanged and
// return false.  (See below for details about this function.)
{
	if (i >= 0 && i < size())
	{
		key = m_grid[i].key;
		value = m_grid[i].value;
		return true;
	}
	return false;
}

void Map::swap(Map& other)
// Exchange the contents of this map with the other one.
{
	// Map temp = other;
	// other = *this;
	// *this = temp;

	grid temp[DEFAULT_MAX_ITEMS];
	int thisSize = size();
	int otherSize = other.size();

	// Puts all keys and values of the other grid into the temporary grid
	for (int i = 0; i < otherSize; i++)
	{
		KeyType otherKey;
		ValueType otherValue;

		other.get(i, otherKey, otherValue);
		temp[i].key = otherKey;
		temp[i].value = otherValue;
	}

	// Erase the other grid.
	while (other.empty() == false)
	{
		KeyType otherKey;
		ValueType otherValue;

		other.get(0, otherKey, otherValue);
		other.erase(otherKey);
	}

	// get all info from this Map and insert into the other Map
	for (int i = 0; i < thisSize; i++)
	{
		KeyType key;
		ValueType value;

		get(i, key, value);
		other.insert(key, value);
	}

	// Erase this grid.
	while (empty() == false)
	{
		KeyType key;
		ValueType value;

		get(0, key, value);
		erase(key);
	}

	// Move values from temp into this Map
	for (int i = 0; i < otherSize; i++)
	{
		insert(temp[i].key, temp[i].value);
	}
}