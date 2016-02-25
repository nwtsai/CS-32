#include "newMap.h"

#include <iostream>
#include <string>
#include <cstdlib>

using namespace std;

Map::Map()  
{
	m_index = -1;
	m_maxSize = DEFAULT_MAX_ITEMS;
	m_grid = new grid[DEFAULT_MAX_ITEMS];
}

Map::Map(int n)
{
	if (n < 0)
	{
		cout << "The Map size cannot be a negative integer." << endl;
		exit(1);
	}

	m_index = -1;
	m_maxSize = n;
	m_grid = new grid[m_maxSize];
}

Map::Map(const Map& map)
{
	m_maxSize = map.m_maxSize;
	m_index = -1;
	m_grid = new grid[m_maxSize];

	for (int i = 0; i < map.size(); i++)
	{
		KeyType key;
		ValueType value;

		map.get(i, key, value);
		insert(key, value);
	}
}

Map::~Map()
{
	delete[] m_grid;
}

Map& Map::operator=(const Map& someMap)
{
	if (this != &someMap)
	{
		Map tempMap(someMap);

		m_maxSize = tempMap.m_maxSize;
		m_index = -1;
		m_grid = new grid[tempMap.m_maxSize];

		for (int i = 0; i < tempMap.size(); i++)
		{
			KeyType key;
			ValueType value;

			tempMap.get(i, key, value);
			insert(key, value);
		}
	}
	return *this;
}

bool Map::empty() const  // Return true if the Map is empty, otherwise false.
{
	if (size() == 0)
		return true;
	return false;
}

int Map::size() const    // Return the number of key/value pairs in the Map.
{
	return m_index + 1;
}

bool Map::insert(const KeyType& key, const ValueType& value)
// If key is not equal to any key currently in the Map, and if the
// key/value pair can be added to the Map, then do so and return true.
// Otherwise, make no change to the Map and return false (indicating
// that either the key is already in the Map, or the Map has a fixed
// capacity and is full).
{
	if (size() >= m_maxSize)
		return false;

	//if (m_index >= m_maxSize - 1)
		//return false;

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
// If key is equal to a key currently in the Map, then make that key no
// longer Map to the value it currently Maps to, but instead Map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the Map and return false.
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
// If key is equal to a key currently in the Map, then make that key no
// longer Map to the value it currently Maps to, but instead Map to
// the value of the second parameter; return true in this case.
// If key is not equal to any key currently in the Map and if the
// key/value pair can be added to the Map, then do so and return true.
// Otherwise, make no change to the Map and return false (indicating
// that the key is not already in the Map and the Map has a fixed
// capacity and is full).
{
	if (update(key, value))
		return true;
	else
		return insert(key, value);

}

bool Map::erase(const KeyType& key)
// If key is equal to a key currently in the Map, remove the key/value
// pair with that key from the Map and return true.  Otherwise, make
// no change to the Map and return false.
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
			m_grid[i].key = m_grid[size() - 1].key;
			m_grid[i].value = m_grid[size() - 1].value;
			m_index--;
			return true;
		}
	}
	return false;
}

bool Map::contains(const KeyType& key) const
// Return true if key is equal to a key currently in the Map, otherwise
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
// If key is equal to a key currently in the Map, set value to the
// value in the Map that that key Maps to, and return true.  Otherwise,
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
// key and value of one of the key/value pairs in the Map and return
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
// Exchange the contents of this Map with the other one.
{
	Map temporaryMap = other;
	other = *this;
	*this = temporaryMap;
}