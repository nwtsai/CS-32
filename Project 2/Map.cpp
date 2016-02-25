#include "Map.h"
#include <iostream>
#include <cstdlib>

using namespace std;

// initialize a Map of size 0 where the head and tails are set to null pointers
Map::Map()
	:m_size(0), m_head(nullptr), m_tail(nullptr)
{
}

// delete the Map
Map::~Map()
{
	// only need to delete if list has items
	if (!empty())
	{
		// if there is only one item, (head = tail), then just delete that one item
		if (m_head == m_tail)
		{
			delete m_head;
		}
		// if there are multiple items, start a temporary node pointer at the head. 
		// As long as the item is not the last item, point temp to the next item and delete the previous item
		else
		{
			Node *temp = m_head;
			while (temp->next != nullptr)
			{
				temp = temp->next;
				delete temp->prev;
			}
			// once you exit while loop, delete the last item.
			delete m_tail;
		}	
	}
}

Map::Map(const Map &other)
{
	// set private members of map to default (0 and null)
	m_size = 0;
	m_head = nullptr;
	m_tail = nullptr;

	// loop through every item of the other map, get the contents, and insert into this map
	for (int i = 0; i < other.size(); i++)
	{
		KeyType k3y;
		ValueType valu3;

		other.get(i, k3y, valu3);
		insert(k3y, valu3);
	}
}

// equals operator
Map& Map::operator=(const Map &rhs)
{
	// checks if this and map have the same address
	if (this != &rhs)
	{
		// if different addresses, create a new map and swap this map with the new map
		Map newMap(rhs);
		swap(newMap);
	}
	// return this map
	return *this;
}

// for testing purposes only
void Map::dump() const
{
	for (Node *p = m_head; p != nullptr; p = p->next)
		cerr << p->pair.key << " " << p->pair.value << endl;
}

bool Map::empty() const
{
	// if head is not initialized, then there are no items.
	if (m_head == nullptr)
		return true; // return true if map is empty, otherwise return false
	return false;
}

int Map::size() const
{
	return m_size;
}

bool Map::insert(const KeyType& key, const ValueType& value)
{
	// if key is already in the list, do nothing and return false
	if (contains(key))
		return false;

	// create a new node, set its keys and values, set its next and prev to null
	Node *newguy = new Node;
	newguy->pair.key = key;
	newguy->pair.value = value;
	newguy->prev = nullptr;
	newguy->next = nullptr;

	// if the list is empty, set head pointer to the newguy, and set tail to the newguy
	if (empty())
	{
		m_head = newguy;
		m_tail = newguy;
	}
	else
	// if the list isn't empty, set newguy's pev to tail, then tail's next to newguy, and point tail to newguy
	{
		newguy->prev = m_tail;
		m_tail->next = newguy;
		m_tail = newguy;
	}
	
	// increment the size counter of the list and return true
	m_size++;
	return true;
}

// If key is equal to a key currently in the map, then make that key no
// longer map to the value it currently maps to, but instead map to
// the value of the second parameter; return true in this case.
// Otherwise, make no change to the map and return false.
bool Map::update(const KeyType& key, const ValueType& value)
{
	// if the key is found in the current list, find the key and set its value to the value parameter
	if (contains(key))
	{
		for (Node *p = m_head; p != nullptr; p = p->next)
		{
			if (p->pair.key == key)
			{
				p->pair.value = value;
				return true;
			}
		}
	}
	// return false if no key is found in the list
	return false;
}

bool Map::insertOrUpdate(const KeyType& key, const ValueType& value)
{
	// if the key is already in the list, update the list
	if (contains(key))
	{
		update(key, value);
	}
	// otherwise, if the key is not in the list, insert it into the list
	else
	{
		insert(key, value);
	}
	// this function always returns true
	return true;
}

bool Map::erase(const KeyType& key)
{
	// if key isn't in the list or the list is empty, do nothing and return false 
	if (!contains(key))
		return false;

	// if there is only one item in the list, delete the item, and set the head and tail pointers to null
	if (size() == 1)
	{
		delete m_head;
		m_head = nullptr;
		m_tail = nullptr;
		m_size--;
		return true;
	}

	// Loop through the list to find the item in the list with the corresponding key
	for (Node *p = m_head; p != nullptr; p = p->next)
	{
		if (p->pair.key == key)
		{
			// if it is the first in the list, set head to its next, set the prev of the next to null
			if (p == m_head)
			{
				m_head = p->next;
				delete p;
				m_head->prev = nullptr;
			}
			// if it is the last in the list, set tail to its prev
			else if (p == m_tail)
			{
				m_tail = p->prev;
				delete p;
				m_tail->next = nullptr;
			}
			// if it is any item in the middle of the list, 
			else
			{
				p->prev->next = p->next;
				p->next->prev = p->prev;
				delete p;
			}

			// decrement the size of the list and return true
			m_size--;
			return true;
		}
	}
	return false;
}

