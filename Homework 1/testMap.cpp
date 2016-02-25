#include "Map.h"
#include "WeightMap.h"
#include <iostream>
#include <cassert>
using namespace std;

int main()
{
	Map m1;
	m1.insert("Fred", 2.956);
	Map m2;
	m2.insert("Ethel", 3.538);
	m2.insert("Lucy", 2.956);
	m1.insert("Freddu", 2.567);
	m2.insert("Jane", 6.765);
	m1.swap(m2);
	assert(m1.size() == 3); 
	assert(m1.contains("Ethel"));
	assert(m1.contains("Lucy"));
	assert(m2.size() == 2);
	assert(m2.contains("Fred"));
	assert(m2.contains("Freddu"));
	assert(m1.contains("Jane"));
}