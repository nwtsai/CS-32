#ifndef NEWMAP_INCLUDED
#define NEWMAP_INCLUDED

#include <string>

const int DEFAULT_MAX_ITEMS = 200;

 typedef std::string KeyType;   // define Keytype as a synonym for string
 typedef double ValueType; // define ValueType as a synonym for double

class Map
{
  public:
	Map();         // Create an empty Map (i.e., one with no key/value pairs)

	Map(int n);

	Map(const Map& map);

	~Map();

	Map& operator=(const Map& rhs);

	bool empty() const;  // Return true if the Map is empty, otherwise false.

	int size() const;    // Return the number of key/value pairs in the Map.

	bool insert(const KeyType& key, const ValueType& value);
	// If key is not equal to any key currently in the Map, and if the
	// key/value pair can be added to the Map, then do so and return true.
	// Otherwise, make no change to the Map and return false (indicating
	// that either the key is already in the Map, or the Map has a fixed
	// capacity and is full).

	bool update(const KeyType& key, const ValueType& value);
	// If key is equal to a key currently in the Map, then make that key no
	// longer Map to the value it currently Maps to, but instead Map to
	// the value of the second parameter; return true in this case.
	// Otherwise, make no change to the Map and return false.

	bool insertOrUpdate(const KeyType& key, const ValueType& value);
	// If key is equal to a key currently in the Map, then make that key no
	// longer Map to the value it currently Maps to, but instead Map to
	// the value of the second parameter; return true in this case.
	// If key is not equal to any key currently in the Map and if the
	// key/value pair can be added to the Map, then do so and return true.
	// Otherwise, make no change to the Map and return false (indicating
	// that the key is not already in the Map and the Map has a fixed
	// capacity and is full).

	bool erase(const KeyType& key);
	// If key is equal to a key currently in the Map, remove the key/value
	// pair with that key from the Map and return true.  Otherwise, make
	// no change to the Map and return false.

	bool contains(const KeyType& key) const;
	// Return true if key is equal to a key currently in the Map, otherwise
	// false.

	bool get(const KeyType& key, ValueType& value) const;
	// If key is equal to a key currently in the Map, set value to the
	// value in the Map that that key Maps to, and return true.  Otherwise,
	// make no change to the value parameter of this function and return
	// false.

	bool get(int i, KeyType& key, ValueType& value) const;
	// If 0 <= i < size(), copy into the key and value parameters the
	// key and value of one of the key/value pairs in the Map and return
	// true.  Otherwise, leave the key and value parameters unchanged and
	// return false.  (See below for details about this function.)

	void swap(Map& other);
	// Exchange the contents of this Map with the other one.

  private:
	int m_index;
	int m_maxSize;

	struct grid
	{
		KeyType key;
		ValueType value;
	};

	grid* m_grid;
};
#endif