// if a key is found in the list, return true
bool Map::contains(const KeyType& key) const
{
	// if the list is empty, do nothing and return false
	if (empty())
		return false;

	// loop through list; if key is found, return true, otherwise return false
	for (Node *p = m_head; p != nullptr; p = p->next)
	{
		if (p->pair.key == key)
		{
			return true;
		}
	}
	return false;
}

// given a key, get the value that the key maps to
bool Map::get(const KeyType& key, ValueType& value) const
{
	// if key is found, find the key and then set value equal to the value that the key maps to
	if (contains(key))
	{
		for (Node *p = m_head; p != nullptr; p = p->next)
		{
			if (p->pair.key == key)
			{
				value = p->pair.value;
				return true;
			}
		}
	}
	// return false if no matching key is found in the list
	return false;
}

// given an int i, find the key and value of the ith item in the list
bool Map::get(int i, KeyType& key, ValueType& value) const
{
	// check to see if i is positive and within the size of the list
	if (i >= 0 && i < size())
	{
		Node *temp = m_head;
		// loop through until temp is the ith pointer in the list
		for (int k = 0; k < i; k++)
		{
			temp = temp->next;
		}

		// set key and value to the key and value that the ith pointer points to in the list
		key = temp->pair.key;
		value = temp->pair.value;

		return true;
	}
	// return false if i is negative or larger than the current size
	return false;
}

void Map::swap(Map& other)
{
	// create two Node temporary pointers and give them the value of head and tail
	Node *tempHead = m_head;
	Node *tempTail = m_tail;

	// set the head and tail equal to the other's head and tail
	m_head = other.m_head;
	m_tail = other.m_tail;

	// set the other's head and tail equal to the temporary pointers created earlier
	other.m_head = tempHead;
	other.m_tail = tempTail;

	// swap the sizes of the map by creating a temporary size variable
	int thisSize = size();
	m_size = other.size();
	other.m_size = thisSize;
}

// If a key appears in exactly one of m1 and m2, then result must contain a pair consisting of that key and its corresponding value.
// If a key appears in both m1 and m2, with the same corresponding value in both, then result must contain a pair with that key and value.
bool combine(const Map& m1, const Map& m2, Map& result)
{
	// to avoid aliasing, make sure different parameters are not referring to the same Map by creating two temps
	// use temporary variables because m1 and m2 are passed by reference
	Map tempM1 = m1;
	Map tempM2 = m2;

	// if result contains anything, go through each item and delete them
	while (!result.empty())
	{
		KeyType k;
		ValueType v;

		// everytime you loop through, delete the first item in the list, until every item is erased
		result.get(0, k, v);
		result.erase(k);
	}

	// return this boolean at the end of the function, checks to see if the values of the same key are the same
	bool AreTheVsTheSame = true;

	// loop through each item of m1
	for (int i = 0; i < tempM1.size(); i++)
	{
		KeyType k;
		ValueType v;
		// store the keys and values of the ith item in m1 into k and v
		tempM1.get(i, k, v);
		// if m2 does not contain the key found in m1, insert the key value pair into result
		if (!tempM2.contains(k))
		{
			result.insert(k, v);
		}
		else
		{
			// if m2 does contain the key found in m1, find the key and check its value
			ValueType vm2;
			tempM2.get(k, vm2);

			// if the values of the key match, insert it into result
			if (v == vm2)
			{
				result.insert(k, v);
			}
			else
			{
				// if the values of the same key do not match between the two lists, don't insert and return false later
				AreTheVsTheSame = false;
			}
		}
	}
	// Check each value of m2, and insert the items that are ONLY in m2 and not in m1
	for (int j = 0; j < tempM2.size(); j++)
	{
		KeyType xx;
		ValueType yy;

		tempM2.get(j, xx, yy);

		// if key of m2 does not show up in m1 and does not show up in the result map, then insert it into result
		if (!tempM1.contains(xx) && !result.contains(xx))
		{
			result.insert(xx, yy);
		}
	}
	return AreTheVsTheSame;
}

// When this function returns, result must contain a copy of all the pairs
// in m1 whose keys don't appear in m2; it must not contain any other pairs
void subtract(const Map& m1, const Map& m2, Map& result)
{
	// to avoid aliasing, make sure different parameters are not referring to the same Map by creating two temps
	// use temporary variables because m1 and m2 are passed by reference
	Map tempM1 = m1;
	Map tempM2 = m2;

	// if the result Map is not empty, proceed to erase each item in the list
	while (!result.empty())
	{
		KeyType k;
		ValueType v;

		result.get(0, k, v);
		result.erase(k);
	}

	// loop through each key in m1 and get each key and value in the list
	for (int i = 0; i < tempM1.size(); i++)
	{
		KeyType k;
		ValueType v;

		tempM1.get(i, k, v);
		// if the current key of m1 does not appear in m2, insert the pair of values into the result map
		if (!tempM2.contains(k))
		{
			result.insert(k, v);
		}
	}
}